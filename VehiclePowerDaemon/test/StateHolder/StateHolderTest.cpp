/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <EVpdState.h>
#include <StateHolder.h>
#include <gtest/gtest.h>

using namespace testing;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

extern std::vector<common::EVpdState> vpdStates;

class StateHolderTest : public Test {
protected:
    StateHolder mStateHolder;
};

TEST_F(StateHolderTest, CreateInstance)
{
    ASSERT_EQ(mStateHolder.getVpdState(), common::EVpdState::APP_START_NA);
}

TEST_F(StateHolderTest, SetState)
{
    for (const auto& state : vpdStates) {
        mStateHolder.setVpdState(state);

        ASSERT_EQ(state, mStateHolder.getVpdState());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
