/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "IConditionMock.h"
#include "IConfigScriptCommandMock.h"
#include "StartNativeCmdConditional.h"

using ::testing::Return;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StartNativeCmdConditionalTest : public ::testing::Test {
public:
    StartNativeCmdConditionalTest();

    IConfigScriptCommandMock* mIConfigScriptCommandMock;
    IConditionMock* mIConditionMock;
    StartNativeCmdConditional mStartNativeCmdConditional;
};

StartNativeCmdConditionalTest::StartNativeCmdConditionalTest()
    : mIConfigScriptCommandMock(new IConfigScriptCommandMock())
    , mIConditionMock(new IConditionMock())
    , mStartNativeCmdConditional(
          std::unique_ptr<IConfigScriptCommandMock>(mIConfigScriptCommandMock),
          std::unique_ptr<IConditionMock>(mIConditionMock))
{
}

/**
 * Verifies that the function execute() of the StartNativeCmdConditional
 * calls the function execute() of the IConfigScriptCommand.
 */
TEST_F(StartNativeCmdConditionalTest,
       execute_of_StartNativeCmdConditional_calls_execute_of_IConfigScriptCommand)
{
    // Arrangements
    ON_CALL(*mIConditionMock, isSatisfied()).WillByDefault(Return(true));

    // Expectation
    EXPECT_CALL(*mIConfigScriptCommandMock, execute()).Times(1);

    // Action
    mStartNativeCmdConditional.execute();
}

/**
 * Vewrifies that the function execute() of the StartNativeCmdConditional
 * returns not empty pointer
 */
TEST_F(StartNativeCmdConditionalTest,
       execute_of_StartNativeCmdConditional_returns_not_empty_pointer)
{
    // Arrangements
    ON_CALL(*mIConditionMock, isSatisfied()).WillByDefault(Return(false));

    // Action
    std::unique_ptr<ICommandResult> cmdResult = mStartNativeCmdConditional.execute();

    // Verification
    EXPECT_NE(cmdResult.get(), nullptr);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
