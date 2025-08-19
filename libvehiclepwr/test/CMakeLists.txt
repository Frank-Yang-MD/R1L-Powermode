project(VPD_LIB_TEST CXX)

file(GLOB_RECURSE VPD_LIB_SRC "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp")

enable_testing()

add_executable(vpd_lib_test "${VPD_LIB_SRC}")

SET_TARGET_PROPERTIES(vpd_lib_test PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(vpd_lib_test PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(vpd_lib_test vpd_lib gtest gtest_main gmock pthread logdogcommon)
