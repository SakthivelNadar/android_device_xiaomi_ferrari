#!/vendor/bin/sh

# Set touchpanel nodes ownership (these are proc_symlinks to the real sysfs nodes)
chown -LR system.system /proc/touchpanel
