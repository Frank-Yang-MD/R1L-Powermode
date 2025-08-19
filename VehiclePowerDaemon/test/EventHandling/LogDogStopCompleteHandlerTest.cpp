/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "LogdogStopCompleteHandler.h"
#include "MockIStopCompleteLogMock.h"

#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using testing::Test;

class LogdogStopCompleteHandlerTest : public Test {
protected:
    LogdogStopCompleteHandlerTest()
        : mLogdogStopCompleteHandler{mStopCompleteLogMock}
    {
    }

    const int mClientId{111};
    MockIStopCompleteLog mStopCompleteLogMock;
    LogdogStopCompleteHandler mLogdogStopCompleteHandler;
};

TEST_F(LogdogStopCompleteHandlerTest, trigger_destructors_for_all_memory_types)
{
    auto logdogStopCompleteHandler{
        std::make_unique<LogdogStopCompleteHandler>(mStopCompleteLogMock)};
}

TEST_F(LogdogStopCompleteHandlerTest, must_call_StopCompleteLogdog_on_StopCompleteLogdog_event)
{
    const common::EVpdEvent event{common::EVpdEvent::StopCompleteLogdog};
    EXPECT_CALL(mStopCompleteLogMock, onStopCompleteLog()).Times(1);
    mLogdogStopCompleteHandler.Process(mClientId, event);
}

TEST_F(LogdogStopCompleteHandlerTest, must_do_nothing_on_other_events)
{
    for (int i = static_cast<int>(common::EVpdEvent::Unsupported);
         i != static_cast<int>(common::EVpdEvent::EVPDEVENT_COUNT); i++) {
        if (i != common::EVpdEvent::StopCompleteLogdog) {
            EXPECT_CALL(mStopCompleteLogMock, onStopCompleteLog()).Times(0);
            mLogdogStopCompleteHandler.Process(mClientId, static_cast<common::EVpdEvent>(i));
        }
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
