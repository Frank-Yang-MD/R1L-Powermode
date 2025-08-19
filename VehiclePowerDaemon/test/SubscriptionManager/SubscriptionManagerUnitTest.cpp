/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "MockIDisconnectNotification.h"
#include "MockSubscriptionNotification.h"
#include "SubscriptionManager.h"
#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::_;
const int SubscriptionManager::voidId;

class SubscriptionTest : public ::testing::Test {
public:
    SubscriptionTest()
        : m_SubscriptionManager{}
    {
        m_SubscriptionManager.setSubscriptionNotification(&m_notification);
        m_SubscriptionManager.setDisconnectNotification(&m_disconnectNotification);
    }
    void SetUp() {}
    void TearDown() {}

    MockSubscriptionNotification m_notification;
    MockIDisconnectNotification m_disconnectNotification;
    SubscriptionManager m_SubscriptionManager;
};

TEST_F(SubscriptionTest, AddGetRemoveClients)
{
    size_t initial_size = m_SubscriptionManager.getNativeSubscribers().size();
    EXPECT_CALL(m_notification, onSubscribeNative(_)).Times(10);
    for (int i = 0; i < 10; i++) {
        m_SubscriptionManager.add(i);
    }
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), (initial_size + 10));
    auto subscribers = m_SubscriptionManager.getNativeSubscribers();
    std::vector<int>::iterator it;
    int i = 0;
    for (it = subscribers.begin(); it != subscribers.end(); ++it, ++i) {
        EXPECT_EQ(*it, i);
    }
    for (int j = 9; j >= 0; j--) {
        m_SubscriptionManager.remove(j);
    }
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), initial_size);
}

TEST_F(SubscriptionTest, AddExistingClient)
{
    constexpr int clientId = 555;
    m_SubscriptionManager.add(clientId);
    size_t initial_size = m_SubscriptionManager.getNativeSubscribers().size();
    m_SubscriptionManager.add(clientId);
    EXPECT_EQ(initial_size, m_SubscriptionManager.getNativeSubscribers().size());
}

TEST_F(SubscriptionTest, AddWithNullNotification)
{
    constexpr int clientId = 555;
    m_SubscriptionManager.setSubscriptionNotification(nullptr);
    m_SubscriptionManager.add(clientId);
    EXPECT_CALL(m_notification, onSubscribeNative(_)).Times(0);
}

TEST_F(SubscriptionTest, AddVPSWithNullNotification)
{
    constexpr int vpsId = 555;
    m_SubscriptionManager.setSubscriptionNotification(nullptr);
    m_SubscriptionManager.setVpsID(vpsId);
    EXPECT_CALL(m_notification, onSubscribeVps(_)).Times(0);
}

TEST_F(SubscriptionTest, RemoveInvalid)
{
    constexpr int socketId = 0xFF;
    size_t initialSize = m_SubscriptionManager.getNativeSubscribers().size();
    EXPECT_CALL(m_notification, onSubscribeNative(_)).Times(1);
    m_SubscriptionManager.add(socketId);
    m_SubscriptionManager.remove(-1);
    m_SubscriptionManager.remove(0xFFA);
    m_SubscriptionManager.remove(0xFFFE);
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), initialSize + 1);
    m_SubscriptionManager.remove(socketId);
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), initialSize);
}

TEST_F(SubscriptionTest, GetSetVpsID)
{
    constexpr int vpsID_1 = 0xFF;
    constexpr int vpsID_2 = 0xFE;
    EXPECT_CALL(m_notification, onSubscribeVps(_)).Times(2);
    m_SubscriptionManager.setVpsID(vpsID_1);
    EXPECT_EQ(m_SubscriptionManager.getVpsID(), vpsID_1);
    m_SubscriptionManager.setVpsID(vpsID_2);
    EXPECT_EQ(m_SubscriptionManager.getVpsID(), vpsID_2);
}

TEST_F(SubscriptionTest, GetSetBootDaemonID)
{
    constexpr int bootID_1 = 444;
    constexpr int bootID_2 = 445;
    m_SubscriptionManager.setBootDaemonID(bootID_1);
    EXPECT_EQ(m_SubscriptionManager.getBootDaemonID(), bootID_1);
    m_SubscriptionManager.setBootDaemonID(bootID_2);
    EXPECT_EQ(m_SubscriptionManager.getBootDaemonID(), bootID_2);
}

TEST_F(SubscriptionTest, SetLogID)
{
    const int numberOfNativeSubscribersBefore = m_SubscriptionManager.getNativeSubscribers().size();
    constexpr int logID_1 = 444;
    constexpr int logID_2 = 445;
    m_SubscriptionManager.setLogID(logID_1);
    EXPECT_EQ(m_SubscriptionManager.getLogID(), logID_1);
    m_SubscriptionManager.setLogID(logID_2);
    EXPECT_EQ(m_SubscriptionManager.getLogID(), logID_2);
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), numberOfNativeSubscribersBefore);
}

TEST_F(SubscriptionTest, DisconnectLogID)
{
    const int numberOfNativeSubscribersBefore = m_SubscriptionManager.getNativeSubscribers().size();
    constexpr int logID = 444;
    m_SubscriptionManager.setLogID(logID);
    m_SubscriptionManager.disconnect(logID);
    EXPECT_EQ(m_SubscriptionManager.getLogID(), SubscriptionManager::voidId);
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), numberOfNativeSubscribersBefore);
}

TEST_F(SubscriptionTest, DisconnectVps)
{
    const int numberOfNativeSubscribersBefore = m_SubscriptionManager.getNativeSubscribers().size();
    constexpr int vpsID = 0xFF;
    size_t initial_size = m_SubscriptionManager.getNativeSubscribers().size();
    EXPECT_CALL(m_notification, onSubscribeNative(_)).Times(1);
    m_SubscriptionManager.add(vpsID);

    EXPECT_CALL(m_notification, onSubscribeVps(_)).Times(1);
    m_SubscriptionManager.setVpsID(vpsID);
    EXPECT_EQ(m_SubscriptionManager.getVpsID(), vpsID);

    m_SubscriptionManager.disconnect(vpsID);
    EXPECT_EQ(m_SubscriptionManager.getVpsID(), SubscriptionManager::voidId);
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), initial_size);
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), numberOfNativeSubscribersBefore);
}

TEST_F(SubscriptionTest, DisconnectBootDaemon)
{
    const int numberOfNativeSubscribersBefore = m_SubscriptionManager.getNativeSubscribers().size();
    constexpr int bootDaemonId = 0xFF;
    size_t initial_size = m_SubscriptionManager.getNativeSubscribers().size();
    EXPECT_CALL(m_notification, onSubscribeNative(_)).Times(1);
    m_SubscriptionManager.add(bootDaemonId);

    EXPECT_CALL(m_notification, onSubscribeVps(_)).Times(0);
    m_SubscriptionManager.setBootDaemonID(bootDaemonId);
    EXPECT_EQ(m_SubscriptionManager.getBootDaemonID(), bootDaemonId);

    m_SubscriptionManager.disconnect(bootDaemonId);
    EXPECT_EQ(m_SubscriptionManager.getBootDaemonID(), SubscriptionManager::voidId);
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), initial_size);
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), numberOfNativeSubscribersBefore);
}

TEST_F(SubscriptionTest, DisconnectNative)
{
    constexpr int vpsID = 0xFF;
    int numOfNativeSubscribers{10};
    size_t initial_size = m_SubscriptionManager.getNativeSubscribers().size();

    EXPECT_CALL(m_notification, onSubscribeNative(_)).Times(numOfNativeSubscribers);
    for (int i = 0; i < numOfNativeSubscribers; i++) {
        m_SubscriptionManager.add(i);
    }

    EXPECT_CALL(m_notification, onSubscribeNative(_)).Times(1);
    m_SubscriptionManager.add(vpsID);
    EXPECT_CALL(m_notification, onSubscribeVps(_)).Times(1);
    m_SubscriptionManager.setVpsID(vpsID);

    for (int i = 0; i < numOfNativeSubscribers; i++) {
        m_SubscriptionManager.disconnect(i);
    }

    EXPECT_EQ(m_SubscriptionManager.getVpsID(), vpsID);
    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), initial_size);
}

TEST_F(SubscriptionTest, SubscribeVpsDoesnotChangeNativeSubscribers)
{
    constexpr int vpsID = 0xFF;
    size_t initial_size = m_SubscriptionManager.getNativeSubscribers().size();
    m_SubscriptionManager.setVpsID(vpsID);

    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), initial_size);
}

TEST_F(SubscriptionTest, SubscribeLogdogDoesnotChangeNativeSubscribers)
{
    constexpr int LogdogID = 0xFF;
    size_t initial_size = m_SubscriptionManager.getNativeSubscribers().size();
    m_SubscriptionManager.setLogID(LogdogID);

    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), initial_size);
}

TEST_F(SubscriptionTest, SubscribeBootdaemonDoesnotChangeNativeSubscribers)
{
    constexpr int bootDaemonID = 0xFF;
    size_t initial_size = m_SubscriptionManager.getNativeSubscribers().size();
    m_SubscriptionManager.setBootDaemonID(bootDaemonID);

    EXPECT_EQ(m_SubscriptionManager.getNativeSubscribers().size(), initial_size);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
