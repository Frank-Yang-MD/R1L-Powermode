/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKPOWERSTATEHOLDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKPOWERSTATEHOLDER_H_

#include <IPowerStateHolder.h>

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockPowerStateChangeListener : public IPowerStateChangeListener {
public:
    MOCK_METHOD1(onPowerStateChange, void(PowerState));
};

class MockPowerStateHolder : public IPowerStateHolder {
public:
    MOCK_METHOD1(onPowerStateChange, void(PowerState));
    MOCK_CONST_METHOD0(isShutdownState, bool());
    MOCK_CONST_METHOD0(getPowerState, PowerState());
    MOCK_METHOD0(onTimeout, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_MOCKPOWERSTATEHOLDER_H_
