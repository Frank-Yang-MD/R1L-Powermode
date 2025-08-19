#!/bin/bash
###---Version-tag:1.1

[ -n "$ANDROID_BUILD_TOP" ] || \
    { echo "ERROR: \$ANDROID_BUILD_TOP variable is not set" >&2; exit 1; }
android_root="$ANDROID_BUILD_TOP"

doxygen_report_generator="${android_root}/vendor/melco/efw/devtools-config/\
doxygen-common/doxygen_report_generator.sh"

################################################################################
# Adjust these variables if needed.
################################################################################
proj_name="VehiclePowerDaemon"

proj_dir="${android_root}/vendor/melco/efw/VehiclePwrMgr/VehiclePowerDaemon"

input_dir="src"
################################################################################

${doxygen_report_generator} \
    --proj-name "${proj_name}" \
    --proj-dir "${proj_dir}" \
    --input-dir "${input_dir}" \
    "$@"
