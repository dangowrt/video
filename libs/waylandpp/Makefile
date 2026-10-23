include $(TOPDIR)/rules.mk

PKG_NAME:=waylandpp
PKG_VERSION:=1.0.1
PKG_RELEASE:=1

PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/NilsBrause/$(PKG_NAME)/tar.gz/$(PKG_VERSION)?
PKG_HASH:=49c362fa5db28ab5472968215b88f1fbe3a7b7f57818dde722fd7d38997d940a

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=MIT
PKG_LICENSE_FILES:=LICENSE

PKG_BUILD_PARALLEL:=1
HOST_BUILD_PARALLEL:=1
PKG_BUILD_DEPENDS:=waylandpp/host mesa
HOST_BUILD_DEPENDS:=pugixml/host
CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/host-build.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/waylandpp
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Wayland C++ bindings
  URL:=https://github.com/NilsBrause/waylandpp
  DEPENDS:=+libwayland +libstdcpp
endef

define Package/waylandpp/description
  C++ bindings for the Wayland client library. The host build
  provides the wayland-scanner++ protocol code generator.
endef

CMAKE_OPTIONS += \
	-DBUILD_SCANNER=OFF \
	-DBUILD_LIBRARIES=ON \
	-DBUILD_SERVER=OFF \
	-DBUILD_EXAMPLES=OFF \
	-DBUILD_DOCUMENTATION=OFF \
	-DWAYLAND_SCANNERPP=$(STAGING_DIR_HOSTPKG)/bin/wayland-scanner++

CMAKE_HOST_OPTIONS += \
	-DBUILD_SCANNER=ON \
	-DBUILD_LIBRARIES=OFF \
	-DBUILD_DOCUMENTATION=OFF

define Build/InstallDev
	$(call Build/InstallDev/cmake,$(1))
	$(SED) 's,^pkgdatadir=.*,pkgdatadir=$$$${prefix}/share/waylandpp,' \
		$(1)/usr/lib/pkgconfig/wayland-*++.pc
	$(SED) 's,/usr/include,$$$${prefix}/include,g' $(1)/usr/lib/pkgconfig/wayland-*++.pc
	$(SED) 's,/usr/lib,$$$${exec_prefix}/lib,g' $(1)/usr/lib/pkgconfig/wayland-*++.pc
	$(INSTALL_DIR) $(1)/usr/lib/pkgconfig
	printf '%s\n' \
		'wayland_scannerpp=$(STAGING_DIR_HOSTPKG)/bin/wayland-scanner++' \
		'Name: wayland-scanner++' \
		'Description: Wayland C++ protocol scanner (host tool)' \
		'Version: $(PKG_VERSION)' \
		> $(1)/usr/lib/pkgconfig/wayland-scanner++.pc
endef

define Package/waylandpp/install
	$(INSTALL_DIR) $(1)/usr/lib
	$(CP) $(PKG_INSTALL_DIR)/usr/lib/libwayland-*++.so.* $(1)/usr/lib/
endef

$(eval $(call HostBuild))
$(eval $(call BuildPackage,waylandpp))
