#
# COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
# ALL RIGHTS RESERVED
#

PRODUCT_PACKAGES += VehiclePowerService \
                    vehiclepowerdaemon \
                    com.mitsubishielectric.ahu.vehiclepwrmgr.vpmbridgedaemon@1.0-service \
                    com.mitsubishielectric.ahu.vehiclepwrmgr.vpmbridgedaemon@1.0-service-perm

BOARD_SEPOLICY_DIRS += vendor/melco/efw/VehiclePwrMgr/sepolicy

PRODUCT_BOOT_JARS += com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr

# Adding VpmBridgeDaemon info to /vendor/manifest.xml
DEVICE_MANIFEST_FILE += \
    $(LOCAL_PATH)/../vpmbridgedaemon/1.0/manifest.xml

DEVICE_FRAMEWORK_COMPATIBILITY_MATRIX_FILE += \
    $(LOCAL_PATH)/../vpmbridgedaemon/1.0/vpmbridgedaemon_compatibility_matrix.xml

BOARD_SEPOLICY_DIRS += vendor/melco/efw/VehiclePwrMgr/vpmbridgedaemon/sepolicy

$(call inherit-product-if-exists, vendor/melco/efw/VehiclePwrMgr/str/str.mk)
