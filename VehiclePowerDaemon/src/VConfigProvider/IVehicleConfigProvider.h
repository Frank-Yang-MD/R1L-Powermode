/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVCONF_PR_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IVCONF_PR_H_

#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

struct TimeoutConfig {
    int VPowerAppStopJavaTimeout;
    int VPowerAppStopLogdogTimeout;
    int VPowerAppStopNativeTimeout;
    int VPowerAppResumeEfwTimeout;
    int VPowerAppResumeAppTimeout;
    int VPowerLogSaveTimeout;
};

class IVehicleConfigProvider {
public:
    // LCOV_EXCL_START
    virtual ~IVehicleConfigProvider() = default;
    // LCOV_EXCL_STOP

    virtual const TimeoutConfig& getConfig() = 0;
    virtual std::string getInitString() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VCONF_PR_H_
