#
# Copyright (C) 2016 The CyanogenMod Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# Inherit from msm8916-common
include device/cyanogen/msm8916-common/BoardConfigCommon.mk

DEVICE_PATH := device/xiaomi/ferrari

# Assertions
TARGET_BOARD_INFO_FILE := $(DEVICE_PATH)/board-info.txt

# Audio
USE_CUSTOM_AUDIO_POLICY := 0
BOARD_USES_GENERIC_AUDIO := true
TARGET_USES_QCOM_MM_AUDIO := true

# Bluetooth
BOARD_BLUETOOTH_BDROID_BUILDCFG_INCLUDE_DIR := $(DEVICE_PATH)/bluetooth

# Camera
TARGET_USE_VENDOR_CAMERA_EXT := true
USE_DEVICE_SPECIFIC_CAMERA := true
TARGET_USES_MEDIA_EXTENSIONS := true
TARGET_NEEDS_PLATFORM_TEXT_RELOCATIONS := true
TARGET_CAMERASERVICE_CLOSES_NATIVE_HANDLES := true
TARGET_NEEDS_LEGACY_CAMERA_HAL1_DYN_NATIVE_HANDLE := true

# Charger
BOARD_CHARGER_ENABLE_SUSPEND := true
BOARD_CHARGER_DISABLE_INIT_BLANK := true

# Crypto
TARGET_HW_DISK_ENCRYPTION := false
TARGET_LEGACY_HW_DISK_ENCRYPTION := false
TARGET_KEYMASTER_WAIT_FOR_QSEE := false

# Filesystem
TARGET_USERIMAGES_USE_F2FS          := true
BOARD_BOOTIMAGE_PARTITION_SIZE      := 0x02000000
BOARD_RECOVERYIMAGE_PARTITION_SIZE  := 0x02000000
BOARD_SYSTEMIMAGE_PARTITION_SIZE    := 1744830464
BOARD_USERDATAIMAGE_PARTITION_SIZE  := 13144931840
BOARD_CACHEIMAGE_PARTITION_SIZE     := 402653184
BOARD_PERSISTIMAGE_PARTITION_SIZE   := 33554432
BOARD_PERSISTIMAGE_FILE_SYSTEM_TYPE := ext4
BOARD_FLASH_BLOCK_SIZE              := 131072 # BOARD_KERNEL_PAGESIZE * 64

# GPS
TARGET_NO_RPC := true
USE_DEVICE_SPECIFIC_GPS := true

# Kernel
BOARD_DTBTOOL_ARGS := -2
BOARD_KERNEL_SEPARATED_DT := true
BOARD_KERNEL_IMAGE_NAME := Image
TARGET_KERNEL_SOURCE := kernel/xiaomi/ferrari
ifeq ($(WITH_TWRP),true)
TARGET_KERNEL_CONFIG := ferrari-recovery_defconfig
else
TARGET_KERNEL_CONFIG := ferrari_defconfig
endif

# LineageHW
BOARD_HARDWARE_CLASS += $(DEVICE_PATH)/lineagehw
TARGET_TAP_TO_WAKE_NODE := "/sys/devices/soc.0/78b9000.i2c/i2c-5/5-004a/wakeup_mode"

# Manifest
DEVICE_MANIFEST_FILE := $(DEVICE_PATH)/manifest.xml

# Properties
TARGET_SYSTEM_PROP += $(DEVICE_PATH)/system.prop

# Recovery
TARGET_RECOVERY_FSTAB := $(DEVICE_PATH)/rootdir/etc/fstab.qcom

# SELinux
BOARD_SEPOLICY_DIRS += $(DEVICE_PATH)/sepolicy

# Shims
TARGET_LD_SHIM_LIBS += \
    /system/lib/libmmjpeg_interface.so|libcamera_shim.so \
    /system/vendor/bin/mm-pp-daemon|libcamera_shim.so \
    /system/vendor/bin/perfd|libcamera_shim.so \
    /system/vendor/lib64/libflp.so|libshims_flp.so \
    /system/vendor/lib64/libizat_core.so|libshims_get_process_name.so \
    /system/vendor/lib/libflp.so|libshims_flp.so \
    /system/vendor/lib/libizat_core.so|libshims_get_process_name.so \
    /system/vendor/lib/libmmcamera2_imglib_modules.so|libcamera_shim.so

# TWRP
ifeq ($(WITH_TWRP),true)
include $(DEVICE_PATH)/twrp.mk
endif

# WLAN
TARGET_PROVIDES_WCNSS_QMI := true

# inherit from the proprietary version
-include vendor/xiaomi/ferrari/BoardConfigVendor.mk
