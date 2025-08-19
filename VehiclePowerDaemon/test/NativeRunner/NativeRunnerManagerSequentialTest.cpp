/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NativeRunnerManagerSequential.h"
#include "MockConfigScriptCmdCreator.h"
#include "MockNativeConfigProvider.h"
#include "MockStartNativeCommand.h"
#include "MockStartNativeCommandResult.h"

#include <gtest/gtest.h>
namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using namespace testing;
using config_t = std::vector<std::string>;
using cmdMock_t = std::unique_ptr<MockStartNativeCommand>;
using cmdResultMock_t = std::unique_ptr<MockStartNativeCommandResult>;

class NativeRunnerManagerSequentialTest : public Test {
public:
    NativeRunnerManagerSequentialTest()
        : mNativeRunnerManager(mConfigProvider, mConfigScriptCmdCreator)
    {
    }

    void SetUp() override
    {
        mFullConfig.push_back(cmd1);
        mFullConfig.push_back(cmd2);
        mFullConfig.push_back(cmd3);
    }

    std::string cmd1{"property1: daemon1"};
    std::string cmd2{"property2: daemon2"};
    std::string cmd3{"property3: daemon3"};

    cmdMock_t cmdMock1 = std::make_unique<MockStartNativeCommand>();
    cmdMock_t cmdMock2 = std::make_unique<MockStartNativeCommand>();
    cmdMock_t cmdMock3 = std::make_unique<MockStartNativeCommand>();

    MockStartNativeCommand* cmdMock1Raw = cmdMock1.get();
    MockStartNativeCommand* cmdMock2Raw = cmdMock2.get();
    MockStartNativeCommand* cmdMock3Raw = cmdMock3.get();

    cmdResultMock_t cmdResult1 = std::make_unique<MockStartNativeCommandResult>();
    cmdResultMock_t cmdResult2 = std::make_unique<MockStartNativeCommandResult>();
    cmdResultMock_t cmdResult3 = std::make_unique<MockStartNativeCommandResult>();

    MockStartNativeCommandResult* cmdResult1Raw = cmdResult1.get();
    MockStartNativeCommandResult* cmdResult2Raw = cmdResult2.get();
    MockStartNativeCommandResult* cmdResult3Raw = cmdResult3.get();

    config_t mEmptyConfig;
    config_t mFullConfig;
    const int mFullConfigSize{3};

    MockNativeConfigProvider mConfigProvider;
    MockConfigScriptCmdCreator mConfigScriptCmdCreator;
    NativeRunnerManagerSequential mNativeRunnerManager;
};

TEST_F(NativeRunnerManagerSequentialTest, RunWithEmptyConfig)
{
    ON_CALL(mConfigProvider, loadConfig()).WillByDefault(Return(mEmptyConfig));
    EXPECT_CALL(mConfigProvider, loadConfig()).Times(1);

    EXPECT_CALL(mConfigScriptCmdCreator, createCommand(_)).Times(0);

    mNativeRunnerManager.run();
    EXPECT_TRUE(mNativeRunnerManager.isDone());
}

TEST_F(NativeRunnerManagerSequentialTest, RunWithFullConfig)
{
    ON_CALL(mConfigProvider, loadConfig()).WillByDefault(Return(mFullConfig));
    EXPECT_CALL(mConfigProvider, loadConfig()).Times(1);

    EXPECT_CALL(mConfigScriptCmdCreator, createCommand(cmd1))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(cmdMock1))));
    EXPECT_CALL(mConfigScriptCmdCreator, createCommand(cmd2))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(cmdMock2))));
    EXPECT_CALL(mConfigScriptCmdCreator, createCommand(cmd3))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(cmdMock3))));

    EXPECT_CALL(*cmdMock1Raw, execute()).Times(1).WillOnce(Return(ByMove(std::move(cmdResult1))));
    ON_CALL(*cmdMock2Raw, execute()).WillByDefault(Return(ByMove(std::move(cmdResult2))));
    ON_CALL(*cmdMock3Raw, execute()).WillByDefault(Return(ByMove(std::move(cmdResult3))));

    EXPECT_CALL(*cmdMock2Raw, execute()).Times(0);
    EXPECT_CALL(*cmdMock3Raw, execute()).Times(0);

    mNativeRunnerManager.run();

    ON_CALL(*cmdResult1Raw, isDone()).WillByDefault(Return(false));
    EXPECT_CALL(*cmdResult1Raw, isDone()).Times(1);
    EXPECT_FALSE(mNativeRunnerManager.isDone());
}

TEST_F(NativeRunnerManagerSequentialTest, RunWithFullConfigWithCreateCommandError)
{
    ON_CALL(mConfigProvider, loadConfig()).WillByDefault(Return(mFullConfig));
    EXPECT_CALL(mConfigProvider, loadConfig()).Times(1);

    EXPECT_CALL(mConfigScriptCmdCreator, createCommand(cmd1))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(nullptr))));
    EXPECT_CALL(mConfigScriptCmdCreator, createCommand(cmd2))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(nullptr))));
    EXPECT_CALL(mConfigScriptCmdCreator, createCommand(cmd3))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(nullptr))));

    mNativeRunnerManager.run();
    EXPECT_TRUE(mNativeRunnerManager.isDone());
}

TEST_F(NativeRunnerManagerSequentialTest, RunWithFullConfigWithExecuteCommandError)
{
    ON_CALL(mConfigProvider, loadConfig()).WillByDefault(Return(mFullConfig));
    EXPECT_CALL(mConfigProvider, loadConfig()).Times(1);

    EXPECT_CALL(mConfigScriptCmdCreator, createCommand(cmd1))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(cmdMock1))));
    EXPECT_CALL(mConfigScriptCmdCreator, createCommand(cmd2))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(cmdMock2))));
    EXPECT_CALL(mConfigScriptCmdCreator, createCommand(cmd3))
        .Times(1)
        .WillOnce(Return(ByMove(std::move(cmdMock3))));

    EXPECT_CALL(*cmdMock1Raw, execute()).Times(1).WillOnce(Return(ByMove(std::move(nullptr))));
    ON_CALL(*cmdMock2Raw, execute()).WillByDefault(Return(ByMove(std::move(nullptr))));
    ON_CALL(*cmdMock3Raw, execute()).WillByDefault(Return(ByMove(std::move(nullptr))));

    mNativeRunnerManager.run();

    EXPECT_CALL(*cmdResult1Raw, isDone()).Times(0);
    EXPECT_CALL(*cmdMock2Raw, execute()).Times(1);
    EXPECT_FALSE(mNativeRunnerManager.isDone());

    EXPECT_CALL(*cmdResult2Raw, isDone()).Times(0);
    EXPECT_CALL(*cmdMock3Raw, execute()).Times(1);
    EXPECT_FALSE(mNativeRunnerManager.isDone());

    EXPECT_CALL(*cmdResult3Raw, isDone()).Times(0);
    EXPECT_TRUE(mNativeRunnerManager.isDone());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
