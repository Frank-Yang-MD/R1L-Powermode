/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "EVpdNotifications.h"
#include "VpdIntegrationTest.h"

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::impl;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

class ResumeTest : public VpdIntegrationTest {
public:
    virtual void SetUp()
    {
        startDaemon();
        coldBoot();
    }
    virtual void TearDown() { stopDaemon(); }
};

// Figure 6.1-29 State transition event to Apps/Services (Resume)
TEST_F(ResumeTest, REF_27548_NormalResume)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(mWakeLockManager, enableWakeLocks()).Times(1);
        EXPECT_CALL(mWakeLockManager, acquireWakeLock()).Times(1);
    }

    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

    {
        testing::InSequence seq;
        EXPECT_CALL(mKernelModulesControl, loadModules()).Times(1);

        EXPECT_CALL(mWakeUpManager, wakeUp()).Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kLogdogSessionId, EVpdNotifications::AppResume))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppResume))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwResume))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppResume))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(1);

        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpd_started", "1")).Times(1);

        EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(1);
    }
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwResumeCompleteVps);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppResumeCompleteVps);

    ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));
    ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand));
}

// Figure 6.1-29 State transition event to Apps/Services (Resume)
TEST_F(ResumeTest, REF_27548_VpdDoesntContinueResumeUntilReceivingSucccessResultOfSentComplete)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(mWakeLockManager, enableWakeLocks()).Times(1);
        EXPECT_CALL(mWakeLockManager, acquireWakeLock()).Times(1);
    }

    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

    mCpuComDeliveryStatusListener(false);
    EXPECT_CALL(mKernelModulesControl, loadModules()).Times(0);
    EXPECT_CALL(mWakeUpManager, wakeUp()).Times(0);
    EXPECT_CALL(mMessageServer, sendNotification(_, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);
}

// Figure 6.1-30 State transition event to Apps/Services (Resume(Timeout))
TEST_F(ResumeTest, REF_27548_ResumeFwResumeCompleteTimeout)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(mWakeLockManager, enableWakeLocks()).Times(1);
        EXPECT_CALL(mWakeLockManager, acquireWakeLock()).Times(1);
    }

    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

    {
        testing::InSequence seq;
        EXPECT_CALL(mKernelModulesControl, loadModules()).Times(1);

        EXPECT_CALL(mWakeUpManager, wakeUp()).Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kLogdogSessionId, EVpdNotifications::AppResume))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppResume))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwResume))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::TimeOutError))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);

        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppResume))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(1);

        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpd_started", "1")).Times(1);
    }
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(testing::AtLeast(1));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppResumeCompleteVps);

    ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));
    ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));
}

// Figure 6.1-30 State transition event to Apps/Services (Resume(Timeout))
TEST_F(ResumeTest, REF_27548_ResumeAppResumeCompleteTimeout)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(mWakeLockManager, enableWakeLocks()).Times(1);
        EXPECT_CALL(mWakeLockManager, acquireWakeLock()).Times(1);
    }

    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

    {
        testing::InSequence seq;
        EXPECT_CALL(mKernelModulesControl, loadModules()).Times(1);

        EXPECT_CALL(mWakeUpManager, wakeUp()).Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kLogdogSessionId, EVpdNotifications::AppResume))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppResume))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwResume))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppResume))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::TimeOutError))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);

        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpd_started", "1")).Times(1);
    }
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(testing::AtLeast(1));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwResumeCompleteVps);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume));

    ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));
    ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));
}

// Request for R244.
// Reference: CACAQ-20834
// System requirements: Android_CmnPFSystemDesign_v3.10_resume2.pdf
// Figure 6.1-29 State transition event to Apps/Services (Resume)
TEST_F(ResumeTest, REF_27548_SuspendRequestIsReceivedDuringResume)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(mWakeLockManager, enableWakeLocks()).Times(1);
        EXPECT_CALL(mWakeLockManager, acquireWakeLock()).Times(1);
    }

    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

    {
        testing::InSequence seq;
        EXPECT_CALL(mKernelModulesControl, loadModules()).Times(1);

        EXPECT_CALL(mWakeUpManager, wakeUp()).Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kLogdogSessionId, EVpdNotifications::AppResume))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppResume))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwResume))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppResume))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(1);

        // Start suspend request processing
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
    }

    EXPECT_CALL(mAndroidInit, propertySet("vendor.vpd_started", "1")).Times(0);
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwResumeCompleteVps);

    // simulate suspend request
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppResumeCompleteVps);

    ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));
    waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppStop);
}

// Request for R244.
// Reference: CACAQ-20834
// System requirements: Android_CmnPFSystemDesign_v3.10_resume2.pdf
// Figure 6.1-29 State transition event to Apps/Services (Resume)
TEST_F(ResumeTest, REF_27548_SuspendRequestAndSuspendCancelRequestIsReceivedDuringResume)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(mWakeLockManager, enableWakeLocks()).Times(1);
        EXPECT_CALL(mWakeLockManager, acquireWakeLock()).Times(1);
    }

    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

    {
        testing::InSequence seq;
        EXPECT_CALL(mKernelModulesControl, loadModules()).Times(1);

        EXPECT_CALL(mWakeUpManager, wakeUp()).Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kLogdogSessionId, EVpdNotifications::AppResume))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppResume))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwResume))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kShutdownCancelAcceptResponse, _)).Times(1);

        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppResume))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(1);
        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpd_started", "1")).Times(1);
        EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(1);
    }
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));

    // Simulate suspend request
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    // And then cancel the suspend request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwResumeCompleteVps);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppResumeCompleteVps);

    ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));
    ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand));
}

TEST_F(ResumeTest, REF_27740_HandOffResposibilityIsProperyHandledDuringResume)
{
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());

    wakeUp();

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwResumeCompleteVps);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppResumeCompleteVps);

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));
}

TEST_F(ResumeTest,
       REF_27740_HandOffResposibilityStartupAnimationAndDiDisplayedIsProperyHandledDuringResume)
{
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());

    wakeUp();

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwResumeCompleteVps);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppResumeCompleteVps);

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));
}

TEST_F(ResumeTest,
       REF_27740_HandOffResposibilityStartupAnimationDisplayedIsProperyHandledDuringResume)
{
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());

    wakeUp();

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwResumeCompleteVps);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppResumeCompleteVps);

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));
}
