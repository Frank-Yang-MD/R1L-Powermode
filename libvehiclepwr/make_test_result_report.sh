#!/bin/bash
##---Version-tag:1.1

[ -n "$ANDROID_BUILD_TOP" ] || \
    { echo "ERROR: \$ANDROID_BUILD_TOP variable is not set" >&2; exit 1; }
android_root="$ANDROID_BUILD_TOP"

test_result_report_generator="${android_root}/vendor/melco/efw/devtools-config/\
unit-test-common/test_result_report_generator.sh"

################################################################################
# Adjust these variables if needed.
################################################################################
proj_name="libVehiclePwr"

proj_dir="${android_root}/vendor/melco/efw/VehiclePwrMgr/libvehiclepwr"

test_bin=${android_root}/out/host/linux-x86/nativetest64/\
libvehiclepwr_tests/libvehiclepwr_tests
################################################################################

${test_result_report_generator} \
    --proj-name "${proj_name}" \
    --proj-dir "${proj_dir}" \
    --test-bin "${test_bin}" \
    "$@"
