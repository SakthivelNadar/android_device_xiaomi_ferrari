# Kernel
BOARD_DTBTOOL_ARGS := -2
BOARD_KERNEL_SEPARATED_DT := true
BOARD_KERNEL_IMAGE_NAME := Image

ifeq ($(WITH_TWRP),true)
TARGET_KERNEL_CONFIG := ferrari-recovery_defconfig
else
TARGET_KERNEL_CONFIG := ferrari_defconfig
endif
TARGET_KERNEL_SOURCE := kernel/xiaomi/ferrari

BOARD_KERNEL_CMDLINE += androidboot.selinux=permissive
