# Kodi for OpenWrt

Kodi 22.0 RC2 (tag `22.0rc2-Piers`, commit `d686683a`) built for the GBM (bare KMS/DRM) and Wayland windowing
systems with GLES rendering. X11 and framebuffer are not supported.

This package supersedes the previous Kodi 21.3 Omega package under the
same name; package name, `KODI_*` config symbols and installed paths are
unchanged. It links against `libffmpeg` from the packages feed's `ffmpeg`
package (FFmpeg 9.0.1); Kodi 22 is the version tracking current FFmpeg.

## Running

- On bare KMS/DRM: `/etc/init.d/kodi start` launches kodi-standalone
  with GBM windowing via procd.
- As a Wayland kiosk: `kodi-cage` runs Kodi fullscreen inside the
  cage compositor.

## FFmpeg

FFmpeg is discovered through stock pkg-config from the standard staging
locations; nothing special is needed on the Kodi side.

Kodi requires libpostproc, which upstream FFmpeg dropped in 8.0 - the
`ffmpeg` package re-adds it through LibreELEC's `postproc` patch, and
the dependency on `FFMPEG_GPL` keeps it enabled.

H.264, HEVC and VC-1 need `BUILD_PATENTED`, for hardware decoding
through V4L2 Request/DRM_PRIME as much as for software decoding. A
V4L2 Request hwaccel is an extension of the native decoder, which
parses the bitstream and drives the hardware from its own slice
callbacks, so `hevc_v4l2request_hwaccel` selects `hevc_decoder` and is
dropped together with it. The stateful V4L2 M2M path is what remains
without the option, its `*_v4l2m2m` decoders being components in their
own right; stateless hardware (rkvdec, cedrus, hantro) is reached only
through V4L2 Request.

## DRM_PRIME / hardware decoding

`patches/201` and `patches/202` are LibreELEC's `drmprime-filter`
patches (their 2026-08-22 rebase, carried on top of the decoder
fallback of `patches/127`): they let the DRM_PRIME video path run FFmpeg filters, which is
what drives the `deinterlace_v4l2m2m` filter (sun50i-di on H6) for
interlaced content.

The 21.3 package additionally carried a backport that created a
hwdevice context for non-DRM device types (V4L2REQUEST); that change is
upstream in master and no longer needed here.

## Python bindings

Kodi generates the Python API bindings at build time with SWIG's own
Python backend and needs SWIG 4.5.0 or newer. They are generated during
the normal build by `swig/host` from the packages feed and the host
Python from staging; nothing is pre-generated or downloaded.

## Patches

The patches come in three ranges, exported in this order from the
`wayland-host` branch of https://github.com/dangowrt/xbmc, which stacks
them on the release tag:

- `0xx`: changes merged upstream after the release tag, backported.
  None at present.
- `1xx`: changes in upstream pull requests that are still open, each
  taken from its pull request branch on Kodi master and adapted to the
  release where needed, in dependency order: `101` (xbmc/xbmc#29962),
  `102` (GBM immutable properties), `103` (xbmc/xbmc#29346), `104`
  (xbmc/xbmc#29347), `105` to `107` (xbmc/xbmc#29348), `108` to `111`
  (xbmc/xbmc#29899), `112` and `113` (xbmc/xbmc#30283), `114` and `115`
  (GBM output selection), `116` to `119` (musl), `120` to `123` (cross
  builds), `124` to `126` (device volume), `127` (DRM PRIME decoder
  fallback), `128` (stop video when the display switches off), `129` to
  `131` (GBM atomic fallback request), `132` and `133` (GL rendering of a
  video the video plane refuses), and the web apps with their Wayland
  host and web player last, `134` to `145` (xbmc/xbmc#30315), which
  needs `101` and `102`, and `132` and `133` for the same fallback of
  web video.
- `2xx`: changes kept downstream: LibreELEC's DRM_PRIME filter patches
  (`201`, `202`, see above), the per-add-on uplinks (`203` to `216`),
  LibreELEC's patch that keeps the target build from cross-building its
  own TexturePacker (`217`), and the add-ons the OpenWrt packages
  install enabled on first start (`218`, `219`).

## libdvdread / libdvdnav

Kodi builds libdvdread and libdvdnav itself (forced on Linux, it needs
its patched libdvdnav) as meson sub-builds. The archives are declared as
additional OpenWrt downloads in the Makefile (`Download/libdvdread`,
`Download/libdvdnav`, sha256-pinned) and passed to cmake as local
`LIBDVDREAD_URL`/`LIBDVDNAV_URL`, so `make download` fetches them and
the build itself needs no network. Kodi's own SHA512 check from
`tools/depends/target/*/*-VERSION` still applies. meson is OpenWrt's
host zipapp (`staging_dir/host/bin/meson.py`), passed explicitly as
`MESON_EXECUTABLE`. libdvdcss stays off.

## crossguid

Kodi links the `crossguid` package (`ENABLE_INTERNAL_CROSSGUID=OFF`).
