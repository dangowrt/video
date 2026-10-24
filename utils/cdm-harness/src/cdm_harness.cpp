/*
 * cdm-harness: standalone de-risk harness for the T6 (EME/Widevine) backend.
 *
 * Drives inputstream.adaptive's media::CdmAdapter against a Chromium CDM shared
 * object (the ChromeOS Widevine blob) to prove, on the box under gcompat, that:
 *   - the blob loads (dlopen) and its versioned entrypoints bind,
 *   - CreateCdmInstance(10) succeeds and GetCdmVersion returns,
 *   - Initialize completes (Host::OnInitialized),
 *   - CreateSessionAndGenerateRequest yields a session message (a licence
 *     request challenge, or an individualisation/provisioning request when the
 *     base path is unprovisioned), and
 *   - provisioning storage (Host::CreateFileIO) is exercised.
 *
 * It does NOT perform a real Decrypt: that needs a licence from a licence
 * server and a full key exchange, which is out of scope for a mechanism proof.
 *
 * This is throwaway-grade proof code, not the shipped backend.
 */

#include "cdm/media/cdm/cdm_adapter.h"
#include "cdm/debug.h"

#include <condition_variable>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include <dirent.h>
#include <unistd.h>

namespace {

const char DEFAULT_BLOB[] = "/srv/kodi/cdm/libwidevinecdm.so";

const uint8_t WIDEVINE_SYSTEM_ID[16] = {
    0xed, 0xef, 0x8b, 0xa9, 0x79, 0xd6, 0x4a, 0xce,
    0xa3, 0xc8, 0x27, 0xdc, 0xd5, 0x1d, 0x21, 0xed};

const uint8_t TEST_KEY_ID[16] = {
    0x00, 0x11, 0x22, 0x33, 0x44, 0x55, 0x66, 0x77,
    0x88, 0x99, 0xaa, 0xbb, 0xcc, 0xdd, 0xee, 0xff};

const char* msg_name(media::CdmAdapterClient::CDMADPMSG msg)
{
  switch (msg)
  {
    case media::CdmAdapterClient::kError: return "kError";
    case media::CdmAdapterClient::kSessionMessage: return "kSessionMessage";
    case media::CdmAdapterClient::kSessionExpired: return "kSessionExpired";
    case media::CdmAdapterClient::kSessionKeysChange: return "kSessionKeysChange";
    case media::CdmAdapterClient::kSessionClosed: return "kSessionClosed";
    case media::CdmAdapterClient::kLegacySessionError: return "kLegacySessionError";
    default: return "unknown";
  }
}

const char* key_status_name(media::CdmAdapterClient::CDMKeyStatus status)
{
  switch (status)
  {
    case media::CdmAdapterClient::CDMKeyStatus::kUsable: return "usable";
    case media::CdmAdapterClient::CDMKeyStatus::kInternalError: return "internal-error";
    case media::CdmAdapterClient::CDMKeyStatus::kExpired: return "expired";
    case media::CdmAdapterClient::CDMKeyStatus::kOutputRestricted: return "output-restricted";
    case media::CdmAdapterClient::CDMKeyStatus::kOutputDownscaled: return "output-downscaled";
    case media::CdmAdapterClient::CDMKeyStatus::kStatusPending: return "status-pending";
    case media::CdmAdapterClient::CDMKeyStatus::kReleased: return "released";
    case media::CdmAdapterClient::CDMKeyStatus::kUsableInFuture: return "usable-in-future";
    default: return "unknown";
  }
}

void hex_dump(const char* label, const uint8_t* data, size_t size)
{
  fprintf(stderr, "%s", label);
  for (size_t i = 0; i < size; ++i)
    fprintf(stderr, "%02x", data[i]);
  fprintf(stderr, "\n");
}

void adapter_log(const CDM_DBG::LogLevel level, const char* msg)
{
  fprintf(stderr, "[adapter:%d] %s\n", static_cast<int>(level), msg);
}

/*
 * Heap-backed cdm::Buffer, modelled on inputstream.adaptive's CdmBuffer:
 * a thin owner over a plain allocation the CDM fills and we hand back.
 */
class HeapBuffer : public cdm::Buffer
{
public:
  explicit HeapBuffer(size_t capacity) : m_capacity(capacity)
  {
    m_data = capacity ? static_cast<uint8_t*>(malloc(capacity)) : nullptr;
  }

  void Destroy() override { delete this; }
  uint32_t Capacity() const override { return static_cast<uint32_t>(m_capacity); }
  uint8_t* Data() override { return m_data; }
  void SetSize(uint32_t size) override { m_size = size; }
  uint32_t Size() const override { return m_size; }

private:
  ~HeapBuffer() override { free(m_data); }

  uint8_t* m_data{nullptr};
  size_t m_capacity{0};
  uint32_t m_size{0};
};

class HarnessClient : public media::CdmAdapterClient
{
public:
  void OnCDMMessage(const char* session,
                    uint32_t session_size,
                    CDMADPMSG msg,
                    const uint8_t* data,
                    size_t data_size,
                    uint32_t status) override
  {
    std::string sid(session ? session : "", session ? session_size : 0);
    fprintf(stderr, "[client] OnCDMMessage session='%s' msg=%s data_size=%zu status=%u\n",
            sid.c_str(), msg_name(msg), data_size, status);

    if (msg != kSessionMessage)
      return;

    std::lock_guard<std::mutex> lock(m_mx);
    m_message_size = data_size;
    m_message_data.assign(data, data + data_size);
    m_got_message = true;
    m_cv.notify_all();
  }

  void OnKeyStatusChange(const std::string& session_id,
                         const std::vector<KeyInfo>& keys_info) override
  {
    fprintf(stderr, "[client] OnKeyStatusChange session='%s' keys=%zu\n",
            session_id.c_str(), keys_info.size());
    for (const KeyInfo& key : keys_info)
    {
      hex_dump("[client]   kid=", key.kid.data(), key.kid.size());
      fprintf(stderr, "[client]   status=%s\n", key_status_name(key.status));
    }
  }

  cdm::Buffer* AllocateBuffer(size_t size) override
  {
    fprintf(stderr, "[client] AllocateBuffer %zu\n", size);
    return new HeapBuffer(size);
  }

  bool message_wait(int timeout_s, size_t* out_size)
  {
    std::unique_lock<std::mutex> lock(m_mx);
    bool ok = m_cv.wait_for(lock, std::chrono::seconds(timeout_s),
                            [this] { return m_got_message; });
    if (ok && out_size)
      *out_size = m_message_size;
    return ok;
  }

  const std::vector<uint8_t>& message_data() const { return m_message_data; }

private:
  std::mutex m_mx;
  std::condition_variable m_cv;
  bool m_got_message{false};
  size_t m_message_size{0};
  std::vector<uint8_t> m_message_data;
};

std::vector<uint8_t> pssh_build(const uint8_t* key_id, size_t key_id_size)
{
  std::vector<uint8_t> data;
  data.push_back(0x08);
  data.push_back(0x01);
  data.push_back(0x12);
  data.push_back(static_cast<uint8_t>(key_id_size));
  data.insert(data.end(), key_id, key_id + key_id_size);

  const uint32_t box_size = 8 + 4 + 16 + 4 + static_cast<uint32_t>(data.size());

  std::vector<uint8_t> box;
  box.push_back((box_size >> 24) & 0xff);
  box.push_back((box_size >> 16) & 0xff);
  box.push_back((box_size >> 8) & 0xff);
  box.push_back(box_size & 0xff);
  box.push_back('p');
  box.push_back('s');
  box.push_back('s');
  box.push_back('h');
  box.push_back(0x00);
  box.push_back(0x00);
  box.push_back(0x00);
  box.push_back(0x00);
  box.insert(box.end(), WIDEVINE_SYSTEM_ID, WIDEVINE_SYSTEM_ID + 16);
  const uint32_t data_size = static_cast<uint32_t>(data.size());
  box.push_back((data_size >> 24) & 0xff);
  box.push_back((data_size >> 16) & 0xff);
  box.push_back((data_size >> 8) & 0xff);
  box.push_back(data_size & 0xff);
  box.insert(box.end(), data.begin(), data.end());
  return box;
}

bool file_read(const char* path, std::vector<uint8_t>* out)
{
  FILE* fp = fopen(path, "rb");
  if (!fp)
    return false;
  fseek(fp, 0, SEEK_END);
  long size = ftell(fp);
  fseek(fp, 0, SEEK_SET);
  if (size <= 0)
  {
    fclose(fp);
    return false;
  }
  out->resize(static_cast<size_t>(size));
  size_t read = fread(out->data(), 1, out->size(), fp);
  fclose(fp);
  return read == out->size();
}

void dir_list(const char* path)
{
  DIR* dir = opendir(path);
  if (!dir)
  {
    fprintf(stderr, "[harness] base path '%s' not created (no provisioning writes)\n", path);
    return;
  }
  fprintf(stderr, "[harness] base path '%s' contents:\n", path);
  struct dirent* entry;
  while ((entry = readdir(dir)) != nullptr)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;
    fprintf(stderr, "[harness]   %s\n", entry->d_name);
  }
  closedir(dir);
}

}  // namespace

int main(int argc, char** argv)
{
  const char* blob_path = argc > 1 ? argv[1] : DEFAULT_BLOB;
  const char* base_path = argc > 2 ? argv[2] : "/tmp/cdm-harness-store/";
  const char* init_data_path = argc > 3 ? argv[3] : nullptr;
  const int timeout_s = argc > 4 ? atoi(argv[4]) : 30;

  CDM_DBG::SetDBGMsgCallback(adapter_log);

  fprintf(stderr, "[harness] blob='%s' base='%s' init_data='%s' timeout=%ds\n",
          blob_path, base_path, init_data_path ? init_data_path : "(embedded)", timeout_s);

  HarnessClient client;

  /*
   * CdmAdapter calls shared_from_this() in SetTimer, so it must be owned by a
   * shared_ptr or that call throws std::bad_weak_ptr. CdmConfig mirrors the
   * Widevine L3 configuration inputstream.adaptive uses: no distinctive
   * identifier, persistent state on (required for provisioning storage via
   * CreateFileIO), no hardware-secure codecs.
   */
  auto adapter = std::make_shared<media::CdmAdapter>(
      "com.widevine.alpha", blob_path, base_path,
      media::CdmConfig(false, true, false), &client);

  if (!adapter->LoadCDM())
  {
    fprintf(stderr, "[harness] FAIL: LoadCDM (dlopen or symbol bind failed)\n");
    return 1;
  }
  fprintf(stderr, "[harness] LoadCDM ok; CDM version='%s'\n", adapter->GetVersion().c_str());

  if (!adapter->Initialize())
  {
    fprintf(stderr, "[harness] FAIL: Initialize (CreateCdmInstance/Initialize/OnInitialized)\n");
    return 1;
  }
  fprintf(stderr, "[harness] Initialize ok\n");

  std::vector<uint8_t> init_data;
  if (init_data_path)
  {
    if (!file_read(init_data_path, &init_data))
    {
      fprintf(stderr, "[harness] FAIL: cannot read init data file '%s'\n", init_data_path);
      return 1;
    }
    fprintf(stderr, "[harness] init data from file, %zu bytes\n", init_data.size());
  }
  else
  {
    init_data = pssh_build(TEST_KEY_ID, sizeof(TEST_KEY_ID));
    fprintf(stderr, "[harness] embedded Widevine CENC PSSH, %zu bytes\n", init_data.size());
  }
  hex_dump("[harness] init data=", init_data.data(), init_data.size());

  std::future<std::string> session_future = adapter->PrepareSessionFuture(1);
  adapter->CreateSessionAndGenerateRequest(1, cdm::SessionType::kTemporary,
                                           cdm::InitDataType::kCenc, init_data.data(),
                                           static_cast<uint32_t>(init_data.size()));

  bool session_resolved = false;
  if (session_future.wait_for(std::chrono::seconds(timeout_s)) == std::future_status::ready)
  {
    try
    {
      std::string session_id = session_future.get();
      session_resolved = true;
      fprintf(stderr, "[harness] session resolved, id='%s'\n", session_id.c_str());
    }
    catch (const std::exception& e)
    {
      fprintf(stderr, "[harness] session promise rejected: %s\n", e.what());
    }
  }
  else
  {
    fprintf(stderr, "[harness] session promise not resolved within %ds\n", timeout_s);
  }

  size_t challenge_size = 0;
  bool got_message = client.message_wait(timeout_s, &challenge_size);

  dir_list(base_path);

  /*
   * The Widevine CDM keeps background timer threads alive via the adapter's
   * std::async tasks. Detach the client so no late callback reaches it, then
   * _exit with the result code to avoid a teardown race with those threads
   * muddying the exit status of a short-lived proof.
   */
  adapter->RemoveClient();

  int code = 2;
  if (got_message)
  {
    fprintf(stderr, "[harness] RESULT: session message produced, %zu bytes\n", challenge_size);
    hex_dump("[harness] challenge=", client.message_data().data(), client.message_data().size());
    fprintf(stderr, "[harness] NOTE: cdm_adapter collapses cdm::MessageType into "
                    "kSessionMessage; a licence-request challenge and a "
                    "provisioning (individualisation) request are both reported "
                    "here. Inspect the base path contents above for provisioning.\n");
    fprintf(stderr, "[harness] PASS: blob load, CreateCdmInstance, GetCdmVersion, "
                    "Initialize, session message all proven. No Decrypt attempted.\n");
    code = 0;
  }
  else
  {
    fprintf(stderr, "[harness] RESULT: no session message within %ds "
                    "(session_resolved=%d)\n", timeout_s, session_resolved);
  }

  fflush(nullptr);
  _exit(code);
}
