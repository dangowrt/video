#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=kodi-standby
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-only

include $(INCLUDE_DIR)/package.mk

define Package/kodi-standby
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Standby for a Kodi media centre
  URL:=https://kodi.tv/
  DEPENDS:=+kodi +python3-light
endef

define Package/kodi-standby/description
  Turns the display off on a key press and takes the machine down with it:
  the cpufreq governor drops to its lowest setting, every core but one goes
  offline and the configured network interfaces come down, while Bluetooth
  keeps running so the remote can wake everything again. The box never
  suspends and never powers off, so a press of any key on the remote brings
  the picture straight back.

  Kodi decides when this happens. Its DPMS state is the trigger, which the
  supplied keymap binds to the remote's secondary power button.
endef

define Package/kodi-standby/conffiles
/etc/kodi-standby.conf
/etc/kodi/userdata/keymaps/kodi-standby.xml
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile
endef

define Package/kodi-standby/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/kodi-standby $(1)/usr/sbin/kodi-standby
	$(INSTALL_BIN) ./files/kodi-standbyd $(1)/usr/sbin/kodi-standbyd
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/kodi-standby.init $(1)/etc/init.d/kodi-standby
	$(INSTALL_DIR) $(1)/etc
	$(INSTALL_CONF) ./files/kodi-standby.conf $(1)/etc/kodi-standby.conf
	$(INSTALL_DIR) $(1)/etc/kodi-standby.d
	$(INSTALL_DIR) $(1)/etc/kodi/userdata/keymaps
	$(INSTALL_CONF) ./files/kodi-standby.xml $(1)/etc/kodi/userdata/keymaps/kodi-standby.xml
endef

$(eval $(call BuildPackage,kodi-standby))
