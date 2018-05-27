# Permissions
PRODUCT_COPY_FILES += \
    frameworks/native/data/etc/android.hardware.ethernet.xml:system/etc/permissions/android.hardware.ethernet.xml \
    frameworks/native/data/etc/android.hardware.usb.host.xml:system/etc/permissions/android.hardware.usb.host.xml

# USB VID
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    ro.usb.vid=2717
    ro.usb.id.rndis=ff80 \
    ro.usb.id.rndis_adb=ff88 \
    ro.usb.id.mtp=ff60 \
    ro.usb.id.mtp_adb=ff68 \
    ro.usb.id.ptp=ff10 \
    ro.usb.id.ptp_adb=ff18 \
    ro.usb.id.ums=F000 \
    ro.usb.id.ums_adb=9015

# midi support (masquerade as marlin)
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    ro.usb.vid.midi=18D1 \
    ro.usb.id.midi=4EE8 \
    ro.usb.id.midi_adb=4EE9

# force enable ADB
PRODUCT_DEFAULT_PROPERTY_OVERRIDES += \
    persist.service.adb.enable=1 \
    persist.service.debuggable=1 \
    persist.sys.usb.config=mtp,adb \
    persist.sys.root_access=3
