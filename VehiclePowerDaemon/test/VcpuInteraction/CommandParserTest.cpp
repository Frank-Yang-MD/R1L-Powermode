/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <CommandParser.h>
#include <CpuCommand.h>

#include "MockEventReceiver.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::_;

using com::mitsubishielectric::ahu::common::CpuCommand;

/**
 * Verify that commands from VCPU are correctly converted to corresponding message types
 * and incorrect VCPU commands are properly handled
 */
class CommandParserTest : public ::testing::Test {
public:
    CommandParserTest()
        : mCommandParser{mEventReceiver}
    {
    }

    MockEventReceiver mEventReceiver;
    CommandParser mCommandParser;
};

TEST_F(CommandParserTest, ShutdownRequest)
{
    EXPECT_CALL(mEventReceiver, onEventReceive(0, ::testing::Eq(common::EVpdEvent::Shutdown)))
        .Times(1);
    mCommandParser.onReceiveCommand(CommandParser::SHUTDOWN_REQUEST, {0x00});
}

TEST_F(CommandParserTest, SuspendRequest)
{
    EXPECT_CALL(mEventReceiver, onEventReceive(0, ::testing::Eq(common::EVpdEvent::Suspend)))
        .Times(1);
    mCommandParser.onReceiveCommand(CommandParser::SHUTDOWN_REQUEST, {0x01});
}

TEST_F(CommandParserTest, RebootRequest)
{
    EXPECT_CALL(mEventReceiver, onEventReceive(0, ::testing::Eq(common::EVpdEvent::Reboot)))
        .Times(1);
    mCommandParser.onReceiveCommand(CommandParser::SHUTDOWN_REQUEST, {0x02});
}

TEST_F(CommandParserTest, RebootRecoveryRequest)
{
    EXPECT_CALL(mEventReceiver, onEventReceive(0, ::testing::Eq(common::EVpdEvent::RebootRecovery)))
        .Times(1);
    mCommandParser.onReceiveCommand(CommandParser::SHUTDOWN_REQUEST, {0x03});
}

TEST_F(CommandParserTest, ShutdownCancelRequest)
{
    EXPECT_CALL(mEventReceiver, onEventReceive(0, ::testing::Eq(common::EVpdEvent::ShutdownCancel)))
        .Times(1);
    mCommandParser.onReceiveCommand(CommandParser::SHUTDOWN_CANCEL_REQUEST, {});
}

TEST_F(CommandParserTest, ShutdownRequestWrongData)
{
    EXPECT_CALL(mEventReceiver, onEventReceive(_, _)).Times(0);
    mCommandParser.onReceiveCommand(CommandParser::SHUTDOWN_REQUEST, {0x05});
}

TEST_F(CommandParserTest, ShutdownRequestEmptyData)
{
    EXPECT_CALL(mEventReceiver, onEventReceive(_, _)).Times(0);
    mCommandParser.onReceiveCommand(CommandParser::SHUTDOWN_REQUEST, {});
}

TEST_F(CommandParserTest, ShutdownCancelRequestWrongData)
{
    EXPECT_CALL(mEventReceiver, onEventReceive(_, _)).Times(0);
    mCommandParser.onReceiveCommand(CommandParser::SHUTDOWN_CANCEL_REQUEST, {0x03});
}

TEST_F(CommandParserTest, WrongCommand)
{
    EXPECT_CALL(mEventReceiver, onEventReceive(_, _)).Times(0);
    mCommandParser.onReceiveCommand(com::mitsubishielectric::ahu::common::CpuCommand{20, 91}, {});
}

TEST_F(CommandParserTest, WrongCommandError)
{
    EXPECT_CALL(mEventReceiver, onEventReceive(_, _)).Times(0);
    mCommandParser.onError(com::mitsubishielectric::ahu::common::CpuCommand{20, 91}, 1);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
