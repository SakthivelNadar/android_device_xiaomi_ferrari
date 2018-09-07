# Audio
PRODUCT_COPY_FILES += \
    $(LOCAL_PATH)/audio/acdb/MTP_WCD9306_Bluetooth_cal.acdb:system/etc/acdbdata/MTP/MTP_WCD9306_Bluetooth_cal.acdb \
    $(LOCAL_PATH)/audio/acdb/MTP_WCD9306_General_cal.acdb:system/etc/acdbdata/MTP/MTP_WCD9306_General_cal.acdb \
    $(LOCAL_PATH)/audio/acdb/MTP_WCD9306_Global_cal.acdb:system/etc/acdbdata/MTP/MTP_WCD9306_Global_cal.acdb \
    $(LOCAL_PATH)/audio/acdb/MTP_WCD9306_Handset_cal.acdb:system/etc/acdbdata/MTP/MTP_WCD9306_Handset_cal.acdb \
    $(LOCAL_PATH)/audio/acdb/MTP_WCD9306_Hdmi_cal.acdb:system/etc/acdbdata/MTP/MTP_WCD9306_Hdmi_cal.acdb \
    $(LOCAL_PATH)/audio/acdb/MTP_WCD9306_Headset_cal.acdb:system/etc/acdbdata/MTP/MTP_WCD9306_Headset_cal.acdb \
    $(LOCAL_PATH)/audio/acdb/MTP_WCD9306_Speaker_cal.acdb:system/etc/acdbdata/MTP/MTP_WCD9306_Speaker_cal.acdb \
    $(LOCAL_PATH)/audio/audio_platform_info.xml:$(TARGET_COPY_OUT_VENDOR)/etc/audio_platform_info_extcodec.xml \
    $(LOCAL_PATH)/audio/audio_policy.conf:$(TARGET_COPY_OUT_VENDOR)/etc/audio_policy.conf \
    $(LOCAL_PATH)/audio/mixer_paths_wcd9306.xml:$(TARGET_COPY_OUT_VENDOR)/etc/mixer_paths_wcd9306.xml

# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.audio.low_latency.xml:system/vendor/etc/permissions/android.hardware.audio.low_latency.xml

# Properties
PRODUCT_PROPERTY_OVERRIDES += \
    audio.offload.disable=1 \
    ro.vendor.audio.sdk.fluencetype=fluence \
    persist.vendor.audio.fluence.voicecall=true \
    persist.vendor.audio.fluence.voicerec=false \
    persist.vendor.audio.fluence.speaker=false

# tinyplay
PRODUCT_PACKAGES += \
    tinyplay
