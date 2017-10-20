# Kernel
BOARD_DTBTOOL_ARGS := -2
BOARD_KERNEL_SEPARATED_DT := true
TARGET_USES_UNCOMPRESSED_KERNEL := true

ifeq ($(WITH_TWRP),true)
TARGET_KERNEL_CONFIG := ferrari-recovery_defconfig
else
TARGET_KERNEL_CONFIG := ferrari_defconfig
endif
TARGET_KERNEL_SOURCE := kernel/xiaomi/ferrari

