/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef MOCKTIMERHANDLER
#define MOCKTIMERHANDLER

#include "EventHandlingMocks.h"
#include "ITimerHandler.h"
#include "MockTimer.h"
#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockTimerHandling : public ITimerHandler {
public:
    MOCK_METHOD0(start, void());
    MOCK_CONST_METHOD0(isExpired, bool());
    MOCK_METHOD0(onTimeout, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif
