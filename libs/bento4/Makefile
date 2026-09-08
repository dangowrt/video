include $(TOPDIR)/rules.mk

PKG_NAME:=bento4
PKG_VERSION:=1.6.0-641-6-Piers
PKG_RELEASE:=1

PKG_SOURCE:=Bento4-$(PKG_VERSION).tar.gz
PKG_SOURCE_URL:=https://codeload.github.com/xbmc/Bento4/tar.gz/$(PKG_VERSION)?
PKG_HASH:=cb5006cd30889238760556e6148f69be34fef4dc48e8d895a9822f805504a5c0
PKG_BUILD_DIR:=$(BUILD_DIR)/Bento4-$(PKG_VERSION)

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-or-later
PKG_LICENSE_FILES:=Documents/LICENSE.txt

PKG_BUILD_PARALLEL:=1
CMAKE_INSTALL:=1

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/bento4
  SECTION:=libs
  CATEGORY:=Libraries
  SUBMENU:=Video
  TITLE:=Bento4 MP4 and DASH library, Kodi fork
  URL:=https://github.com/xbmc/Bento4
  BUILDONLY:=1
endef

define Package/bento4/description
  Static build of Kodi's fork of the Bento4 MP4/DASH toolkit at the tag
  pinned by the kodi-inputstream-adaptive addon, which links it into its
  shared object.
endef

CMAKE_OPTIONS += \
	-DBUILD_APPS=OFF \
	-DBUILD_SHARED_LIBRARY=OFF \
	-DCMAKE_POSITION_INDEPENDENT_CODE=ON

$(eval $(call BuildPackage,bento4))
