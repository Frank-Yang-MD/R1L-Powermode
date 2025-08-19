/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WakeUpEventDetector.h"

#include "IEventReceiverMock.h"

#include <mock/mock_IExecutor.h>
#include <mock/mock_IODevice.h>

#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using com::mitsubishielectric::ahu::common::IODevice;
using com::mitsubishielectric::ahu::common::mock_IExecutor;
using com::mitsubishielectric::ahu::common::mock_IODevice;

using testing::_;
using testing::ByMove;
using testing::InSequence;
using testing::Return;
using testing::SaveArg;

class WakeUpEventDetectorTest : public ::testing::Test {
public:
    mock_IExecutor executorMock;
    mock_IODevice ioDeviceMock;
    IEventReceiverMock eventReceiverMock;

    std::function<void()> detectorLoop;
    std::promise<void> pr;

    WakeUpEventDetector wakeUpEventDetector{executorMock, ioDeviceMock, eventReceiverMock};
};

TEST_F(WakeUpEventDetectorTest, openFailed)
{
    EXPECT_CALL(ioDeviceMock, open(IODevice::OpenMode::ReadOnly)).WillOnce(Return(false));
    EXPECT_CALL(executorMock, execute(_)).Times(0);

    wakeUpEventDetector.start();
}

TEST_F(WakeUpEventDetectorTest, openSuccess_readSuccess_wakeUpDetected)
{
    {
        InSequence seq;

        EXPECT_CALL(ioDeviceMock, open(IODevice::OpenMode::ReadOnly)).WillOnce(Return(true));
        EXPECT_CALL(ioDeviceMock, read(_, IODevice::kTimeoutImmediate))
            .WillOnce(Return(IODevice::Result::Success));

        EXPECT_CALL(executorMock, execute(_))
            .WillOnce(DoAll(SaveArg<0>(&detectorLoop), Return(ByMove(pr.get_future()))));

        EXPECT_CALL(ioDeviceMock, seek(0, IODevice::Whence::Beginning)).Times(1);

        EXPECT_CALL(ioDeviceMock, read(_, IODevice::kTimeoutInfinite, IODevice::Events::PollPri))
            .WillOnce([](auto* data, auto, auto) {
                data[0] = '0';
                return IODevice::Result::Success;
            });

        EXPECT_CALL(eventReceiverMock, onEventReceive(0, common::EVpdEvent::WakeUp))
            .WillOnce([this](auto, auto) { wakeUpEventDetector.stop(); });
    }

    wakeUpEventDetector.start();
    detectorLoop();
}

TEST_F(WakeUpEventDetectorTest, openSuccess_readSuccess_wakeNotUpDetected)
{
    {
        InSequence seq;

        EXPECT_CALL(ioDeviceMock, open(IODevice::OpenMode::ReadOnly)).WillOnce(Return(true));
        EXPECT_CALL(ioDeviceMock, read(_, IODevice::kTimeoutImmediate))
            .WillOnce(Return(IODevice::Result::Success));

        EXPECT_CALL(executorMock, execute(_))
            .WillOnce(DoAll(SaveArg<0>(&detectorLoop), Return(ByMove(pr.get_future()))));

        EXPECT_CALL(ioDeviceMock, seek(0, IODevice::Whence::Beginning)).Times(1);

        EXPECT_CALL(ioDeviceMock, read(_, IODevice::kTimeoutInfinite, IODevice::Events::PollPri))
            .WillOnce([this](auto* data, auto, auto) {
                data[0] = '1';
                wakeUpEventDetector.stop();
                return IODevice::Result::Success;
            });

        EXPECT_CALL(eventReceiverMock, onEventReceive(_, _)).Times(0);
    }

    wakeUpEventDetector.start();
    detectorLoop();
}

TEST_F(WakeUpEventDetectorTest, openSuccess_readError_reopenSuccess)
{
    {
        InSequence seq;

        EXPECT_CALL(ioDeviceMock, open(IODevice::OpenMode::ReadOnly)).WillOnce(Return(true));
        EXPECT_CALL(ioDeviceMock, read(_, IODevice::kTimeoutImmediate))
            .WillOnce(Return(IODevice::Result::Success));

        EXPECT_CALL(executorMock, execute(_))
            .WillOnce(DoAll(SaveArg<0>(&detectorLoop), Return(ByMove(pr.get_future()))));

        EXPECT_CALL(ioDeviceMock, seek(0, IODevice::Whence::Beginning)).Times(1);

        EXPECT_CALL(ioDeviceMock, read(_, IODevice::kTimeoutInfinite, IODevice::Events::PollPri))
            .WillOnce(Return(IODevice::Result::Error));

        EXPECT_CALL(ioDeviceMock, close()).Times(1);

        EXPECT_CALL(ioDeviceMock, open(IODevice::OpenMode::ReadOnly)).WillOnce(Return(true));
        EXPECT_CALL(ioDeviceMock, read(_, IODevice::kTimeoutImmediate))
            .WillOnce([this](auto*, auto) {
                wakeUpEventDetector.stop();
                return IODevice::Result::Success;
            });
    }

    wakeUpEventDetector.start();
    detectorLoop();
}

TEST_F(WakeUpEventDetectorTest, openSuccess_readError_reopenFailed)
{
    // +1 for the first try, which is not considered as retry
    constexpr int retryNumber = 10 + 1;

    EXPECT_CALL(ioDeviceMock, open(IODevice::OpenMode::ReadOnly))
        .Times(retryNumber)
        .WillRepeatedly(Return(true));
    EXPECT_CALL(ioDeviceMock, read(_, IODevice::kTimeoutImmediate)).Times(retryNumber);
    EXPECT_CALL(ioDeviceMock, seek(0, IODevice::Whence::Beginning)).Times(retryNumber);

    EXPECT_CALL(executorMock, execute(_))
        .WillOnce(DoAll(SaveArg<0>(&detectorLoop), Return(ByMove(pr.get_future()))));

    EXPECT_CALL(ioDeviceMock, read(_, IODevice::kTimeoutInfinite, IODevice::Events::PollPri))
        .Times(retryNumber)
        .WillRepeatedly(Return(IODevice::Result::Error));

    // -1 because close is called only on error
    EXPECT_CALL(ioDeviceMock, close()).Times(retryNumber - 1);

    wakeUpEventDetector.start();
    detectorLoop();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
