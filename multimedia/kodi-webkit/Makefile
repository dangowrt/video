include $(TOPDIR)/rules.mk

PKG_NAME:=kodi-webkit
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/dangowrt/kodi-webkit.git
PKG_SOURCE_DATE:=2026-11-05
PKG_SOURCE_VERSION:=1bf635d2cb681326e3f80ac16f95be87925ca69d
PKG_MIRROR_HASH:=f004052da6a747ef65ec0396500e7f0622cac4c6ec6dd850c3a04a98cf456bb4

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
  DEPENDS:=+libwpewebkit +libgio-gnutls +libgst1audio +libgst1video +libgst1allocators \
	+bubblewrap +xdg-dbus-proxy +USE_MUSL:gcompat
endef

define Package/kodi-webkit/description
 kodi-webview, the program Kodi starts to show web app add-ons. It runs
 as a Wayland client of Kodi and drives one WPE WebKit view per web app
 over Kodi's kodi_web_v1 protocol, the compiler that turns Adblock Plus
 filter lists into WebKit content filters, a GStreamer audio sink that
 plays the pages' audio through Kodi's audio engine, and a GStreamer
 video sink that shows the pages' video on Kodi's video plane.
endef

MAKE_FLAGS += \
	PKG_CONFIG="$(PKG_CONFIG)" \
	WAYLAND_SCANNER="$(STAGING_DIR_HOSTPKG)/bin/wayland-scanner" \
	$(if $(CONFIG_USE_MUSL),WIDEVINE_PRELOAD=/lib/libgcompat-ifunc.so.0)

define Package/kodi-webkit/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/kodi-webview $(1)/usr/bin/
	$(INSTALL_DIR) $(1)/usr/libexec
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/kodi-webview-filter $(1)/usr/libexec/
	$(INSTALL_DIR) $(1)/usr/lib/gstreamer-1.0
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/libgstkodiaudiosink.so $(1)/usr/lib/gstreamer-1.0/
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/libgstkodivideosink.so $(1)/usr/lib/gstreamer-1.0/
endef

$(eval $(call BuildPackage,kodi-webkit))
