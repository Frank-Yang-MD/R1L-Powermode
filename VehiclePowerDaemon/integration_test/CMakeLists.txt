project(VPD_INTEGRATION_TESTS CXX)

file(GLOB_RECURSE VPD_INT_TEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

enable_testing()

add_executable(vpd_int_test "${VPD_INT_TEST_SRC}")

SET_TARGET_PROPERTIES(vpd_int_test PROPERTIES LINKER_LANGUAGE CXX)


target_include_directories(vpd_int_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/mocks
    ${VPD_ROOT}/VehiclePowerDaemon/test/WakeUpEventDetector/
    ${VPD_ROOT}/VehiclePowerDaemon/test/ShutdownManager/
)

target_link_libraries(vpd_int_test VPD gtest gtest_main gmock pthread melcocommon cpucomv2 logdogcommon melcocommonnet)
