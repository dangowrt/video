include $(TOPDIR)/rules.mk

PKG_NAME:=kodi-addon-youtubetv
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/dangowrt/kodi-webapp-youtube.git
PKG_SOURCE_DATE:=2026-10-22
PKG_SOURCE_VERSION:=c186a4d34e2172337681814b9809f278120e1215
PKG_MIRROR_HASH:=d5894d064c80b42911c8bc13d90da26bd91f1854deabdaf7253ea313165297c1

PKG_LICENSE:=GPL-3.0-or-later MIT
PKG_LICENSE_FILES:=LICENSE LICENSE.VacuumTube
PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>

include $(INCLUDE_DIR)/package.mk

define Package/kodi-addon-youtubetv
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=YouTube TV web app for Kodi
  URL:=https://github.com/dangowrt/kodi-webapp-youtube
  DEPENDS:=+kodi +kodi-webkit
  PKGARCH:=all
endef

define Package/kodi-addon-youtubetv/description
 The YouTube TV app as a Kodi web app add-on (webapp.youtube), shown in
 Kodi through the kodi-webkit client.
endef

define Build/Compile
endef

define Package/kodi-addon-youtubetv/install
	$(INSTALL_DIR) $(1)/usr/share/kodi/addons/webapp.youtube
	$(CP) $(PKG_BUILD_DIR)/addon.xml $(PKG_BUILD_DIR)/LICENSE \
		$(PKG_BUILD_DIR)/LICENSE.VacuumTube $(PKG_BUILD_DIR)/profile \
		$(PKG_BUILD_DIR)/resources \
		$(1)/usr/share/kodi/addons/webapp.youtube/
endef

$(eval $(call BuildPackage,kodi-addon-youtubetv))
