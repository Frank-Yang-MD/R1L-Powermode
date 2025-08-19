/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "NativeClient.h"

#include "MockIClient.h"
#include "MockINativeCallback.h"

using namespace std;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using testing::NiceMock;
using testing::StrictMock;

class NativeClientTest : public testing::Test {
protected:
    NativeClientTest()
        : mClientMock{std::make_unique<NiceMock<MockIClient>>()}
        , mClientMockPtr{mClientMock.get()}
        , mNativeClient{std::move(mClientMock)}
    {
    }

    ~NativeClientTest() { testing::Mock::VerifyAndClearExpectations(mClientMockPtr); }

    std::unique_ptr<NiceMock<MockIClient>> mClientMock;
    NiceMock<MockIClient>* mClientMockPtr;
    NativeClient mNativeClient;

    StrictMock<MockINativeCallback> mMockNativeCallback;
};

/* Verifies that the function sendVpdEvent() of the interface IClient
   can be called from NativeClient.
*/
TEST_F(NativeClientTest, sendVpdEventCall)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(EVpdEvent::StopCompleteNative)).Times(1);

    // Action
    mNativeClient.stopProcessingComplete();
}

/* Verifies that the function startConnection() can be called from
   NativeClient.
*/
TEST_F(NativeClientTest, startConnectionCall)
{
    EXPECT_CALL(*mClientMockPtr, startConnection()).Times(1);

    // Action
    mNativeClient.startConnection();
}

/* Verifies that the function sendVpdEvent(common::EVpdEvent::SubscribeNative)
   of the interface IClient can be called from NativeClient,
   if NativeClient connected.
*/
TEST_F(NativeClientTest, sendVpdEventCallSubscribeNativeConnected)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(EVpdEvent::SubscribeNative)).Times(1);

    mNativeClient.onConnect();
}

/*
 * Verifies that NativeClient implementation returns value from IClient member.
 */
TEST_F(NativeClientTest, GetVPDState)
{
    const auto expectedValue = EVpdState::APP_RESUME;

    EXPECT_CALL(*mClientMockPtr, getVpdState()).WillOnce(testing::Return(expectedValue));

    EXPECT_EQ(expectedValue, mNativeClient.GetVPDState());
}

/*
 * Dummy test to cover onDisconenct function.
 */
TEST_F(NativeClientTest, onDisconnect) { mNativeClient.onDisconnect(); }

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
