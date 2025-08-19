project(vehiclepowerdaemon CXX)

file(GLOB_RECURSE VPD_SRC "${VPD_ROOT}/VehiclePowerDaemon/src/*.cpp")
list(REMOVE_ITEM VPD_SRC "${VPD_ROOT}/VehiclePowerDaemon/src/main.cpp")

add_library(VPD ${VPD_SRC})
SET_TARGET_PROPERTIES(VPD PROPERTIES LINKER_LANGUAGE CXX)

target_include_directories(VPD PUBLIC
    ${VPD_ROOT}/VehiclePowerDaemon/src/
    ${VPD_ROOT}/VehiclePowerDaemon/src/ClientsInteraction/
    ${VPD_ROOT}/VehiclePowerDaemon/src/EventHandling/
    ${VPD_ROOT}/VehiclePowerDaemon/src/EventQueue/
    ${VPD_ROOT}/VehiclePowerDaemon/src/EventTransport/
    ${VPD_ROOT}/VehiclePowerDaemon/src/LogDaemonActions/
    ${VPD_ROOT}/VehiclePowerDaemon/src/ModulesControl/
    ${VPD_ROOT}/VehiclePowerDaemon/src/NativeRunner/
    ${VPD_ROOT}/VehiclePowerDaemon/src/ShutdownManager/
    ${VPD_ROOT}/VehiclePowerDaemon/src/StateHolder/
    ${VPD_ROOT}/VehiclePowerDaemon/src/StateMachine/
    ${VPD_ROOT}/VehiclePowerDaemon/src/States/
    ${VPD_ROOT}/VehiclePowerDaemon/src/SubscriptionManager/
    ${VPD_ROOT}/VehiclePowerDaemon/src/TimeoutNotification/
    ${VPD_ROOT}/VehiclePowerDaemon/src/VConfigProvider/
    ${VPD_ROOT}/VehiclePowerDaemon/src/VcpuInteraction/
    ${VPD_ROOT}/VehiclePowerDaemon/src/WakeUpEventDetector/
)

target_link_libraries(VPD log vehicleconfig)
