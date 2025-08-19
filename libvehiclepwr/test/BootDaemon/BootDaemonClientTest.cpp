/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "BootDaemonClient.h"

#include "MockIBootDaemonCallback.h"
#include "MockIClient.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using testing::NiceMock;

class BootDaemonClientTest : public testing::Test {
protected:
    BootDaemonClientTest()
        : mClientMock{std::make_unique<NiceMock<MockIClient>>()}
        , mClientMockPtr{mClientMock.get()}
        , mBootDaemonClient{std::move(mClientMock)}
    {
    }

    ~BootDaemonClientTest() { testing::Mock::VerifyAndClearExpectations(mClientMockPtr); }

    std::unique_ptr<NiceMock<MockIClient>> mClientMock;
    NiceMock<MockIClient>* mClientMockPtr;
    BootDaemonClient mBootDaemonClient;
};

/*
 * Verifies that the callback pointer is passed to the client implementation.
 */
TEST_F(BootDaemonClientTest, subscribeBootdHmiClient)
{
    MockIBootDaemonCallback bootDaemonCallback;

    EXPECT_CALL(*mClientMockPtr, setBootDaemonCallback(&bootDaemonCallback)).Times(1);

    // Action
    mBootDaemonClient.subscribeBootdHmiClient(&bootDaemonCallback);
}

/*
 * Verifies that the callback pointer is passed to the client implementation.
 */
TEST_F(BootDaemonClientTest, unsubscribe)
{
    EXPECT_CALL(*mClientMockPtr, setBootDaemonCallback(nullptr)).Times(1);

    // Action
    mBootDaemonClient.unsubscribe();
}

/* Verifies that the function handOffResponsibility() can be called from
   BootDaemonClient.
*/
TEST_F(BootDaemonClientTest, handOffResponsibility)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::handOffResponsibility)).Times(1);

    // Action
    mBootDaemonClient.handOffResponsibility();
}

/* Verifies that the function handOffResponsibilityStartupAnimationDisplayed() can be called from
   BootDaemonClient.
*/
TEST_F(BootDaemonClientTest, handOffResponsibilityStartupAnimationDisplayed)
{
    EXPECT_CALL(*mClientMockPtr,
                sendVpdEvent(common::EVpdEvent::handOffResponsibilityStartupAnimationDisplayed))
        .Times(1);

    // Action
    mBootDaemonClient.handOffResponsibilityStartupAnimationDisplayed();
}

/* Verifies that the function handOffResponsibilityStartupAnimationAndDiDisplayed() can be called
 * from BootDaemonClient.
 */
TEST_F(BootDaemonClientTest, handOffResponsibilityStartupAnimationAndDiDisplayed)
{
    EXPECT_CALL(
        *mClientMockPtr,
        sendVpdEvent(common::EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed))
        .Times(1);

    // Action
    mBootDaemonClient.handOffResponsibilityStartupAnimationAndDiDisplayed();
}

/* Verifies that the function startConnection() can be called from
   BootDaemonClient.
*/
TEST_F(BootDaemonClientTest, startConnection)
{
    EXPECT_CALL(*mClientMockPtr, startConnection()).Times(1);

    // Action
    mBootDaemonClient.startConnection();
}

/* Verifies that the SubscribeBootDaemon event can be called from
   BootDaemonClient.
*/
TEST_F(BootDaemonClientTest, onConnect)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(common::EVpdEvent::SubscribeBootDaemon)).Times(1);

    // Action
    mBootDaemonClient.onConnect();
}

/*
 * Dummy test, just to achieve high line coverage.
 */
TEST_F(BootDaemonClientTest, onDisconnect)
{
    // Action
    mBootDaemonClient.onDisconnect();
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
