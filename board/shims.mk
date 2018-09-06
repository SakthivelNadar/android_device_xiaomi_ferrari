TARGET_LD_SHIM_LIBS += \
    /system/bin/mm-pp-daemon|libcamera_shim.so \
    /vendor/bin/perfd|libcamera_shim.so \
    /system/vendor/lib64/libizat_core.so|libshims_get_process_name.so \
    /system/vendor/lib/libmmcamera2_imglib_modules.so|libcamera_shim.so \
    /system/lib/libmmjpeg_interface.so|libshim_atomic.so
