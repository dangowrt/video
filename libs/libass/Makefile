include $(TOPDIR)/rules.mk

PKG_NAME:=libass
PKG_VERSION:=0.17.4
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.xz
PKG_SOURCE_URL:=https://github.com/libass/libass/releases/download/$(PKG_VERSION)/
PKG_HASH:=78f1179b838d025e9c26e8fef33f8092f65611444ffa1bfc0cfac6a33511a05a

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=ISC
PKG_LICENSE_FILES:=COPYING
PKG_CPE_ID:=cpe:/a:libass_project:libass

PKG_BUILD_PARALLEL:=1
PKG_INSTALL:=1

include $(INCLUDE_DIR)/package.mk

define Package/libass
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Subtitle renderer for ASS/SSA subtitles
  URL:=https://github.com/libass/libass
  DEPENDS:=+libfreetype +fribidi +harfbuzz +fontconfig
endef

define Package/libass/description
  libass is a portable subtitle renderer for the ASS/SSA
  (Advanced Substation Alpha/Substation Alpha) subtitle format.
endef

CONFIGURE_ARGS += \
	--disable-asm \
	--disable-libunibreak \
	--enable-fontconfig

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/ass $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libass.{a,so*} $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/libass.pc $(1)/usr/lib/pkgconfig/
	$(SED) 's,/usr/include,$$$${prefix}/include,g' $(1)/usr/lib/pkgconfig/libass.pc
	$(SED) 's,/usr/lib,$$$${exec_prefix}/lib,g' $(1)/usr/lib/pkgconfig/libass.pc
endef

define Package/libass/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libass.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,libass))
