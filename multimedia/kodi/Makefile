include $(TOPDIR)/rules.mk

PKG_NAME:=kodi
PKG_VERSION:=21.3
PKG_VERSION_NAME:=Omega
PKG_RELEASE:=1

PKG_SOURCE:=xbmc-$(PKG_VERSION)-$(PKG_VERSION_NAME).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/xbmc/xbmc/tar.gz/$(PKG_VERSION)-$(PKG_VERSION_NAME)?
PKG_HASH:=a60afcb556e4a00cb19fc35b7e77758107c9a8096c6ce9b66af5f92396be31aa
PKG_BUILD_DIR:=$(BUILD_DIR)/xbmc-$(PKG_VERSION)-$(PKG_VERSION_NAME)
HOST_BUILD_DIR:=$(BUILD_DIR_HOST)/xbmc-$(PKG_VERSION)-$(PKG_VERSION_NAME)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=LICENSE.md
PKG_CPE_ID:=cpe:/a:kodi:kodi

CROSSGUID_VERSION:=ca1bf4b810e2d188d04cb6286f957008ee1b7681

PKG_BUILD_PARALLEL:=1
HOST_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=kodi/host rapidjson flatbuffers waylandpp/host wayland-protocols python3/host
HOST_BUILD_DEPENDS:=libpng/host giflib/host libjpeg-turbo/host lzo/host flatbuffers/host
CMAKE_INSTALL:=1
CMAKE_BINARY_SUBDIR:=openwrt-build

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/cmake.mk
include $(TOPDIR)/feeds/packages/lang/python/python3-version.mk

define Download/crossguid
  FILE:=crossguid-$(CROSSGUID_VERSION).tar.gz
  URL:=https://mirrors.kodi.tv/build-deps/sources/
  HASH:=6be27e0b3a4907f0cd3cfadec255ee1b925569e1bd06e67a4d2f4267299b69c4
endef
$(eval $(call Download,crossguid))

# Pre-generated Python API bindings, see README.md for regeneration
define Download/python-bindings
  FILE:=kodi-python-bindings-$(PKG_VERSION)-$(PKG_VERSION_NAME).tar.xz
  URL:=https://github.com/openwrt/video/releases/download/kodi-$(PKG_VERSION)/
  HASH:=aac636cbfdb33dd2b2e29530f28d402bb8e274429e5a6fdfcee03789aa19f56a
endef
$(eval $(call Download,python-bindings))

define Package/kodi
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=Kodi media centre
  URL:=https://kodi.tv/
  DEPENDS:=@FFMPEG_FULL_GPL +libffmpeg-full +libass +waylandpp +libwayland \
	+libxkbcommon +libinput +libudev +libdisplay-info +libmesa +libdrm \
	+libva +alsa-lib +libfreetype +fontconfig +fribidi +harfbuzz \
	+taglib +libsqlite3 +libcurl +libmicrohttpd +libdbus +libfmt +libcdio \
	+spdlog +libtinyxml +libtinyxml2 +libfstrcmp +libexiv2 +libudfread \
	+liblcms2 +libpcre2 +giflib +libjpeg-turbo +libpng +liblzo \
	+zlib +libbz2 +libopenssl +libpython3 +python3 +libuuid \
	+libstdcpp +libatomic $(ICONV_DEPENDS)
endef

define Package/kodi/description
  Kodi is a free and open source media player and entertainment hub.
  This build supports the GBM (direct KMS/DRM) and Wayland windowing
  systems with GLES rendering. Run it via the kodi init script on
  bare KMS/DRM, or as a fullscreen kiosk session inside cage on
  Wayland using kodi-cage.
endef

CMAKE_OPTIONS += \
	-DCORE_PLATFORM_NAME="gbm wayland" \
	-DAPP_RENDER_SYSTEM=gles \
	-DENABLE_INTERNAL_FFMPEG=OFF \
	-DENABLE_INTERNAL_CROSSGUID=ON \
	-DCROSSGUID_URL=$(DL_DIR)/crossguid-$(CROSSGUID_VERSION).tar.gz \
	-DENABLE_INTERNAL_FLATBUFFERS=OFF \
	-DENABLE_INTERNAL_RapidJSON=OFF \
	-DENABLE_INTERNAL_FMT=OFF \
	-DENABLE_INTERNAL_SPDLOG=OFF \
	-DENABLE_INTERNAL_FSTRCMP=OFF \
	-DENABLE_INTERNAL_EXIV2=OFF \
	-DENABLE_INTERNAL_UDFREAD=OFF \
	-DENABLE_INTERNAL_DAV1D=OFF \
	-DENABLE_OPTICAL=OFF \
	-DENABLE_DVDCSS=OFF \
	-DENABLE_TESTING=OFF \
	-DENABLE_GOLD=OFF \
	-DENABLE_DEBUGFISSION=OFF \
	-DENABLE_CCACHE=OFF \
	-DENABLE_AIRTUNES=OFF \
	-DENABLE_AVAHI=OFF \
	-DENABLE_BLUETOOTH=OFF \
	-DENABLE_CAP=OFF \
	-DENABLE_CEC=OFF \
	-DENABLE_NFS=OFF \
	-DENABLE_BLURAY=OFF \
	-DENABLE_SMBCLIENT=OFF \
	-DENABLE_MARIADBCLIENT=OFF \
	-DENABLE_MYSQLCLIENT=OFF \
	-DENABLE_UDFREAD=ON \
	-DENABLE_LCMS2=ON \
	-DENABLE_LIRCCLIENT=OFF \
	-DENABLE_PULSEAUDIO=OFF \
	-DENABLE_PIPEWIRE=OFF \
	-DENABLE_SNDIO=OFF \
	-DENABLE_ALSA=ON \
	-DENABLE_VAAPI=ON \
	-DENABLE_VDPAU=OFF \
	-DENABLE_UDEV=ON \
	-DENABLE_DBUS=ON \
	-DENABLE_XSLT=OFF \
	-DENABLE_UPNP=ON \
	-DENABLE_EVENTCLIENTS=OFF \
	-DENABLE_APP_AUTONAME=OFF \
	-DADDONS_CONFIGURE_AT_STARTUP=OFF \
	-DPYTHON_BINDINGS_DIR=$(PKG_BUILD_DIR)/pregen-bindings \
	-DPYTHON_EXECUTABLE=$(STAGING_DIR_HOSTPKG)/bin/python$(PYTHON3_VERSION) \
	-DPYTHON_VER=$(PYTHON3_VERSION) \
	-DWITH_TEXTUREPACKER=$(STAGING_DIR_HOSTPKG)/bin/TexturePacker \
	-DWITH_JSONSCHEMABUILDER=$(STAGING_DIR_HOSTPKG)/bin/JsonSchemaBuilder

TARGET_LDFLAGS += -Wl,-z,stack-size=2097152

define Build/Prepare
	$(call Build/Prepare/Default)
	mkdir -p $(PKG_BUILD_DIR)/pregen-bindings
	$(TAR) -xJf $(DL_DIR)/kodi-python-bindings-$(PKG_VERSION)-$(PKG_VERSION_NAME).tar.xz \
		--strip-components=1 -C $(PKG_BUILD_DIR)/pregen-bindings
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
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,kodi))
