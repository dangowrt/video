# Kodi for OpenWrt

Kodi built for the GBM (bare KMS/DRM) and Wayland windowing systems
with GLES rendering. X11 and framebuffer are not supported.

## Running

- On bare KMS/DRM: `/etc/init.d/kodi start` launches kodi-standalone
  with GBM windowing via procd.
- As a Wayland kiosk: `kodi-cage` runs Kodi fullscreen inside the
  cage compositor.

## Pre-generated Python bindings

Upstream generates the Python API bindings at build time using SWIG
and a Groovy code generator, which requires a Java runtime on the
build host. To keep Java out of the build entirely, this package uses
bindings generated once per release and distributed as a tarball,
consumed via the `PYTHON_BINDINGS_DIR` cmake variable added by
`patches/200-support-pregenerated-python-bindings.patch`.

The generated sources are plain C++ against the CPython API. They are
independent of the target architecture and depend only on the Kodi
source tree, so they are reproducible for a given release.

To regenerate after a version bump, on a host with swig, a JRE (11 or
newer), curl and unzip:

```
./files/generate-python-bindings.sh <kodi-source-tree> 21.3-Omega
```

The script prints the SHA256 of the produced tarball; upload the
tarball to the location referenced by `Download/python-bindings` in
the Makefile and update its hash.

## FFmpeg

Kodi requires libpostproc, hence the dependency on `FFMPEG_FULL_GPL`
(enabled by default in the ffmpeg package). Software decoding of
H.264/HEVC/VC-1 additionally requires `BUILD_PATENTED`; without it,
hardware decoding via stateful V4L2 M2M decoders still works on
supported platforms.
