#
# Copyright (C) 2019 Jianhui Zhao <zhaojh329@gmail.com>
#
# This is free software, licensed under the MIT.
#

include $(TOPDIR)/rules.mk
include $(INCLUDE_DIR)/kernel.mk
include ./version.mk

PKG_NAME:=transip-sdk-info
PKG_VERSION:=$(strip $(call findrev))
PKG_RELEASE:=1

include $(INCLUDE_DIR)/package.mk

define KernelPackage/transip-sdk-info
  CATEGORY:=transip-sdk
  SUBMENU:=Kernel modules
  TITLE:=TransIP hardware information
  FILES:=$(PKG_BUILD_DIR)/transip-sdk-info.ko
  DEPENDS:=+uboot-envtools
  AUTOLOAD:=$(call AutoLoad,01,transip-sdk-info,1)
  KCONFIG:=
endef

define Build/Prepare
	$(INSTALL_DIR) $(PKG_BUILD_DIR)
	$(CP) ./src/. $(PKG_BUILD_DIR)
endef

KERNEL_MAKE_FLAGS?= \
	ARCH="$(LINUX_KARCH)" \
	CROSS_COMPILE="$(TARGET_CROSS)"

MAKE_OPTS:= \
	$(KERNEL_MAKE_FLAGS) \
	M="$(PKG_BUILD_DIR)"

define Build/Compile
	$(MAKE) -C "$(LINUX_DIR)" \
		$(MAKE_OPTS) \
		modules
endef

define KernelPackage/transip-sdk-info/install
	$(INSTALL_DIR) $(1)/etc/uci-defaults
	$(INSTALL_DATA) ./files/transip-hw-env.default $(1)/etc/uci-defaults/00-transip-hw-env
endef

$(eval $(call KernelPackage,transip-sdk-info))
