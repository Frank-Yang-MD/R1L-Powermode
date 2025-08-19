/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gtest/gtest.h>

#include "StartNativeCmdConditionalResult.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StartNativeCmdConditionalResultTest : public ::testing::Test {
protected:
    StartNativeCmdConditionalResultTest() {}

    StartNativeCmdConditionalResult mStartNativeCmdConditionalResult;
};

TEST_F(StartNativeCmdConditionalResultTest, isDone_return_true)
{
    EXPECT_TRUE(mStartNativeCmdConditionalResult.isDone());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com