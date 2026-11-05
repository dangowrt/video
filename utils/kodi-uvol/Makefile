#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=kodi-uvol
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-only

include $(INCLUDE_DIR)/package.mk

define Package/kodi-uvol
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Keep Kodi's data on a uvol volume
  URL:=https://kodi.tv/
  DEPENDS:=+kodi +uvol +autopart
  PKGARCH:=all
endef

define Package/kodi-uvol/description
  Gives Kodi a read-write volume of its own for its data directory on
  first boot, unless the directory already has storage: a mount in
  /etc/config/fstab, or data in it. autopart turns the free space of the
  boot disk into an LVM volume group, uvol creates the volume in it, and
  the data directory becomes a link to the volume's mount point, which
  Kodi waits for before it starts.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile
endef

define Package/kodi-uvol/install
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_BIN) ./files/kodi-uvol.defaults $(1)/etc/uci-defaults/85-kodi-uvol
endef

$(eval $(call BuildPackage,kodi-uvol))
