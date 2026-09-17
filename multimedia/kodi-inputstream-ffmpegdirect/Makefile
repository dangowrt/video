include $(TOPDIR)/rules.mk

PKG_NAME:=kodi-inputstream-ffmpegdirect
PKG_VERSION:=22.2.7
PKG_VERSION_NAME:=Piers
PKG_RELEASE:=1

PKG_SOURCE:=inputstream.ffmpegdirect-$(PKG_VERSION)-$(PKG_VERSION_NAME).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/xbmc/inputstream.ffmpegdirect/tar.gz/$(PKG_VERSION)-$(PKG_VERSION_NAME)?
PKG_HASH:=90f0bfc4a3e5fe397749f5244608ee8881c6722c9a68c33fd61eb5becd976f23
PKG_BUILD_DIR:=$(BUILD_DIR)/inputstream.ffmpegdirect-$(PKG_VERSION)-$(PKG_VERSION_NAME)

PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=LICENSE.md
PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/kodi-inputstream-ffmpegdirect
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=Kodi inputstream addon for FFmpeg-demuxed streams
  URL:=https://github.com/xbmc/inputstream.ffmpegdirect
  DEPENDS:=@FFMPEG_GPL +kodi +libffmpeg +zlib +libbz2 +libstdcpp
endef

define Package/kodi-inputstream-ffmpegdirect/description
 Kodi inputstream addon for streams that FFmpeg's libavformat or Kodi's
 cURL can open: plain TS, HLS and DASH without DRM, among many others.
 It also drives archive/catchup services, which offer a replay window of
 usually a few days, and can timeshift across that span.
 Version-locked to the Kodi 22 (Piers) addon API - keep in sync with the
 kodi package's major version.
endef

# The staged KodiConfig.cmake was generated for the TARGET prefix: it
# hardcodes /usr/... into CMAKE_MODULE_PATH and defaults the (overridable)
# KODI_* dirs to /usr/... - host-absolute nonsense under cross-compile
# ("include could not find requested file: AddonHelpers"). Re-root all of
# them into staging explicitly.
CMAKE_OPTIONS += \
	-DKODI_PREFIX=$(STAGING_DIR)/usr \
	-DKODI_INCLUDE_DIR=$(STAGING_DIR)/usr/include/kodi \
	-DKODI_LIB_DIR=$(STAGING_DIR)/usr/lib/kodi \
	-DKODI_DATA_DIR=$(STAGING_DIR)/usr/share/kodi \
	-DCMAKE_MODULE_PATH=$(STAGING_DIR)/usr/share/kodi/cmake

# With the KODI_* dirs overridden (see CMAKE_OPTIONS), AddonHelpers
# installs straight into ${KODI_LIB_DIR}/addons and ${KODI_DATA_DIR}/addons
# - absolute STAGING paths, which under DESTDIR end up at
# $(PKG_INSTALL_DIR)$(STAGING_DIR)/... . Already the correct lib/share
# split (shared object vs. addon.xml/resources), so just re-root both into
# the package.
define Package/kodi-inputstream-ffmpegdirect/install
	$(INSTALL_DIR) $(1)/usr/lib/kodi/addons
	$(CP) $(PKG_INSTALL_DIR)$(STAGING_DIR)/usr/lib/kodi/addons/inputstream.ffmpegdirect \
		$(1)/usr/lib/kodi/addons/
	$(INSTALL_DIR) $(1)/usr/share/kodi/addons
	$(CP) $(PKG_INSTALL_DIR)$(STAGING_DIR)/usr/share/kodi/addons/inputstream.ffmpegdirect \
		$(1)/usr/share/kodi/addons/
endef

$(eval $(call BuildPackage,kodi-inputstream-ffmpegdirect))
