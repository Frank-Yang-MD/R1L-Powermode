/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpdStateMachineContext.h"

#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using testing::Test;

class VpdStateMachineContextTest : public Test {
protected:
    VpdStateMachineContext mContext;
};

TEST_F(VpdStateMachineContextTest, default_isVpsDisconnectedValue)
{
    ASSERT_TRUE(mContext.isVpsDisconnected());
}

TEST_F(VpdStateMachineContextTest, setVpsDisconnectedValue_true)
{
    const bool expectedValue{true};
    mContext.setVpsDisconnected(expectedValue);
    ASSERT_EQ(mContext.isVpsDisconnected(), expectedValue);
}

TEST_F(VpdStateMachineContextTest, setVpsDisconnectValue_false)
{
    const bool expectedValue{false};
    mContext.setVpsDisconnected(true);
    mContext.setVpsDisconnected(expectedValue);

    ASSERT_EQ(mContext.isVpsDisconnected(), expectedValue);
}

TEST_F(VpdStateMachineContextTest, default_isVpsStartCompleted)
{
    ASSERT_FALSE(mContext.isVpsStartCompleted());
}

TEST_F(VpdStateMachineContextTest, setVpsStartCompletedValue_true)
{
    const bool expectedValue{true};
    mContext.setVpsStartCompleted(expectedValue);
    ASSERT_EQ(mContext.isVpsStartCompleted(), expectedValue);
}

TEST_F(VpdStateMachineContextTest, setVpsStartCompletedValue_false)
{
    const bool expectedValue{false};
    mContext.setVpsStartCompleted(true);
    mContext.setVpsStartCompleted(expectedValue);
    ASSERT_EQ(mContext.isVpsStartCompleted(), expectedValue);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
