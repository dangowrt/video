# Kodi for OpenWrt

Kodi 22.0 beta 2 (tag `22.0b2-Piers`, commit `e513e0ff`) built for the GBM (bare KMS/DRM) and Wayland windowing
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
the dependency on `FFMPEG_GPL` keeps it enabled. Software
decoding of H.264/HEVC/VC-1 additionally requires `BUILD_PATENTED`;
without it, hardware decoding (V4L2 M2M and V4L2 Request/DRM_PRIME)
still works.

## DRM_PRIME / hardware decoding

`patches/140` and `patches/141` are LibreELEC's `drmprime-filter`
patches (their 2026-08-22 rebase, applied unchanged): they let the DRM_PRIME video path run FFmpeg filters, which is
what drives the `deinterlace_v4l2m2m` filter (sun50i-di on H6) for
interlaced content.

The 21.3 package additionally carried a backport that created a
hwdevice context for non-DRM device types (V4L2REQUEST); that change is
upstream in master and no longer needed here.

## Pre-generated Python bindings

Upstream generates the Python API bindings at build time using SWIG and
a Groovy code generator, which requires a Java runtime plus three
archive downloads during cmake configure. To keep Java and the network
out of the build, this package ships bindings generated once per
snapshot in `files/kodi-python-bindings-22.0-e513e0ff.tar.xz`, consumed
via the `PYTHON_BINDINGS_DIR` cmake variable added by
`patches/200-support-pregenerated-python-bindings.patch`.

The generated sources are plain C++ against the CPython API, are
architecture independent and depend only on the Kodi source tree, so
they are reproducible for a given commit.

To regenerate after a version bump, on a host with swig, a JRE (11 or
newer), curl and unzip - e.g.:

```
./files/generate-python-bindings.sh <kodi-source-tree> 22.0_beta2-<commithash>
```

Keep the groovy/commons-lang/commons-text versions in that script in
sync with `xbmc/interfaces/swig/CMakeLists.txt`. Put the resulting
tarball into `files/` and update `PYTHON_BINDINGS_TARBALL` in the
Makefile.

## Patch triage vs the 21.3 package

Dropped (fixed upstream in master): `003-dyn-pagesize`, `004-gcc13`,
`005-gcc15-musl`, `007-other-archs`, `100-pcre2`,
`142-DRMPRIME-create-non-drm-hwdevice-context`; `110-ffmpeg7` is
obsolete with FFmpeg 8. Rebased: `006-sse-build`,
`130-findpulseaudio-mainloop-optional`,
`200-support-pregenerated-python-bindings`. Added:
`210-disable-internal-texturepacker` (LibreELEC's - master otherwise
cross-builds and ships a target TexturePacker, whose sub-build does not
find lzo2); `220-meson-crossfile-pkg-config-override` (own, upstreamable:
adds `MESON_PKG_CONFIG_EXECUTABLE` so the libdvd meson cross files use
OpenWrt's unwrapped `pkg-config.real` - the wrapper's `${prefix}` rewrite
breaks the `.pc` files in Kodi's private depends prefix).

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
