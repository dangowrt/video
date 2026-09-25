include $(TOPDIR)/rules.mk

PKG_NAME:=crossguid
PKG_VERSION:=0.2.3
PKG_RELEASE:=1

PKG_SOURCE_VERSION:=ca1bf4b810e2d188d04cb6286f957008ee1b7681
PKG_SOURCE:=$(PKG_NAME)-$(PKG_SOURCE_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/graeme-hill/crossguid/tar.gz/$(PKG_SOURCE_VERSION)?
PKG_HASH:=6be27e0b3a4907f0cd3cfadec255ee1b925569e1bd06e67a4d2f4267299b69c4
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_SOURCE_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE

PKG_INSTALL:=1
PKG_BUILD_PARALLEL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

CMAKE_OPTIONS += \
	-DCROSSGUID_TESTS=OFF \
	-DBUILD_SHARED_LIBS=ON

define Package/crossguid
  SECTION:=libs
  CATEGORY:=Libraries
  TITLE:=Cross platform C++ GUID/UUID library
  URL:=https://github.com/graeme-hill/crossguid
  DEPENDS:=+libuuid +libstdcpp
endef

define Package/crossguid/description
  Generates, parses and formats globally unique identifiers behind one
  C++ interface, over libuuid on Linux. Kodi uses it to identify its
  own objects.
endef

define Build/InstallDev
	$(INSTALL_DIR) $(1)/usr/include
	$(CP) $(PKG_INSTALL_DIR)/usr/include/crossguid $(1)/usr/include/
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libcrossguid.so* $(1)/usr/lib/
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/pkgconfig/crossguid.pc $(1)/usr/lib/pkgconfig/
endef

define Package/crossguid/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libcrossguid.so.* $(1)/usr/lib/
endef

$(eval $(call BuildPackage,crossguid))
