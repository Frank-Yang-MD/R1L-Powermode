/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ClientActions.h"
#include "MockStateSender.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

std::vector<common::EVpdState> vpdStates{
    common::EVpdState::APP_START_NA, common::EVpdState::APP_START_COLDBOOT,
    common::EVpdState::APP_STOP, common::EVpdState::APP_RESUME, common::EVpdState::APP_RESTART};

TEST(ClientActionsTest, trigger_destuctors_for_all_memory_type)
{
    MockStateSender stateSender;
    auto clientActions{std::make_unique<ClientActions>(stateSender)};
}

/**
 * Check if 'sendState' method is called to notify about system state changing
 */

TEST(ClientActionsTest, callNotifySystemChange)
{
    MockStateSender stateSender;
    ClientActions clientActions(stateSender);

    const int randomValidClientId = 123;
    for (const auto& validVpdState : vpdStates) {
        for (const auto& state : vpdStates) {
            if (state == validVpdState) {
                EXPECT_CALL(stateSender, sendState(randomValidClientId, state)).Times(1);
            }
            else {
                EXPECT_CALL(stateSender, sendState(randomValidClientId, state)).Times(0);
            }
        }
        clientActions.notifySystemStateChange(randomValidClientId, validVpdState);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
