# COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
# ALL RIGHTS RESERVED

PRODUCT_PACKAGES += \
    init.pre_suspend.sh \
    init.post_wakeup.sh

BOARD_SEPOLICY_DIRS += vendor/melco/efw/VehiclePwrMgr/str/sepolicy
