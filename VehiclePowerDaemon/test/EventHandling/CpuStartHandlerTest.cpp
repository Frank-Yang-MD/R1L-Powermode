/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "MockIStartCompleteCpuCom.h"

#include "CpuStartHandler.h"

#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using testing::Test;

class CpuStartHandlerTest : public Test {
protected:
    CpuStartHandlerTest()
        : mCpuStartHandler{mStartCompleteCpuComMock}
    {
    }

    const int mClientId{111};
    MockIStartCompleteCpuCom mStartCompleteCpuComMock;
    CpuStartHandler mCpuStartHandler;
};

TEST_F(CpuStartHandlerTest, trigger_destructors_for_all_memory_types)
{
    auto cpuStartHandler{std::make_unique<CpuStartHandler>(mStartCompleteCpuComMock)};
}

TEST_F(CpuStartHandlerTest, must_call_onStartCompleteCpuCom_on_CpuComStarted_event)
{
    const common::EVpdEvent event{common::EVpdEvent::CpuComStarted};

    EXPECT_CALL(mStartCompleteCpuComMock, onStartCompleteCpuCom()).Times(1);

    mCpuStartHandler.Process(mClientId, event);
}

TEST_F(CpuStartHandlerTest, must_do_nothing_on_other_events)
{
    for (int i = static_cast<int>(common::EVpdEvent::Unsupported);
         i != static_cast<int>(common::EVpdEvent::EVPDEVENT_COUNT); i++) {
        if (i != common::EVpdEvent::CpuComStarted) {
            EXPECT_CALL(mStartCompleteCpuComMock, onStartCompleteCpuCom()).Times(0);
            mCpuStartHandler.Process(mClientId, static_cast<common::EVpdEvent>(i));
        }
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
