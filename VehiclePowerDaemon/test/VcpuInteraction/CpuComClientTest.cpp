/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <CommandParser.h>
#include <CpuComClient.h>

#include "MockEventReceiver.h"
#include <mock/mock_CpuCom.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::_;
using ::testing::Eq;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Return;

using namespace cpucom::v2;

/**
 * Verify that the IVcpuInteraction correctly starts
 */
class IVcpuInteractionTest : public ::testing::Test {
public:
    IVcpuInteraction* mVcpuInteraction;
    NiceMock<MockEventReceiver> mEventReceiver;
    NiceMock<mock_ICpuCom>* mCpuCom;

    IVcpuInteractionTest()
    {
        mVcpuInteraction = nullptr;
        mCpuCom = nullptr;
    }

    ~IVcpuInteractionTest()
    {
        if (mVcpuInteraction) {
            delete mVcpuInteraction;
        }
    }

    /**
     * VCPU commands
     */
    const CpuCommand adtrRequest = {0x71, 0x04};
    const CpuCommand startCompleteNotification = {0x71, 0x81};
    const CpuCommand shutdownCancelAcceptResponse = {0x71, 0x85};
    const CpuCommand shutdownStartNotification = {0x71, 0x83};
    const CpuCommand shutdownCancelComplete = {0x71, 0x0D};
    const CpuCommand wdtrRequest = {0x71, 0x06};
    const CpuCommand mainStartCompleteCmd = {0x71, 0x87};

    void initVcpuInteration()
    {
        mCpuCom = new NiceMock<mock_ICpuCom>();
        mVcpuInteraction = new CpuComClient{mEventReceiver,
                                            [this]() { return std::unique_ptr<ICpuCom>(mCpuCom); }};
        mVcpuInteraction->start();
    }

    void terminateVcpuInteration()
    {
        if (mCpuCom) {
            delete mCpuCom;
        }
    }
};

/**
 * Test successfull start and subscribtion
 * CpuCom pointer points to real object
 */
TEST_F(IVcpuInteractionTest, SuccessfullStartTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, [this]() { return std::unique_ptr<ICpuCom>(mCpuCom); }};

    {
        InSequence s;

        EXPECT_CALL(*mCpuCom, initialize(_, _)).Times(1);
        EXPECT_CALL(*mCpuCom, subscribe(CommandParser::SHUTDOWN_REQUEST, _)).Times(1);
        EXPECT_CALL(*mCpuCom, subscribe(CommandParser::SHUTDOWN_CANCEL_REQUEST, _)).Times(1);
        EXPECT_CALL(*mCpuCom, connect()).WillOnce(Return(true));
    }

    EXPECT_TRUE(mVcpuInteraction->start());
}

/**
 * Test failed start
 * CpuCom pointer points to null
 */
TEST_F(IVcpuInteractionTest, FailedStartTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};

    EXPECT_CALL(*mCpuCom, connect()).Times(0);
    EXPECT_CALL(*mCpuCom, subscribe(CommandParser::SHUTDOWN_REQUEST, _)).Times(0);
    EXPECT_CALL(*mCpuCom, subscribe(CommandParser::SHUTDOWN_CANCEL_REQUEST, _)).Times(0);
    EXPECT_CALL(*mCpuCom, initialize(_, _)).Times(0);

    EXPECT_FALSE(mVcpuInteraction->start());

    terminateVcpuInteration();
}

TEST_F(IVcpuInteractionTest, StopTest)
{
    initVcpuInteration();

    EXPECT_CALL(*mCpuCom, unsubscribe(CommandParser::SHUTDOWN_REQUEST)).Times(2);
    EXPECT_CALL(*mCpuCom, unsubscribe(CommandParser::SHUTDOWN_CANCEL_REQUEST)).Times(2);
    EXPECT_CALL(*mCpuCom, disconnect()).Times(1);

    mVcpuInteraction->stop();
}

TEST_F(IVcpuInteractionTest, StopNoCpuComTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};

    EXPECT_CALL(*mCpuCom, unsubscribe(CommandParser::SHUTDOWN_REQUEST)).Times(0);
    EXPECT_CALL(*mCpuCom, unsubscribe(CommandParser::SHUTDOWN_CANCEL_REQUEST)).Times(0);
    EXPECT_CALL(*mCpuCom, disconnect()).Times(0);

    mVcpuInteraction->stop();

    terminateVcpuInteration();
}

TEST_F(IVcpuInteractionTest, AdtrRequestTest)
{
    initVcpuInteration();
    EXPECT_CALL(*mCpuCom, send(adtrRequest, std::vector<uint8_t>{})).Times(1);
    mVcpuInteraction->sendAdtrRequest();
}

TEST_F(IVcpuInteractionTest, AdtrRequestNoCpuComTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};
    EXPECT_CALL(*mCpuCom, send(adtrRequest, std::vector<uint8_t>{})).Times(0);
    mVcpuInteraction->sendAdtrRequest();

    terminateVcpuInteration();
}

TEST_F(IVcpuInteractionTest, StartCompleteTest)
{
    initVcpuInteration();
    EXPECT_CALL(*mCpuCom, send(startCompleteNotification, std::vector<uint8_t>{}, _)).Times(1);
    mVcpuInteraction->sendStartComplete();
}

TEST_F(IVcpuInteractionTest, StartCompleteNoCpuComTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};
    EXPECT_CALL(*mCpuCom, send(startCompleteNotification, std::vector<uint8_t>{}, _)).Times(0);
    mVcpuInteraction->sendStartComplete();

    terminateVcpuInteration();
}

TEST_F(IVcpuInteractionTest, ShutdownCancelAcceptTest)
{
    initVcpuInteration();
    EXPECT_CALL(*mCpuCom, send(shutdownCancelAcceptResponse, std::vector<uint8_t>{})).Times(1);
    mVcpuInteraction->sendShutdownCancelAccept();
}

TEST_F(IVcpuInteractionTest, ShutdownCancelAcceptNoCpuComTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};
    EXPECT_CALL(*mCpuCom, send(shutdownCancelAcceptResponse, std::vector<uint8_t>{})).Times(0);
    mVcpuInteraction->sendShutdownCancelAccept();

    terminateVcpuInteration();
}

TEST_F(IVcpuInteractionTest, ShutdownProcessingStartTest)
{
    initVcpuInteration();
    EXPECT_CALL(*mCpuCom, send(shutdownStartNotification, std::vector<uint8_t>{}, _)).Times(1);
    mVcpuInteraction->sendShutdownProcessingStart();
}

TEST_F(IVcpuInteractionTest, ShutdownProcessingStartNoCpuComTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};
    EXPECT_CALL(*mCpuCom, send(shutdownStartNotification, std::vector<uint8_t>{}, _)).Times(0);
    mVcpuInteraction->sendShutdownProcessingStart();

    terminateVcpuInteration();
}

TEST_F(IVcpuInteractionTest, ShutdownCancelCompleteTest)
{
    initVcpuInteration();
    EXPECT_CALL(*mCpuCom, send(shutdownCancelComplete, std::vector<uint8_t>{})).Times(1);
    mVcpuInteraction->sendShutdownCancelComplete();
}

TEST_F(IVcpuInteractionTest, ShutdownCancelCompleteNoCpuComTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};
    EXPECT_CALL(*mCpuCom, send(shutdownCancelComplete, std::vector<uint8_t>{})).Times(0);
    mVcpuInteraction->sendShutdownCancelComplete();

    terminateVcpuInteration();
}

TEST_F(IVcpuInteractionTest, WdtrRequestTest)
{
    initVcpuInteration();
    EXPECT_CALL(*mCpuCom, send(wdtrRequest, std::vector<uint8_t>{})).Times(1);
    mVcpuInteraction->sendWdtrRequest();
}

TEST_F(IVcpuInteractionTest, WdtrRequestNoCpuComTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};
    EXPECT_CALL(*mCpuCom, send(wdtrRequest, std::vector<uint8_t>{})).Times(0);
    mVcpuInteraction->sendWdtrRequest();

    terminateVcpuInteration();
}

TEST_F(IVcpuInteractionTest, DestructorNoCpuComTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    IVcpuInteraction* vcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};

    EXPECT_CALL(*mCpuCom, unsubscribe(CommandParser::SHUTDOWN_REQUEST)).Times(0);
    EXPECT_CALL(*mCpuCom, unsubscribe(CommandParser::SHUTDOWN_CANCEL_REQUEST)).Times(0);

    delete vcpuInteraction;
    terminateVcpuInteration();
}

TEST_F(IVcpuInteractionTest, must_send_SelfRebootTrue_to_VCPU_in_sendSelfRebootTrue_method)
{
    initVcpuInteration();

    const CpuCommand MAIN_STATUS_CHANGE_NOTIFICATION{0x85, 0x8F};
    const uint8_t dataTypeSelfReboot = 10;
    const uint8_t dataValueHigh = 0;
    const uint8_t dataValueLow = 1;

    std::vector<uint8_t> data{dataTypeSelfReboot, dataValueHigh, dataValueLow};

    EXPECT_CALL(*mCpuCom, send(Eq(MAIN_STATUS_CHANGE_NOTIFICATION), Eq(data))).Times(1);

    mVcpuInteraction->sendSelfRebootTrue();
}

TEST_F(IVcpuInteractionTest, must_not_send_SelfRebootTrue_to_VCPU_in_sendSelfRebootTrue_method)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};
    EXPECT_CALL(*mCpuCom, send(_, _)).Times(0);
    mVcpuInteraction->sendSelfRebootTrue();
    terminateVcpuInteration();
}

/**
 * Verifies that CpuComClient sends MAIN_START_COMPLETE command to CpuCom
 * when the interface function sendMainStartComplete() called.
 */
TEST_F(IVcpuInteractionTest, MainStartCompleteTest)
{
    initVcpuInteration();
    EXPECT_CALL(*mCpuCom, send(mainStartCompleteCmd, std::vector<uint8_t>{})).Times(1);
    mVcpuInteraction->sendMainStartComplete();
}

/**
 * Verifies that CpuComClient does not send MAIN_START_COMPLETE command to CpuCom
 * when the interface function sendMainStartComplete() is called but CpuCom is not initialized.
 */
TEST_F(IVcpuInteractionTest, MainStartCompleteNoCpuComTest)
{
    mCpuCom = new NiceMock<mock_ICpuCom>();
    mVcpuInteraction =
        new CpuComClient{mEventReceiver, []() { return std::unique_ptr<ICpuCom>(nullptr); }};
    EXPECT_CALL(*mCpuCom, send(mainStartCompleteCmd, std::vector<uint8_t>{})).Times(0);
    mVcpuInteraction->sendMainStartComplete();
    terminateVcpuInteration();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
