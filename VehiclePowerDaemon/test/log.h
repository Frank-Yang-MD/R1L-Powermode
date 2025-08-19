//
// Created by rmakarenko on 06.07.22.
//

#ifndef VPD_LOG_H
#define VPD_LOG_H

#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class LogInitializer final {
public:
    LogInitializer() { InitializeVehiclePowerDaemonLogMessages(); }

    ~LogInitializer() { TerminateVehiclePowerDaemonLogMessages(); }
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
#endif  // VPD_LOG_H
