/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#pragma once

#include "IAndroidPowerManager.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockVpdBridgeAdapter : public IAndroidPowerManager {
public:
    MOCK_METHOD(void, shutdown, ());
    MOCK_METHOD(void, suspend, ());
    MOCK_METHOD(void, reboot, ());
    MOCK_METHOD(void, rebootRecovery, ());
    MOCK_METHOD(void, wakeUp, ());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
