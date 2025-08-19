/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKVEHICLECONFIGPROVIDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKVEHICLECONFIGPROVIDER_H_

#include "IVehicleConfigProvider.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockVehicleConfigProvider : public IVehicleConfigProvider {
public:
    MOCK_METHOD0(getConfig, const TimeoutConfig&());
    MOCK_METHOD0(getInitString, std::string());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKVEHICLECONFIGPROVIDER_H_
