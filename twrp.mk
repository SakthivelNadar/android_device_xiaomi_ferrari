TARGET_RECOVERY_DEVICE_DIRS += device/xiaomi/ferrari/twrp
TW_INCLUDE_CRYPTO := true
TW_CRYPTO_USE_SYSTEM_VOLD := qseecomd hwservicemanager keymaster-3-0
TW_THEME := portrait_hdpi

RECOVERY_SDCARD_ON_DATA := true
TARGET_HW_DISK_ENCRYPTION := false
TARGET_RECOVERY_QCOM_RTC_FIX := true
TARGET_USE_CUSTOM_LUN_FILE_PATH := "/sys/devices/platform/msm_hsusb/gadget/lun%d/file"
TW_BRIGHTNESS_PATH := /sys/class/leds/lcd-backlight/brightness
TW_MTP_DEVICE := "/dev/mtp_usb"
TW_NO_USB_STORAGE := true
TW_OEM_BUILD := false
TW_EXCLUDE_TWRPAPP := true
TW_USE_TOOLBOX := true

TWRP_INCLUDE_LOGCAT := true
TARGET_USES_LOGD := true

# Time Zone data for TWRP
PRODUCT_COPY_FILES += \
    system/timezone/output_data/iana/tzdata:recovery/root/system/usr/share/zoneinfo/tzdata
