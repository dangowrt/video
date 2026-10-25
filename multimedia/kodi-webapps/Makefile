include $(TOPDIR)/rules.mk

PKG_NAME:=kodi-webapps
PKG_VERSION:=1.0.0
PKG_RELEASE:=4

PKG_LICENSE:=GPL-2.0-or-later
PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>

include $(INCLUDE_DIR)/package.mk

# Chrome on Linux reports x86_64 whatever the CPU
WEBAPP_USER_AGENT:=Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/156.0.0.0 Safari/537.36

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile
endef

# 1: package suffix, 2: add-on id, 3: name, 4: URL
define WebApp
  define Package/kodi-addon-$(1)
    SECTION:=multimedia
    CATEGORY:=Multimedia
    TITLE:=$(3) web app for Kodi
    URL:=$(4)
    DEPENDS:=+kodi +kodi-webkit
    PKGARCH:=all
  endef

  define Package/kodi-addon-$(1)/description
   A Kodi web app add-on ($(2)) opening $(3) in the kodi-webkit client.
  endef

  define Package/kodi-addon-$(1)/install
	$(INSTALL_DIR) $$(1)/usr/share/kodi/addons/$(2)
	$(CP) ./files/common/. ./files/$(2)/. $$(1)/usr/share/kodi/addons/$(2)/
	$(SED) 's,@ADDON_ID@,$(2),' \
		$$(1)/usr/share/kodi/addons/$(2)/resources/settings.xml
	$(SED) 's|@USER_AGENT@|$(WEBAPP_USER_AGENT)|' \
		$$(1)/usr/share/kodi/addons/$(2)/addon.xml
  endef
endef

$(eval $(call WebApp,duckduckgo,webapp.duckduckgo,DuckDuckGo,https://duckduckgo.com/))
$(eval $(call WebApp,raiplay,webapp.raiplay,RaiPlay,https://www.raiplay.it/))
$(eval $(call WebApp,ardmediathek,webapp.ardmediathek,ARD Mediathek,https://www.ardmediathek.de/))
$(eval $(call WebApp,netflix,webapp.netflix,Netflix,https://www.netflix.com/))
$(eval $(call WebApp,primevideo,webapp.primevideo,Prime Video,https://www.primevideo.com/))

$(eval $(call BuildPackage,kodi-addon-duckduckgo))
$(eval $(call BuildPackage,kodi-addon-raiplay))
$(eval $(call BuildPackage,kodi-addon-ardmediathek))
$(eval $(call BuildPackage,kodi-addon-netflix))
$(eval $(call BuildPackage,kodi-addon-primevideo))
