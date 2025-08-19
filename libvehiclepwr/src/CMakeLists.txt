project(VPD_LIB CXX)

file(GLOB_RECURSE VPD_LIB_SRC "${VPD_ROOT}/libvehiclepwr/src/*.cpp")

add_library(vpd_lib ${VPD_LIB_SRC})

target_include_directories(vpd_lib PUBLIC
    ${VPD_ROOT}/libvehiclepwr/src/
    ${VPD_ROOT}/libvehiclepwr/include/
    ${VPD_ROOT}/libvehiclepwr/src/BootDaemon/
    ${VPD_ROOT}/libvehiclepwr/src/Client/
    ${VPD_ROOT}/libvehiclepwr/src/CpuCom/
    ${VPD_ROOT}/libvehiclepwr/src/EventHandling/
    ${VPD_ROOT}/libvehiclepwr/src/Log/
    ${VPD_ROOT}/libvehiclepwr/src/Logdog/
    ${VPD_ROOT}/libvehiclepwr/src/Messenger/
    ${VPD_ROOT}/libvehiclepwr/src/NativeClient/
    ${VPD_ROOT}/libvehiclepwr/src/Vps/
)

set_target_properties(vpd_lib PROPERTIES LINKER_LANGUAGE CXX)

target_link_libraries(vpd_lib)
