#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=webkit-contentfilter
PKG_VERSION:=1
PKG_RELEASE:=1

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=GPL-2.0-only

include $(INCLUDE_DIR)/package.mk

define Package/webkit-contentfilter
  SECTION:=utils
  CATEGORY:=Utilities
  TITLE:=Adblock filter lists for WebKit content blockers
  PKGARCH:=all
  DEPENDS:=+ucode +ucode-mod-fs +ucode-mod-uci +ucode-mod-uloop \
	+ucode-mod-uclient +ca-bundle
endef

define Package/webkit-contentfilter/description
  Fetches Adblock Plus filter lists such as EasyList and EasyPrivacy from
  the URLs configured in /etc/config/webkit-contentfilter and converts
  them into WebKit content blocker JSON under /var/run/webkit-contentfilter.

  Network rules and CSS element hiding are converted; scriptlets, regular
  expression rules and extended cosmetic filters have no WebKit equivalent
  and are left out. The lists are fetched once the clock is set by NTP and
  again after each day of uptime, and stay in RAM unless a storage path is
  configured.
endef

define Package/webkit-contentfilter/conffiles
/etc/config/webkit-contentfilter
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
endef

define Build/Compile
endef

define Package/webkit-contentfilter/install
	$(INSTALL_DIR) $(1)/usr/sbin
	$(INSTALL_BIN) ./files/webkit-contentfilter.uc $(1)/usr/sbin/webkit-contentfilter
	$(INSTALL_DIR) $(1)/usr/share/ucode/webkit-contentfilter
	$(INSTALL_DATA) ./files/convert.uc $(1)/usr/share/ucode/webkit-contentfilter/convert.uc
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) ./files/webkit-contentfilter.init $(1)/etc/init.d/webkit-contentfilter
	$(INSTALL_DIR) $(1)/etc/config
	$(INSTALL_CONF) ./files/webkit-contentfilter.config $(1)/etc/config/webkit-contentfilter
	$(INSTALL_DIR) $(1)/etc/hotplug.d/ntp
	$(INSTALL_DATA) ./files/webkit-contentfilter.hotplug $(1)/etc/hotplug.d/ntp/50-webkit-contentfilter
endef

$(eval $(call BuildPackage,webkit-contentfilter))
