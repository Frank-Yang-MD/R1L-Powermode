/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef MOCKWATCHDOGTIMERHANDLER
#define MOCKWATCHDOGTIMERHANDLER

#include "MockActions.h"
#include "WatchDogTimerHandler.h"
#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockWatchDogTimerHandler : public WatchDogTimerHandler {
public:
    MockWatchDogTimerHandler()
        : WatchDogTimerHandler(nullptr, mMockVcpuActions)
    {
    }

    MOCK_METHOD0(start, void());
    MOCK_CONST_METHOD0(isExpired, bool());
    MOCK_METHOD0(onTimeout, void());

private:
    MockVcpuActions mMockVcpuActions;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // MOCKWATCHDOGTIMERHANDLER
