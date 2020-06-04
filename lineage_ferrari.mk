# Copyright (C) 2014 The CyanogenMod Project
#           (C) 2017 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

$(call inherit-product, device/xiaomi/ferrari/full_ferrari.mk)

# Inherit some common LineageOS stuff.
$(call inherit-product, vendor/lineage/config/common_full_phone.mk)

# Must define platform variant before including any common things
TARGET_BOARD_PLATFORM_VARIANT := msm8939

# Apex
$(call inherit-product, $(SRC_TARGET_DIR)/product/updatable_apex.mk)

# Device identifier. This must come after all inclusions
PRODUCT_DEVICE := ferrari
PRODUCT_NAME := lineage_ferrari
PRODUCT_BRAND := Xiaomi
PRODUCT_MODEL := Mi 4i
PRODUCT_MANUFACTURER := Xiaomi

PRODUCT_GMS_CLIENTID_BASE := android-xiaomi

PRODUCT_BUILD_PROP_OVERRIDES += \
	PRIVATE_BUILD_DESC="ferrari-user 5.0.2 LRX22G V9.2.3.0.LXIMIEK release-keys" \
	PRODUCT_NAME="ferrari"

BUILD_FINGERPRINT := "Xiaomi/ferrari/ferrari:5.0.2/LRX22G/V9.2.3.0.LXIMIEK:user/release-keys"
