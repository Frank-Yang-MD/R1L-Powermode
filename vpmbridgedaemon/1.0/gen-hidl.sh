#!/bin/bash

PACKAGE=com.mitsubishielectric.ahu.vehiclepwrmgr.vpmbridgedaemon@1.0
LOC=default
ROOT_PACKAGE_PATH=com.mitsubishielectric.ahu.vehiclepwrmgr:vendor/melco/efw/VehiclePwrMgr

hidl-gen -o $LOC -Lc++-impl -r$ROOT_PACKAGE_PATH $PACKAGE
hidl-gen -o $LOC -Landroidbp-impl -r$ROOT_PACKAGE_PATH $PACKAGE

source $ANDROID_BUILD_TOP/system/tools/hidl/update-makefiles-helper.sh

do_makefiles_update $ROOT_PACKAGE_PATH
