/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ConfigReader.h"
#include "CpuCom.h"
#include "EventQueue.h"
#include "KernelModulesControl.h"
#include "LogDaemonActions.h"
#include "MessageServerWraper.h"
#include "Timer.h"
#include "VehiclePowerDaemon.h"
#include "VehiclePowerDaemonLog.h"
#include "VpdConst.h"
#include "VpdMessageServer.h"
#include "VpmBridgeDaemonAdapter.h"
#include "WakeLockManager.h"
#include "WakeUpManager.h"

#include "Executors.h"
#include "IODevice.h"
#include "libMelcoCommon.h"

#include <libCpuCom.h>
#include <libVehicleConfig.h>

namespace vpd = com::mitsubishielectric::ahu::vehiclepwrmgr::impl;
namespace vpd_common = com::mitsubishielectric::ahu::vehiclepwrmgr::common;
namespace cpucom = com::mitsubishielectric::ahu::cpucom;
namespace common = com::mitsubishielectric::ahu::common;
namespace vehicleconfig = com::mitsubishielectric::ahu::vehicleconfig;

namespace {
std::unique_ptr<cpucom::v2::ICpuCom> cpuComClientCreator() { return cpucom::v2::ICpuCom::create(); }

using namespace std::chrono_literals;

const std::chrono::microseconds ABNORMAL_PERIOD{5s};
const std::chrono::microseconds WATCHDOG_PERIOD{9s};
const std::chrono::microseconds MODULES_UNLOAD_TIMEOUT{10s};
}  // namespace

int main()
{
    com::mitsubishielectric::ahu::common::InitializeCommonLogMessages();
    vpd::InitializeVehiclePowerDaemonLogMessages();
    com::mitsubishielectric::ahu::cpucom::InitializeLibCpuComLogMessages();
    vehicleconfig::InitializeLibVehicleConfigLogMessages();

    vpd::VpmBridgeDaemonAdapter vpdBridgeDaemonAdapter{};
    vpd::WakeLockManager wakeLockManager{};
    vpd::AndroidInit androidInit{};
    vpd::KernelModulesControl kernelModulesControl{androidInit};
    vpd::LogDaemonActions logDaemonActions{};
    vpd::ConfigReader configReader{vehicleconfig::IVehicleConfigReader::create()};
    vpd::EventQueue eventQueue{};
    vpd::VpdClock vpdClock{};
    common::SingleThreadExecutor wakeUpEventDetectorExecutor{};
    common::IODevice wakeUpIoDevice{"/sys/devices/platform/vcpu-gpio/str_trg_vtm"};
    vpd::WakeUpEventDetector wakeUpEventDetector{wakeUpEventDetectorExecutor, wakeUpIoDevice,
                                                 eventQueue};
    vpd::WakeUpManager wakeUpManager{vpdBridgeDaemonAdapter};

    common::MessageServer<vpd_common::EVpdMsgId> messageServer{
        vpd_common::SERVER_NAME, std::make_unique<common::SingleThreadExecutor>(),
        std::make_unique<common::SingleThreadExecutor>()};
    common::SingleThreadExecutor executor{};
    vpd::MessageServerWraper messageServerWrapper{messageServer, executor};
    vpd::VpdMessageServer vpdMessageServer{eventQueue, messageServerWrapper};
    vpdMessageServer.start();

    vpd::VehiclePowerDaemon vehiclePowerDaemon(
        vpdBridgeDaemonAdapter, wakeLockManager, kernelModulesControl, logDaemonActions,
        configReader, eventQueue, eventQueue, vpdMessageServer, cpuComClientCreator, vpdClock,
        androidInit, wakeUpEventDetector, wakeUpManager, ABNORMAL_PERIOD, WATCHDOG_PERIOD,
        MODULES_UNLOAD_TIMEOUT);
    vehiclePowerDaemon.StartDaemon();

    vehicleconfig::TerminateLibVehicleConfigLogMessages();
    com::mitsubishielectric::ahu::cpucom::TerminateLibCpuComLogMessages();
    vpd::TerminateVehiclePowerDaemonLogMessages();
    com::mitsubishielectric::ahu::common::TerminateCommonLogMessages();

    return 0;
}
