#
# This is free software, licensed under the GNU General Public License v2.
# See /LICENSE for more information.
#
# cdm-harness is a throwaway-grade de-risk tool for the T6 (EME/Widevine)
# backend work. It is NOT shipped and is not part of any image; it drives
# inputstream.adaptive's cdm_adapter against the Chromium Widevine blob to
# prove the blob loads and produces a licence-request challenge on the box
# under gcompat before the engine is embedded in the WPEWebKit backend.
#

include $(TOPDIR)/rules.mk

PKG_NAME:=cdm-harness
PKG_VERSION:=0.1.0
PKG_RELEASE:=1

PKG_MAINTAINER:=Daniel Golle <daniel@makrotopia.org>
PKG_LICENSE:=BSD-3-Clause

include $(INCLUDE_DIR)/package.mk

define Package/cdm-harness
  SECTION:=utils
  CATEGORY:=Utilities
  SUBMENU:=Video
  TITLE:=Chromium-CDM de-risk harness (T6 test tool, not shipped)
  URL:=https://wpewebkit.org
  DEPENDS:=+gcompat +libstdcpp
endef

define Package/cdm-harness/description
  Standalone harness that loads a Chromium CDM shared object (the ChromeOS
  Widevine blob) through inputstream.adaptive's media::CdmAdapter and drives
  CreateCdmInstance, Initialize and CreateSessionAndGenerateRequest to prove
  the blob loads under gcompat and produces a licence-request challenge, and
  that provisioning storage is exercised. It performs no decryption. This is a
  test tool for the EME backend bring-up and is not part of any firmware image.
endef

define Build/Prepare
	mkdir -p $(PKG_BUILD_DIR)
	$(CP) ./src/* $(PKG_BUILD_DIR)/
endef

CDM_HARNESS_SRCS:= \
	cdm_harness.cpp \
	cdm/media/cdm/cdm_adapter.cc \
	cdm/base/native_library.cc \
	cdm/base/native_library_posix.cc \
	cdm/debug.cpp

define Build/Compile
	( cd $(PKG_BUILD_DIR); \
	  $(TARGET_CXX) $(TARGET_CFLAGS) $(TARGET_CPPFLAGS) -std=c++17 -I. \
	    -o cdm-harness $(CDM_HARNESS_SRCS) \
	    $(TARGET_LDFLAGS) -ldl -lpthread )
endef

define Package/cdm-harness/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/cdm-harness $(1)/usr/bin/cdm-harness
endef

$(eval $(call BuildPackage,cdm-harness))
