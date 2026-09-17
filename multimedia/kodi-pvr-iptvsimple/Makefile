include $(TOPDIR)/rules.mk

PKG_NAME:=kodi-pvr-iptvsimple
PKG_VERSION:=22.6.4
PKG_VERSION_NAME:=Piers
PKG_RELEASE:=2

PKG_SOURCE:=pvr.iptvsimple-$(PKG_VERSION)-$(PKG_VERSION_NAME).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/kodi-pvr/pvr.iptvsimple/tar.gz/$(PKG_VERSION)-$(PKG_VERSION_NAME)?
PKG_HASH:=1f4ff290ad66760ca5774647860e828ff2a3bc162b3ed7636fa859f357824a27
PKG_BUILD_DIR:=$(BUILD_DIR)/pvr.iptvsimple-$(PKG_VERSION)-$(PKG_VERSION_NAME)

PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=LICENSE.md
PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

# The addon hands playback to inputstream.ffmpegdirect or
# inputstream.adaptive depending on the stream, and declares both as
# non-optional imports in its addon.xml.
define Package/kodi-pvr-iptvsimple
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=Kodi PVR addon for IPTV M3U playlists
  URL:=https://github.com/kodi-pvr/pvr.iptvsimple
  DEPENDS:=+kodi +kodi-inputstream-adaptive +kodi-inputstream-ffmpegdirect \
	+libpugixml +zlib +liblzma +libstdcpp
endef

define Package/kodi-pvr-iptvsimple/description
 Kodi PVR addon that turns an M3U playlist into live TV and radio
 channels, with XMLTV programme guide, channel groups, providers,
 catchup/archive playback and media entries from the same playlist.
 Version-locked to the Kodi 22 (Piers) PVR addon API - keep in sync with
 the kodi package's major version.
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
define Package/kodi-pvr-iptvsimple/install
	$(INSTALL_DIR) $(1)/usr/lib/kodi/addons
	$(CP) $(PKG_INSTALL_DIR)$(STAGING_DIR)/usr/lib/kodi/addons/pvr.iptvsimple \
		$(1)/usr/lib/kodi/addons/
	$(INSTALL_DIR) $(1)/usr/share/kodi/addons
	$(CP) $(PKG_INSTALL_DIR)$(STAGING_DIR)/usr/share/kodi/addons/pvr.iptvsimple \
		$(1)/usr/share/kodi/addons/
endef

$(eval $(call BuildPackage,kodi-pvr-iptvsimple))
