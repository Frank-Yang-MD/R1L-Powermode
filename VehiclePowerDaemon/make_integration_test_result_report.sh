#!/bin/sh
###---Version-tag:1.0

[ -n "$ANDROID_BUILD_TOP" ] || \
    { echo "ERROR: \$ANDROID_BUILD_TOP variable is not set" >&2; exit 1; }

integration_test_report_out_dir=${ANDROID_BUILD_TOP}/out/test_result_report/vendor/melco/efw/VehiclePwrMgr/VehiclePowerDaemon
report_file_name="$integration_test_report_out_dir/report.xml"
test_log_file_name="$integration_test_report_out_dir/integration_test_log.log"
test_result_report_generator=${ANDROID_BUILD_TOP}/vendor/melco/efw/devtools-config/unit-test-common/gen_gtest_report.py

mkdir -p ${integration_test_report_out_dir}

echo "Running integration tests..."

./run_integration_test.sh --gtest_output=xml:$report_file_name > ${test_log_file_name}


################################################################################
echo "Generating report..."
echo ""

${test_result_report_generator} ${report_file_name}
