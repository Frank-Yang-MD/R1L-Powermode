/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKTIMEOUTCONTROL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKTIMEOUTCONTROL_H_

#include <chrono>
#include <gmock/gmock.h>

#include "ITimeoutControl.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockTimeOutControl : public ITimeoutControl {
public:
    MOCK_METHOD0(stop, void());
    MOCK_METHOD0(isStarted, bool());
    MOCK_METHOD2(setupTimeout, void(int, std::chrono::milliseconds));
    MOCK_METHOD1(setupTimeout, void(std::chrono::microseconds));
    MOCK_METHOD1(clearTimeout, void(int));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKTIMEOUTCONTROL_H_
