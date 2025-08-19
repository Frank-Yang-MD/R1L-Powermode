/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpdMessageServer.h"
#include "MockEventReceiver.h"
#include "MockMessageServer.h"

#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::Test;

class VpdMessageServerTest : public Test {
protected:
    VpdMessageServerTest()
        : mPid(1)
        , mVpdNotification(common::EVpdNotifications::AppStartNa)
        , mReadyVpdEvent(common::EVpdEvent::VpsReady)
        , mDisconnectVpdEvent(common::EVpdEvent::Disconnect)
        , mEventReceiveId1("EventReceiveId1")
        , mEventReceiveId2("EventReceiveId2")
        , mVpdMessageServer(mEventReceiver, mMessageServer)
    {
    }
    ~VpdMessageServerTest() {}

    int mPid;
    common::EVpdNotifications mVpdNotification;
    common::EVpdEvent mReadyVpdEvent;
    common::EVpdEvent mDisconnectVpdEvent;

    SessionID mEventReceiveId1;
    SessionID mEventReceiveId2;

    std::function<void(SessionID)> mClientConnected;
    std::function<void(SessionID)> mClientDisconnected;
    std::function<void(SessionID, common::EVpdEvent, int)> mEventReceived;

    NiceMock<MockEventReceiver> mEventReceiver;
    NiceMock<MockMessageServer> mMessageServer;

    VpdMessageServer mVpdMessageServer;
};

TEST_F(VpdMessageServerTest, clientConnectedTest)
{
    EXPECT_CALL(mMessageServer, initialize(_, _))
        .WillOnce(DoAll(SaveArg<0>(&mClientConnected), SaveArg<1>(&mClientDisconnected)));
    EXPECT_CALL(mMessageServer, setMessageHandler(_));
    EXPECT_CALL(mMessageServer, start());

    mVpdMessageServer.start();
    mClientConnected(mEventReceiveId1);
    mClientDisconnected(mEventReceiveId1);
}

TEST_F(VpdMessageServerTest, clientDisconnectedWithEventReceivedTest)
{
    EXPECT_CALL(mMessageServer, initialize(_, _))
        .WillOnce(DoAll(SaveArg<0>(&mClientConnected), SaveArg<1>(&mClientDisconnected)));
    EXPECT_CALL(mMessageServer, setMessageHandler(_)).WillOnce(DoAll(SaveArg<0>(&mEventReceived)));
    EXPECT_CALL(mMessageServer, start());
    EXPECT_CALL(mEventReceiver, onEventReceive(mPid, mReadyVpdEvent));
    EXPECT_CALL(mEventReceiver, onEventReceive(mPid, mDisconnectVpdEvent));

    mVpdMessageServer.start();
    mClientConnected(mEventReceiveId1);
    mEventReceived(mEventReceiveId1, mReadyVpdEvent, mPid);
    mClientDisconnected(mEventReceiveId1);
}

TEST_F(VpdMessageServerTest, clientDisconnectedWithDifferentEventReceivedTest)
{
    EXPECT_CALL(mMessageServer, initialize(_, _))
        .WillOnce(DoAll(SaveArg<0>(&mClientConnected), SaveArg<1>(&mClientDisconnected)));
    EXPECT_CALL(mMessageServer, setMessageHandler(_)).WillOnce(DoAll(SaveArg<0>(&mEventReceived)));
    EXPECT_CALL(mMessageServer, start());
    EXPECT_CALL(mEventReceiver, onEventReceive(mPid, mReadyVpdEvent)).Times(2);

    mVpdMessageServer.start();
    mClientConnected(mEventReceiveId1);
    mEventReceived(mEventReceiveId1, mReadyVpdEvent, mPid);
    mEventReceived(mEventReceiveId2, mReadyVpdEvent, mPid);
    mClientDisconnected(mEventReceiveId2);
}

TEST_F(VpdMessageServerTest, sendNotificationTest)
{
    EXPECT_CALL(mMessageServer, initialize(_, _));
    EXPECT_CALL(mMessageServer, setMessageHandler(_));
    EXPECT_CALL(mMessageServer, start());
    EXPECT_CALL(mMessageServer, sendNotification(mEventReceiveId1, mVpdNotification)).Times(0);

    mVpdMessageServer.start();
    mVpdMessageServer.sendNotification(mPid, mVpdNotification);
}

TEST_F(VpdMessageServerTest, sendNotificationWithEventReceivedTest)
{
    EXPECT_CALL(mMessageServer, initialize(_, _));
    EXPECT_CALL(mMessageServer, setMessageHandler(_)).WillOnce(DoAll(SaveArg<0>(&mEventReceived)));
    EXPECT_CALL(mMessageServer, start());
    EXPECT_CALL(mMessageServer, sendNotification(mEventReceiveId1, mVpdNotification));
    EXPECT_CALL(mEventReceiver, onEventReceive(mPid, mReadyVpdEvent));

    mVpdMessageServer.start();
    mEventReceived(mEventReceiveId1, mReadyVpdEvent, mPid);
    mVpdMessageServer.sendNotification(mPid, mVpdNotification);
}

TEST_F(VpdMessageServerTest, sendNotificationWithDoubleEventReceivedTest)
{
    EXPECT_CALL(mMessageServer, initialize(_, _));
    EXPECT_CALL(mMessageServer, setMessageHandler(_)).WillOnce(DoAll(SaveArg<0>(&mEventReceived)));
    EXPECT_CALL(mMessageServer, start());
    EXPECT_CALL(mEventReceiver, onEventReceive(mPid, mReadyVpdEvent)).Times(2);
    EXPECT_CALL(mMessageServer, sendNotification(mEventReceiveId1, mVpdNotification));

    mVpdMessageServer.start();
    mEventReceived(mEventReceiveId1, mReadyVpdEvent, mPid);
    mEventReceived(mEventReceiveId1, mReadyVpdEvent, mPid);
    mVpdMessageServer.sendNotification(mPid, mVpdNotification);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
