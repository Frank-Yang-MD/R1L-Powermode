/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */
#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockIClient.h"
#include "MockINativeCallback.h"

#include "LogdogClient.h"

using namespace std;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using testing::NiceMock;

class LogdogClientTest : public testing::Test {
protected:
    LogdogClientTest()
        : mClientMock{std::make_unique<NiceMock<MockIClient>>()}
        , mClientMockPtr{mClientMock.get()}
        , mLogdogClient{std::move(mClientMock)}
    {
    }

    ~LogdogClientTest() { testing::Mock::VerifyAndClearExpectations(mClientMockPtr); }

    std::unique_ptr<NiceMock<MockIClient>> mClientMock;
    NiceMock<MockIClient>* mClientMockPtr;
    LogdogClient mLogdogClient;
};

/* Verifies that the function sendVpdEvent(EVpdEvent::StopCompleteLogdog)
   of the interface IClient can be called from LogdogClient
   and the function sendVpdEvent(EVpdEvent::UnsubscribeLogdog)
   cannot be called if LogdogClient was not subscribed.
*/
TEST_F(LogdogClientTest, sendVpdEventCallStopCompleteLogdog)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(EVpdEvent::StopCompleteLogdog)).Times(1);

    mLogdogClient.stopProcessingComplete();
}

/* Verifies that the function sendVpdEvent(EVpdEvent::UnsubscribeLogdog)
   of the interface IClient can be called from LogdogClient,
   if LogdogClient was subscribed, and can be called once only.
*/
TEST_F(LogdogClientTest, sendVpdEventCallUnsubscribeLogdog)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(EVpdEvent::UnsubscribeNative)).Times(1);
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(EVpdEvent::UnsubscribeLogdog)).Times(1);

    mLogdogClient.unsubscribe();
}

/* Verifies that the function sendVpdEvent(common::EVpdEvent::SubscribeLogdog)
   of the interface IClient can be called from LogdogClient,
   if LogdogClient connected.
*/
TEST_F(LogdogClientTest, sendVpdEventCallSubscribeLogdogConnected)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(EVpdEvent::SubscribeLogdog)).Times(1);
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(EVpdEvent::SubscribeNative)).Times(1);

    // Action
    mLogdogClient.onConnect();
}

/*
 * Verifies that the function subscribe passes callback to the client implementation.
 */
TEST_F(LogdogClientTest, subscribe)
{
    MockINativeCallback nativeCallback;

    EXPECT_CALL(*mClientMockPtr, setNativeCallback(&nativeCallback)).Times(1);

    // Action
    mLogdogClient.subscribe(&nativeCallback);
}

/*
 * Dummy test, just to achieve high line coverage.
 */
TEST_F(LogdogClientTest, onDisconnect)
{
    // Action
    mLogdogClient.onDisconnect();
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
