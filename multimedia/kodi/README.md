# Kodi for OpenWrt

Kodi 22.0 RC1 (tag `22.0rc1-Piers`, commit `28ea2eac`) built for the GBM (bare KMS/DRM) and Wayland windowing
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

`patches/140` and `patches/141` are LibreELEC's `drmprime-filter`
patches (their 2026-08-22 rebase, applied unchanged): they let the DRM_PRIME video path run FFmpeg filters, which is
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

`001` to `120` make Kodi build against musl and for cross targets,
`140` and `141` are LibreELEC's DRM_PRIME filter patches (see above),
`210` is LibreELEC's and keeps the target build from cross-building its
own TexturePacker, and `220` to `231` fix the cmake lookups for a cross
build with OpenWrt's staging directory. The remaining patches are the
downstream features: display power management (`142`, `143`), hwmon
temperatures (`144` to `146`), GBM output selection (`147`), per-add-on
uplinks (`150` to `155`) and the web apps with their Wayland host and
web player (`240` to `265`). Each is exported from the `wayland-host`
branch of https://github.com/dangowrt/xbmc and carries that commit's
hash in its `From` line.

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
