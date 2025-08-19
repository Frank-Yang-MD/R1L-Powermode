/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VCONF_PR_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VCONF_PR_H_

#include "IConfigReader.h"
#include "IVehicleConfigProvider.h"

#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class VehicleConfigProvider : public IVehicleConfigProvider {
public:
    VehicleConfigProvider(const IConfigReader&);
    inline const TimeoutConfig& getConfig() override { return mTimeoutConfig; }
    std::string getInitString() override;

private:
    const IConfigReader& mReader;
    struct TimeoutConfig mTimeoutConfig;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VCONF_PR_H_
