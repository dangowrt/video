include $(TOPDIR)/rules.mk

PKG_NAME:=kodi-inputstream-adaptive
PKG_VERSION:=22.3.21
PKG_VERSION_NAME:=Piers
PKG_RELEASE:=1

PKG_SOURCE:=inputstream.adaptive-$(PKG_VERSION)-$(PKG_VERSION_NAME).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/xbmc/inputstream.adaptive/tar.gz/$(PKG_VERSION)-$(PKG_VERSION_NAME)?
PKG_HASH:=6f249bcd7557c75d2814869527745e739d2a44b5730d6a13c514495dd96d2b32
PKG_BUILD_DIR:=$(BUILD_DIR)/inputstream.adaptive-$(PKG_VERSION)-$(PKG_VERSION_NAME)

PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=LICENSE.md
PKG_MAINTAINER:=Mirko Vogt <mirko-openwrt@nanl.de>

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

# kodi: provides the addon dev-kit (staging usr/include/kodi +
#   usr/lib/kodi/cmake/KodiConfig.cmake) the addon builds against.
# bento4: static MP4/DASH lib at the exact tag the addon's depends pin
#   (see bento4 package); found via the addon's FindBento4.
# pugixml: XML parsing (libpugixml also a runtime dep below).
# nlohmannjson: header-only JSON parsing (replaced rapidjson in 22.x).
PKG_BUILD_DEPENDS:=kodi bento4 pugixml nlohmannjson

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/kodi-inputstream-adaptive
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=Kodi inputstream addon for adaptive streams (DASH/HLS/ISM)
  URL:=https://github.com/xbmc/inputstream.adaptive
  DEPENDS:=+kodi +libpugixml
endef

define Package/kodi-inputstream-adaptive/description
 Kodi binary addon that demuxes adaptive bitrate streams (MPEG-DASH, HLS,
 Microsoft Smooth Streaming) with multi-resolution/bandwidth switching.
 Required by e.g. the YouTube addon. Version-locked to the Kodi 22
 (Piers) addon API - keep in sync with the kodi package's major version.
 DRM (Widevine) playback is NOT available on this platform (the CDM
 blob needs glibc); plain adaptive streaming - which is what YouTube
 uses - works without it.
endef

# Explicit and deterministic: no tests (would pull googletest; upstream
# also auto-disables them when cross-compiling), and the addon must use
# the packaged deps from staging - its depends/ mechanism (download +
# ExternalProject-style sub-builds) cannot cross-compile under OpenWrt,
# same rationale as kodi's ENABLE_INTERNAL_*=OFF set.
#
# The staged KodiConfig.cmake was generated for the TARGET prefix: it
# hardcodes /usr/... into CMAKE_MODULE_PATH and defaults the (overridable)
# KODI_* dirs to /usr/... - host-absolute nonsense under cross-compile
# ("include could not find requested file: AddonHelpers"). Re-root all of
# them into staging explicitly.
CMAKE_OPTIONS += \
	-DBUILD_TESTING=OFF \
	-DKODI_PREFIX=$(STAGING_DIR)/usr \
	-DKODI_INCLUDE_DIR=$(STAGING_DIR)/usr/include/kodi \
	-DKODI_LIB_DIR=$(STAGING_DIR)/usr/lib/kodi \
	-DKODI_DATA_DIR=$(STAGING_DIR)/usr/share/kodi \
	-DCMAKE_MODULE_PATH=$(STAGING_DIR)/usr/share/kodi/cmake

# With the KODI_* dirs overridden (see CMAKE_OPTIONS), AddonHelpers
# installs straight into ${KODI_LIB_DIR}/addons and ${KODI_DATA_DIR}/addons
# - absolute STAGING paths, which under DESTDIR end up at
# $(PKG_INSTALL_DIR)$(STAGING_DIR)/... . Already the correct lib/share
# split (shared objects incl. the cdm loader vs. addon.xml/resources), so
# just re-root both into the package.
define Package/kodi-inputstream-adaptive/install
	$(INSTALL_DIR) $(1)/usr/lib/kodi/addons
	$(CP) $(PKG_INSTALL_DIR)$(STAGING_DIR)/usr/lib/kodi/addons/inputstream.adaptive \
		$(1)/usr/lib/kodi/addons/
	$(INSTALL_DIR) $(1)/usr/share/kodi/addons
	$(CP) $(PKG_INSTALL_DIR)$(STAGING_DIR)/usr/share/kodi/addons/inputstream.adaptive \
		$(1)/usr/share/kodi/addons/
endef

$(eval $(call BuildPackage,kodi-inputstream-adaptive))
