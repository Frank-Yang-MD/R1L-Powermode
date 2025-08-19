/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#pragma once

#include "IWakeLockManager.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockWakeLockManager : public IWakeLockManager {
public:
    MOCK_METHOD(void, acquireWakeLock, ());
    MOCK_METHOD(void, releaseWakeLock, ());
    MOCK_METHOD(void, enableWakeLocks, ());
    MOCK_METHOD(void, disableWakeLocks, ());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
