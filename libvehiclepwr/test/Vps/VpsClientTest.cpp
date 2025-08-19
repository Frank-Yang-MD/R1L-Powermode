/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "MockIClient.h"
#include "MockIVpsCallback.h"

#include "VpsClient.h"

using namespace std;

using ::testing::NiceMock;
using ::testing::StrictMock;

using com::mitsubishielectric::ahu::vehiclepwrmgr::common::EVpdEvent;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class VpsClientTest : public testing::Test {
protected:
    VpsClientTest()
        : mClientMock{std::make_unique<NiceMock<MockIClient>>()}
        , mClientMockPtr{mClientMock.get()}
        , mVpsClient{std::move(mClientMock)}
    {
    }

    ~VpsClientTest() { testing::Mock::VerifyAndClearExpectations(mClientMockPtr); }

    std::unique_ptr<NiceMock<MockIClient>> mClientMock;
    NiceMock<MockIClient>* mClientMockPtr;
    VpsClient mVpsClient;

    StrictMock<MockIVpsCallback> mMockVpsCallback;
};

/* Verifies that the function sendVpdEvent() can be called from
   VpsClient with parameter EVpdEvent::SubscribeVps.
*/
TEST_F(VpsClientTest, sendVpdEventCallSubscribeVps)
{
    EXPECT_CALL(*mClientMockPtr, setVpsCallback(&mMockVpsCallback)).Times(1);

    // Action
    mVpsClient.subscribeVps(&mMockVpsCallback);
}

/* Verifies that the function sendVpdEvent() can be called once from
   VpsClient with parameter EVpdEvent::SubscribeVps.
*/
TEST_F(VpsClientTest, sendVpdEventCallSubscribeVpsOnce)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::SubscribeNative)).Times(1);
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::SubscribeVps)).Times(1);

    // Action
    mVpsClient.onConnect();
}

/* Verifies that the function sendVpdEvent() can be called from
   VpsClient with parameter EVpdEvent::VpsStarted.
*/
TEST_F(VpsClientTest, sendVpdEventCallVpsStarted)
{
    // Arrangements
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::VpsStarted)).Times(1);

    // Action
    mVpsClient.startProcessingComplete();
}

/* Verifies that the function sendVpdEvent() can be called from
   VpsClient with parameter EVpdEvent::AppStopCompleteVps.
*/
TEST_F(VpsClientTest, sendVpdEventCallAppStopCompleteVps)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::AppStopCompleteVps)).Times(1);

    // Action
    mVpsClient.appStopProcessingComplete();
}

/* Verifies that the function sendVpdEvent() can be called from
   VpsClient with parameter EVpdEvent::FwStopCompleteVps.
*/
TEST_F(VpsClientTest, sendVpdEventCallFwStopCompleteVps)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::FwStopCompleteVps)).Times(1);

    // Action
    mVpsClient.fwStopProcessingComplete();
}

/* Verifies that the function sendVpdEvent() can be called from
   VpsClient with parameter EVpdEvent::VpsReady.
*/
TEST_F(VpsClientTest, sendVpdEventCallVpsReady)
{
    // Arrangements
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::VpsReady)).Times(1);

    // Action
    mVpsClient.vpsReady();
}

/* Verifies that the function sendVpdEvent() can be called from
   VpsClient with parameter EVpdEvent::UnsubscribeVps if the subscription was called.
*/
TEST_F(VpsClientTest, sendVpdEventCallUnsubscribeVps)
{
    EXPECT_CALL(*mClientMockPtr, setVpsCallback(nullptr)).Times(1);
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::UnsubscribeVps)).Times(1);

    // Action
    mVpsClient.unsubscribe();
}

TEST_F(VpsClientTest, stopProcessingFailed_method_must_send_VPD_event_StopFailedVps)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(EVpdEvent::StopFailedVps)).Times(1);

    mVpsClient.stopProcessingFailed();
}

TEST_F(VpsClientTest, startProcessingFailed_method_must_send_VPD_event_StartFailedVps)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(EVpdEvent::StartFailedVps)).Times(1);

    mVpsClient.startProcessingFailed();
}

/*
 * Verifies that the function sendVpdEvent() can be called from
 * VpsClient with parameter EVpdEvent::FwRestartCompleteVps.
 */
TEST_F(VpsClientTest, sendVpdEventCallFwRestartCompleteVps)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::FwRestartCompleteVps)).Times(1);

    // Action
    mVpsClient.fwRestartProcessingComplete();
}

/*
 * Verifies that the function sendVpdEvent() can be called from
 * VpsClient with parameter EVpdEvent::FwResumeCompleteVps.
 */
TEST_F(VpsClientTest, sendVpdEventCallFwResumeCompleteVps)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::FwResumeCompleteVps)).Times(1);

    // Action
    mVpsClient.fwResumeProcessingComplete();
}

/*
 * Verifies that the function sendVpdEvent() can be called from
 * VpsClient with parameter EVpdEvent::appRestartCompleteVps.
 */
TEST_F(VpsClientTest, sendVpdEventCallAppRestartCompleteVps)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::AppRestartCompleteVps)).Times(1);

    // Action
    mVpsClient.appRestartProcessingComplete();
}

/*
 * Verifies that the function sendVpdEvent() can be called from
 * VpsClient with parameter EVpdEvent::appResumeCompleteVps.
 */
TEST_F(VpsClientTest, sendVpdEventCallAppResumeCompleteVps)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::AppResumeCompleteVps)).Times(1);

    // Action
    mVpsClient.appResumeProcessingComplete();
}

TEST_F(VpsClientTest, unmountCompleteCheckForEventSend)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::UnmountComplete)).Times(1);

    // Action
    mVpsClient.unmountComplete();
}

TEST_F(VpsClientTest, endEarlyHmiCheckForEventSend)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::EndEarlyHmi)).Times(1);

    // Action
    mVpsClient.endEarlyHmi();
}

TEST_F(VpsClientTest, forcedCompletionEarlyServicesCheckForEventSend)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::ForcedCompletionEarlyServices))
        .Times(1);

    // Action
    mVpsClient.forcedCompletionEarlyServices();
}

/*
 * Dummy test, just to achieve high line coverage.
 */
TEST_F(VpsClientTest, onDisconnect)
{
    // Action
    mVpsClient.onDisconnect();
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
