#
# Kodi 22 (Piers, beta), linked against the packages feed's ffmpeg package.
#
# Successor of the kodi 21.3 package that previously lived here; the
# package name, config symbols and installed paths are unchanged.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=kodi
PKG_VERSION:=22.0_beta2
PKG_RELEASE:=2

# Tag 22.0b2-Piers (version.txt: 22.0 BETA2); pinned by commit so the
# tarball name stays stable should the tag ever move.
PKG_SOURCE_VERSION:=e513e0ff4331fc25fd2454659a9dd3e6b7670146
PKG_SOURCE:=xbmc-$(PKG_SOURCE_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/xbmc/xbmc/tar.gz/$(PKG_SOURCE_VERSION)?
PKG_HASH:=0a1f5c0910f39f21e7d7521835ae0eba6286cff1a6c665badf502443f8e4acd0
PKG_BUILD_DIR:=$(BUILD_DIR)/xbmc-$(PKG_SOURCE_VERSION)
HOST_BUILD_DIR:=$(BUILD_DIR_HOST)/xbmc-$(PKG_SOURCE_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=LICENSE.md
PKG_CPE_ID:=cpe:/a:kodi:kodi

PKG_BUILD_PARALLEL:=1
HOST_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=kodi/host nlohmannjson flatbuffers flatbuffers/host waylandpp/host wayland-protocols python3/host swig/host
HOST_BUILD_DEPENDS:=libpng/host giflib/host libjpeg-turbo/host lzo/host flatbuffers/host
CMAKE_INSTALL:=1
CMAKE_BINARY_SUBDIR:=openwrt-build

PKG_CONFIG_DEPENDS:= \
	CONFIG_KODI_WEBSERVER \
	CONFIG_KODI_DBUS \
	CONFIG_KODI_UPNP \
	CONFIG_KODI_SMB \
	CONFIG_KODI_PULSEAUDIO \
	CONFIG_KODI_PIPEWIRE \
	CONFIG_KODI_AVAHI \
	CONFIG_KODI_BLUETOOTH

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/cmake.mk
include $(TOPDIR)/feeds/packages/lang/python/python3-version.mk

# Kodi insists on building libdvdread/libdvdnav itself on Linux (it needs
# its own patched libdvdnav) and would download the archives during the
# cmake build; crossguid has no OpenWrt package and is built in-tree as
# well. Fetch the exact files Kodi expects through OpenWrt's download
# machinery instead and hand them over as local URLs below; Kodi's own
# SHA512 check (tools/depends/target/*/*-VERSION) still applies to them.
LIBDVDREAD_VERSION:=7.0.1
LIBDVDNAV_VERSION:=7.0.0
CROSSGUID_VERSION:=ca1bf4b810e2d188d04cb6286f957008ee1b7681

define Download/libdvdread
  FILE:=libdvdread-$(LIBDVDREAD_VERSION).tar.bz2
  URL:=https://mirrors.kodi.tv/build-deps/sources/
  HASH:=b69f74d9ceea1ed173b579deba99f669c2cb42f3fd06d7d23b33ff222aa63763
endef
$(eval $(call Download,libdvdread))

define Download/libdvdnav
  FILE:=libdvdnav-$(LIBDVDNAV_VERSION).tar.bz2
  URL:=https://mirrors.kodi.tv/build-deps/sources/
  HASH:=1363cdfaf6e92c0b574579299b5480f5867fb32989451468a28f3f402ec48787
endef
$(eval $(call Download,libdvdnav))

define Download/crossguid
  FILE:=crossguid-$(CROSSGUID_VERSION).tar.gz
  URL:=https://mirrors.kodi.tv/build-deps/sources/
  HASH:=6be27e0b3a4907f0cd3cfadec255ee1b925569e1bd06e67a4d2f4267299b69c4
endef
$(eval $(call Download,crossguid))

define Package/kodi
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=Kodi media centre
  URL:=https://kodi.tv/
  DEPENDS:=@FFMPEG_GPL +libffmpeg +libass +waylandpp +libwayland \
	+libxkbcommon +xkeyboard-config +libinput +libudev +libdisplay-info +libmesa +libdrm \
	+KODI_WEBSERVER:libmicrohttpd +KODI_DBUS:libdbus +KODI_SMB:samba4-libs \
	+KODI_PULSEAUDIO:libpulse +KODI_PIPEWIRE:pipewire \
	+KODI_AVAHI:libavahi-client +KODI_AVAHI:avahi-dbus-daemon \
	+KODI_BLUETOOTH:bluez-libs \
	+x86_64:libva +alsa-lib +libfreetype +fontconfig +fribidi +harfbuzz \
	+taglib +libsqlite3 +libcurl +libfmt +libcdio \
	+spdlog +libtinyxml +libtinyxml2 +libfstrcmp +libexiv2 \
	+libpcre2 +giflib +libjpeg-turbo +libpng +liblzo \
	+zlib +libbz2 +libopenssl +libpython3 +python3 +libuuid \
	+libstdcpp +libatomic $(ICONV_DEPENDS)
endef

define Package/kodi/config
menu "Kodi feature configuration"

config KODI_WEBSERVER
	bool "Web server (HTTP JSON-RPC, web interface, remote apps)"
	default n

config KODI_DBUS
	bool "DBus integration (logind/upower, needs a system dbus daemon)"
	default n

config KODI_UPNP
	bool "UPnP / DLNA client and server (internal Platinum, no extra deps)"
	default n

config KODI_SMB
	bool "SMB client (browse Samba/Windows shares)"
	default n

config KODI_PULSEAUDIO
	bool "PulseAudio audio backend"
	default n

config KODI_AVAHI
	bool "Avahi / Zeroconf discovery (announce and discover services)"
	default n

config KODI_BLUETOOTH
	bool "Bluetooth support (BlueZ)"
	default n

config KODI_PIPEWIRE
	bool "PipeWire audio backend"
	default n

endmenu
endef

define Package/kodi/description
  Kodi is a free and open source media player and entertainment hub.
  This build supports the GBM (direct KMS/DRM) and Wayland windowing
  systems with GLES rendering. Run it via the kodi init script on
  bare KMS/DRM, or as a fullscreen kiosk session inside cage on
  Wayland using kodi-cage.
endef

# Every Kodi build option is pinned explicitly; nothing is left to
# auto-detection. The set below is a minimal-but-running GBM+Wayland
# mediacenter: GLES rendering, ALSA audio, udev input, Python addons.
# Optional protocols/services (webserver, UPnP, DBus, SMB/NFS, ...) are
# OFF by default - flip individually as needed.

# Platform, toolchain and developer-tool switches.
# Optimization (-Ox), LTO and the mold linker are governed by the
# OpenWrt-wide settings (TARGET_CFLAGS, CONFIG_USE_LTO, CONFIG_USE_MOLD
# via package.mk; opt out with PKG_BUILD_FLAGS if ever needed). Kodi's
# own USE_LTO stays OFF so it cannot stack a second -flto on top.
# cmake.mk confines programs, libraries and headers to the find roots
# but not cmake package configs, which Kodi's find modules try first;
# without MODE_PACKAGE=ONLY a host /usr/lib*/cmake/<dep> wins.
CMAKE_OPTIONS += \
	-DCMAKE_FIND_ROOT_PATH_MODE_PACKAGE=ONLY \
	-DCORE_PLATFORM_NAME="gbm wayland" \
	-DAPP_RENDER_SYSTEM=gles \
	-DENABLE_NEON=$(if $(findstring aarch64,$(ARCH)),ON,OFF) \
	-DUSE_LTO=OFF \
	-DENABLE_GOLD=OFF \
	-DENABLE_DEBUGFISSION=OFF \
	-DENABLE_CCACHE=OFF \
	-DENABLE_TESTING=OFF \
	-DENABLE_CLANGFORMAT=OFF \
	-DENABLE_CLANGTIDY=OFF \
	-DENABLE_CPPCHECK=OFF \
	-DENABLE_INCLUDEWHATYOUUSE=OFF

# Internal (vendored/ExternalProject) dependencies: use the system
# package wherever one exists. crossguid has none, so Kodi builds it as
# a cmake ExternalProject from the pre-fetched archive; such a sub-build
# only inherits a toolchain file, hence the one generated in
# Build/Configure from files/openwrt-toolchain.cmake.in.
# Exception KissFFT=ON: builds Kodi's bundled in-tree copy
# (xbmc/contrib, plain sources compiled with the target toolchain, no
# download, no ExternalProject) - there is no system kissfft package.
CMAKE_OPTIONS += \
	-DENABLE_INTERNAL_ASS=OFF \
	-DENABLE_INTERNAL_BLURAY=OFF \
	-DENABLE_INTERNAL_CURL=OFF \
	-DENABLE_INTERNAL_FFMPEG=OFF \
	-DENABLE_INTERNAL_LCMS2=OFF \
	-DENABLE_INTERNAL_LIBMICROHTTPD=OFF \
	-DENABLE_INTERNAL_LZO2=OFF \
	-DENABLE_INTERNAL_MARIADBCLIENT=OFF \
	-DENABLE_INTERNAL_XSLT=OFF \
	-DENABLE_INTERNAL_CROSSGUID=ON \
	-DCROSSGUID_URL=$(DL_DIR)/crossguid-$(CROSSGUID_VERSION).tar.gz \
	-DCROSSGUID_TOOLCHAIN_FILE=$(PKG_BUILD_DIR)/openwrt-toolchain.cmake \
	-DENABLE_INTERNAL_FLATBUFFERS=OFF \
	-DFLATBUFFERS_FLATC_EXECUTABLE=$(STAGING_DIR_HOSTPKG)/bin/flatc \
	-DENABLE_INTERNAL_NLOHMANNJSON=OFF \
	-DENABLE_INTERNAL_FMT=OFF \
	-DENABLE_INTERNAL_SPDLOG=OFF \
	-DENABLE_INTERNAL_FSTRCMP=OFF \
	-DENABLE_INTERNAL_EXIV2=OFF \
	-DENABLE_INTERNAL_UDFREAD=OFF \
	-DENABLE_INTERNAL_DAV1D=OFF \
	-DENABLE_INTERNAL_CEC=OFF \
	-DENABLE_INTERNAL_GTEST=OFF \
	-DENABLE_INTERNAL_NFS=OFF \
	-DENABLE_INTERNAL_PCRE2=OFF \
	-DENABLE_INTERNAL_TAGLIB=OFF \
	-DENABLE_INTERNAL_TINYXML2=OFF

# Features and optional dependencies (alphabetical). Minimal profile:
# audio out, input and Python stay ON; everything else OFF unless its
# KODI_* toggle (see config menu above) is set. AIRTUNES (needs
# unpackaged shairplay) and CEC (needs unpackaged libcec+p8-platform)
# deliberately have no toggle - decided against packaging for now.
CMAKE_OPTIONS += \
	-DENABLE_ALSA=ON \
	-DENABLE_AIRTUNES=OFF \
	-DENABLE_APP_AUTONAME=OFF \
	-DENABLE_AVAHI=$(if $(CONFIG_KODI_AVAHI),ON,OFF) \
	-DENABLE_BLUETOOTH=$(if $(CONFIG_KODI_BLUETOOTH),ON,OFF) \
	-DENABLE_BLURAY=OFF \
	-DENABLE_CAP=OFF \
	-DENABLE_CEC=OFF \
	-DENABLE_DAV1D=OFF \
	-DENABLE_DBUS=$(if $(CONFIG_KODI_DBUS),ON,OFF) \
	-DENABLE_DVDCSS=OFF \
	-DENABLE_EVENTCLIENTS=OFF \
	-DENABLE_ISO9660PP=OFF \
	-DENABLE_LCMS2=OFF \
	-DENABLE_LIRCCLIENT=OFF \
	-DENABLE_MARIADBCLIENT=OFF \
	-DENABLE_MDNS=OFF \
	-DENABLE_MICROHTTPD=$(if $(CONFIG_KODI_WEBSERVER),ON,OFF) \
	-DENABLE_MYSQLCLIENT=OFF \
	-DENABLE_NFS=OFF \
	-DENABLE_OPTICAL=OFF \
	-DENABLE_PIPEWIRE=$(if $(CONFIG_KODI_PIPEWIRE),ON,OFF) \
	-DENABLE_PLIST=OFF \
	-DENABLE_PULSEAUDIO=$(if $(CONFIG_KODI_PULSEAUDIO),ON,OFF) \
	-DENABLE_PYTHON=ON \
	-DENABLE_SMBCLIENT=$(if $(CONFIG_KODI_SMB),ON,OFF) \
	-DENABLE_SNDIO=OFF \
	-DENABLE_UDEV=ON \
	-DENABLE_UDFREAD=OFF \
	-DENABLE_UPNP=$(if $(CONFIG_KODI_UPNP),ON,OFF) \
	-DENABLE_VAAPI=$(if $(findstring x86_64,$(ARCH)),ON,OFF) \
	-DENABLE_XSLT=OFF \
	-DADDONS_CONFIGURE_AT_STARTUP=OFF

# Host tools, Python bindings generation, pre-fetched libdvd sources.
# The Python API bindings are generated at build time by swig/host and
# the Python code generator from patches/200 (see README.md); both the
# swig binary and the host interpreter come from staging so the result
# does not depend on the build host. PYTHON_INTERPRETER_PATH is a
# search hint (directory) for Kodi's FindPythonInterpreter.
# The libdvd* sub-builds use meson+ninja; OpenWrt ships meson as a zipapp
# named meson.py, which Kodi's find_program(meson) would not find. Their
# cross files must use the unwrapped pkg-config: OpenWrt's wrapper rewrites
# ${prefix} to the staging dir, which is wrong for the .pc files Kodi
# installs into its private depends prefix (patches/220).
CMAKE_OPTIONS += \
	-DMESON_EXECUTABLE=$(STAGING_DIR_HOST)/bin/meson.py \
	-DMESON_PKG_CONFIG_EXECUTABLE=$(STAGING_DIR_HOST)/bin/pkg-config.real \
	-DNINJA_EXECUTABLE=$(STAGING_DIR_HOST)/bin/ninja \
	-DLIBDVDREAD_URL=$(DL_DIR)/libdvdread-$(LIBDVDREAD_VERSION).tar.bz2 \
	-DLIBDVDNAV_URL=$(DL_DIR)/libdvdnav-$(LIBDVDNAV_VERSION).tar.bz2 \
	-DSWIG_EXECUTABLE=$(STAGING_DIR_HOSTPKG)/bin/swig \
	-DPYTHON_INTERPRETER_PATH=$(STAGING_DIR_HOSTPKG)/bin \
	-DPYTHON_EXECUTABLE=$(STAGING_DIR_HOSTPKG)/bin/python$(PYTHON3_VERSION) \
	-DPYTHON_VER=$(PYTHON3_VERSION) \
	-DWITH_TEXTUREPACKER=$(STAGING_DIR_HOSTPKG)/bin/TexturePacker \
	-DWITH_JSONSCHEMABUILDER=$(STAGING_DIR_HOSTPKG)/bin/JsonSchemaBuilder

TARGET_LDFLAGS += -Wl,-z,stack-size=2097152
# Resolve transitive NEEDED of libpulse (libpulsecommon in the private
# pulseaudio/ dir) and libsmbclient (samba private libs) at link time.
TARGET_LDFLAGS += \
	-Wl,-rpath-link,$(STAGING_DIR)/usr/lib/pulseaudio \
	-Wl,-rpath-link,$(STAGING_DIR)/usr/lib/samba

define Build/Configure
	$(STAGING_DIR_HOST)/bin/sed \
		-e 's|@ARCH@|$(ARCH)|g' \
		-e 's|@CC@|$(CMAKE_C_COMPILER)|g' \
		-e 's|@CXX@|$(CMAKE_CXX_COMPILER)|g' \
		-e 's|@LAUNCHER@|$(CMAKE_C_COMPILER_LAUNCHER)|g' \
		-e 's|@AR@|$(CMAKE_AR)|g' \
		-e 's|@NM@|$(CMAKE_NM)|g' \
		-e 's|@RANLIB@|$(CMAKE_RANLIB)|g' \
		-e 's|@FIND_ROOT_PATH@|$(CMAKE_FIND_ROOT_PATH)|g' \
		-e 's|@CFLAGS@|$(TARGET_CFLAGS) $(EXTRA_CFLAGS) $(TARGET_CPPFLAGS) $(EXTRA_CPPFLAGS)|g' \
		-e 's|@CXXFLAGS@|$(TARGET_CXXFLAGS) $(EXTRA_CXXFLAGS) $(TARGET_CPPFLAGS) $(EXTRA_CPPFLAGS)|g' \
		-e 's|@LDFLAGS@|$(TARGET_LDFLAGS) $(EXTRA_LDFLAGS)|g' \
		./files/openwrt-toolchain.cmake.in > $(PKG_BUILD_DIR)/openwrt-toolchain.cmake
	$(call Build/Configure/Default)
endef

define Host/Configure
	$(STAGING_DIR_HOST)/bin/cmake \
		-S $(HOST_BUILD_DIR)/tools/depends/native/JsonSchemaBuilder/src \
		-B $(HOST_BUILD_DIR)/jsonschemabuilder-build \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INSTALL_PREFIX=$(STAGING_DIR_HOSTPKG)
	$(STAGING_DIR_HOST)/bin/cmake \
		-S $(HOST_BUILD_DIR)/tools/depends/native/TexturePacker/src \
		-B $(HOST_BUILD_DIR)/texturepacker-build \
		-DCMAKE_INSTALL_RPATH=$(STAGING_DIR_HOSTPKG)/lib \
		-DCMAKE_BUILD_WITH_INSTALL_RPATH=ON \
		-DCMAKE_BUILD_TYPE=Release \
		-DCMAKE_INSTALL_PREFIX=$(STAGING_DIR_HOSTPKG) \
		-DCMAKE_PREFIX_PATH=$(STAGING_DIR_HOSTPKG) \
		-DKODI_SOURCE_DIR=$(HOST_BUILD_DIR) \
		"-DARCH_DEFINES=-DTARGET_POSIX -DTARGET_LINUX"
endef

define Host/Compile
	+$(STAGING_DIR_HOST)/bin/cmake --build $(HOST_BUILD_DIR)/jsonschemabuilder-build
	+$(STAGING_DIR_HOST)/bin/cmake --build $(HOST_BUILD_DIR)/texturepacker-build
endef

define Host/Install
	$(STAGING_DIR_HOST)/bin/cmake --install $(HOST_BUILD_DIR)/jsonschemabuilder-build
	$(STAGING_DIR_HOST)/bin/cmake --install $(HOST_BUILD_DIR)/texturepacker-build
endef

define Package/kodi/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/kodi $(1)/usr/bin/
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/kodi-standalone $(1)/usr/bin/
	$(INSTALL_BIN) ./files/kodi-cage $(1)/usr/bin/kodi-cage
	$(INSTALL_DIR) $(1)/usr/lib/kodi
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/kodi/* $(1)/usr/lib/kodi/
	$(INSTALL_DIR) $(1)/usr/share/kodi
	$(CP) $(PKG_INSTALL_DIR)/usr/share/kodi/* $(1)/usr/share/kodi/
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/kodi.init $(1)/etc/init.d/kodi
	# Preserve /srv/kodi (HOME) across sysupgrade
	$(INSTALL_DIR) $(1)/lib/upgrade/keep.d
	$(INSTALL_DATA) ./files/kodi.upgrade $(1)/lib/upgrade/keep.d/kodi
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,kodi))
