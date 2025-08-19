project(VPD_TESTS CXX)

file(GLOB_RECURSE VPD_GTEST_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

enable_testing()

add_executable(vpd_gtest "${VPD_GTEST_SRC}")

SET_TARGET_PROPERTIES(vpd_gtest PROPERTIES LINKER_LANGUAGE CXX)


target_include_directories(vpd_gtest PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/ClientInteraction/
    ${CMAKE_CURRENT_SOURCE_DIR}/EventHandling/
    ${CMAKE_CURRENT_SOURCE_DIR}/EventQueue/
    ${CMAKE_CURRENT_SOURCE_DIR}/EventTransport/
    ${CMAKE_CURRENT_SOURCE_DIR}/ModulesControl/
    ${CMAKE_CURRENT_SOURCE_DIR}/NativeRunner/
    ${CMAKE_CURRENT_SOURCE_DIR}/ShutdownManager/
    ${CMAKE_CURRENT_SOURCE_DIR}/StateHolder/
    ${CMAKE_CURRENT_SOURCE_DIR}/StateMachine/
    ${CMAKE_CURRENT_SOURCE_DIR}/States/
    ${CMAKE_CURRENT_SOURCE_DIR}/SubscriptionManager/
    ${CMAKE_CURRENT_SOURCE_DIR}/TimeoutNotification/
    ${CMAKE_CURRENT_SOURCE_DIR}/VConfigProvider/
    ${CMAKE_CURRENT_SOURCE_DIR}/VcpuInteraction/
    ${CMAKE_CURRENT_SOURCE_DIR}/WakeUpEventDetector/
)

target_link_libraries(vpd_gtest VPD gtest gtest_main gmock pthread melcocommon cpucomv2 logdogcommon melcocommonnet)
