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

## Python bindings

Upstream generates the Python API bindings at build time with SWIG and a
Groovy code generator, which needs a Java runtime plus three archive
downloads during cmake configure. This package instead carries the
Python rewrite of that generator from upstream PR 28454
(`patches/200-python-bindings-codegenerator-in-python.patch`, standard
library only) and the matching build-system change
(`patches/201-python-bindings-generate-with-swig-and-python.patch`).
The bindings are generated during the normal build by `swig/host` and
the host Python from staging; nothing is pre-generated or downloaded.

The generator output was verified byte-identical to the Groovy output
for all seven modules of this snapshot, using `swig -xml` output of
swig 4.3.0 for both; the feed's `swig/host` is 4.2.1 and was not part
of that comparison. The only deliberate difference from the PR's
generator is the Python API `__version__` constant (3.1.0, as in
upstream's template since Beta 2). The PR is a draft targeting Kodi 23;
when bumping Kodi, re-check the constant against
`addons/xbmc.python/addon.xml` and rebase the two patches.

`host/generate-python-bindings.sh` is the previous Groovy-based
generation (swig, a JRE, curl and unzip on the host), kept for the
moment as the reference to cross-check the Python generator's output
against.

## Patch triage vs the 21.3 package

Dropped (fixed upstream in master): `003-dyn-pagesize`, `004-gcc13`,
`005-gcc15-musl`, `007-other-archs`, `100-pcre2`,
`142-DRMPRIME-create-non-drm-hwdevice-context`; `110-ffmpeg7` is
obsolete with FFmpeg 8. Rebased: `006-sse-build`,
`130-findpulseaudio-mainloop-optional`,
`200-python-bindings-codegenerator-in-python` and
`201-python-bindings-generate-with-swig-and-python` (replace the
former pre-generated bindings, see above). Added:
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
