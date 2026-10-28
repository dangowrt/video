include $(TOPDIR)/rules.mk

PKG_NAME:=kodi-webapps
PKG_RELEASE:=1

PKG_SOURCE_PROTO:=git
PKG_SOURCE_URL:=https://github.com/dangowrt/kodi-webapps.git
PKG_SOURCE_DATE:=2026-10-28
PKG_SOURCE_VERSION:=54e6dafc440e362850c03859e54f4c633c008fc4
PKG_MIRROR_HASH:=24df25486f40b37c5ae0db4955a74c100ba0aa2e1f25d9385901027d90a2757e

PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=LICENSE
PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>

include $(INCLUDE_DIR)/package.mk

define Package/kodi-addon-repository-webapps
  SECTION:=multimedia
  CATEGORY:=Multimedia
  TITLE:=Kodi add-on repository of web apps
  URL:=https://github.com/dangowrt/kodi-webapps
  DEPENDS:=+kodi +kodi-webkit
  PKGARCH:=all
endef

define Package/kodi-addon-repository-webapps/description
 The Kodi add-on repository that installs and updates web apps, such as
 Netflix, YouTube TV or the ARD Mediathek, from inside Kodi.
endef

define Build/Compile
endef

define Package/kodi-addon-repository-webapps/install
	$(INSTALL_DIR) $(1)/usr/share/kodi/addons/repository.dangowrt.webapps
	$(INSTALL_DATA) $(PKG_BUILD_DIR)/repository.dangowrt.webapps/addon.xml \
		$(1)/usr/share/kodi/addons/repository.dangowrt.webapps/
endef

$(eval $(call BuildPackage,kodi-addon-repository-webapps))
