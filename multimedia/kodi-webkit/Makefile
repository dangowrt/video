include $(TOPDIR)/rules.mk

PKG_NAME:=kodi-webkit
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/dangowrt/kodi-webkit.git
PKG_SOURCE_DATE:=2026-11-01
PKG_SOURCE_VERSION:=0cd619833fa477854388b4ffd0c2225e43c64dc0
PKG_MIRROR_HASH:=5b8f742b5ab7d9cca6c4b53806fafd35af9b7b52471ec369fab13e30d19fa135

PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>

PKG_BUILD_DEPENDS:=wayland/host

include $(INCLUDE_DIR)/package.mk

define Package/kodi-webkit
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=WPE WebKit client for Kodi web apps
  URL:=https://github.com/dangowrt/kodi-webkit
  DEPENDS:=+libwpewebkit +libgio-gnutls +libgst1audio +bubblewrap +xdg-dbus-proxy \
	+USE_MUSL:gcompat
endef

define Package/kodi-webkit/description
 kodi-webview, the program Kodi starts to show web app add-ons. It runs
 as a Wayland client of Kodi and drives one WPE WebKit view per web app
 over Kodi's kodi_web_v1 protocol, and a GStreamer audio sink that
 plays the pages' audio through Kodi's audio engine.
endef

MAKE_FLAGS += \
	PKG_CONFIG="$(PKG_CONFIG)" \
	WAYLAND_SCANNER="$(STAGING_DIR_HOSTPKG)/bin/wayland-scanner" \
	$(if $(CONFIG_USE_MUSL),WIDEVINE_PRELOAD=/lib/libgcompat-ifunc.so.0)

define Package/kodi-webkit/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/kodi-webview $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/lib/gstreamer-1.0
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/libgstkodiaudiosink.so $(1)/usr/lib/gstreamer-1.0/
endef

$(eval $(call BuildPackage,kodi-webkit))
