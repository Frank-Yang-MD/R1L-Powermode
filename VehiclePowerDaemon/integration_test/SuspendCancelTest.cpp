/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "EVpdNotifications.h"
#include "VpdIntegrationTest.h"

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::impl;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

class SuspendCancelTest : public VpdIntegrationTest {
public:
    virtual void SetUp()
    {
        startDaemon();
        coldBoot();
        ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
    }

    virtual void TearDown() { stopDaemon(); }
};

// Figure 6.1-31 State transition event to Apps/Services (Shutdown/Reboot/Suspend Cancel
TEST_F(SuspendCancelTest, REF_27584_SuspendCancelRequestReceived)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kShutdownCancelAcceptResponse, _)).Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kLogdogSessionId, EVpdNotifications::AppRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppRestart))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kShutdownCancelCompleteResponse, _)).Times(1);
        EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(testing::AtLeast(1));
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());
    // No self reboot request
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);

    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);

    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    stopEfwServices(false, false, false);
    stopNativeService(false);
    stopLogdogDaemon(false);

    // notify VPS about FwRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwRestartCompleteVps);

    // notify VPS about AppRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppRestartCompleteVps);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownCancelAcceptResponse));
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(SuspendCancelTest, REF_29851_ShutdownRequestReceivedOnSuspendCancelHandling)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kShutdownCancelAcceptResponse, _)).Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kLogdogSessionId, EVpdNotifications::AppRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppRestart))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kShutdownCancelCompleteResponse, _)).Times(1);

        // ensure that new shutdown sequence started
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());
    // No self reboot request
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);

    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);

    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    stopEfwServices(false, false, false);
    stopNativeService(false);
    stopLogdogDaemon(false);

    // notify VPS about FwRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwRestartCompleteVps);

    // request shutdown before restart sequence is over
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);

    // notify VPS about AppRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppRestartCompleteVps);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownCancelAcceptResponse));

    // wait until new shutdown sequence is started
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppStop));
}

TEST_F(SuspendCancelTest, REF_27585_SuspendCancelLateRequestReceived)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);

        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(1);

        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kShutdownCancelAcceptResponse, _)).Times(1);

        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesUnloaded()).Times(1);
        EXPECT_CALL(mKernelModulesControl, loadModules()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestMount))
            .Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kLogdogSessionId, EVpdNotifications::AppRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppRestart))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kShutdownCancelCompleteResponse, _)).Times(1);
        EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(testing::AtLeast(1));
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());
    // No self reboot request
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);

    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);
    stopLogdogDaemon(false);

    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);
    ASSERT_TRUE(waitForCpuComMessage(kShutdownCancelAcceptResponse));

    unmountUsbDevices(false);

    // VPD requests mounting USB devices
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestMount));

    // notify EIK about AppRestart
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    // notify VPS about FwRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwRestartCompleteVps);

    // notify VPS about AppRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppRestartCompleteVps);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(SuspendCancelTest, REF_27586_SuspendCancelReceivedDuringModulesUnload)
{
    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);

        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(1);

        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);

        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);

        EXPECT_CALL(*mCpuCom, send(kShutdownCancelAcceptResponse, _)).Times(1);

        EXPECT_CALL(mKernelModulesControl, isModulesUnloaded()).Times(1);
        EXPECT_CALL(mKernelModulesControl, loadModules()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestMount))
            .Times(1);

        EXPECT_CALL(mMessageServer,
                    sendNotification(kLogdogSessionId, EVpdNotifications::AppRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwRestart))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppRestart))
            .Times(1);

        EXPECT_CALL(*mCpuCom, send(kShutdownCancelCompleteResponse, _)).Times(1);
        EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(testing::AtLeast(1));
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());
    // No self reboot request
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);

    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);
    stopLogdogDaemon(false);
    unmountUsbDevices(false);

    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);
    ASSERT_TRUE(waitForCpuComMessage(kShutdownCancelAcceptResponse));

    // VPD requests mounting USB devices
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestMount));

    // notify EIK about AppRestart
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    // notify VPS about FwRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwRestartCompleteVps);

    // notify VPS about AppRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppRestartCompleteVps);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(SuspendCancelTest, REF_27740_HandOffResposibilityIsPrpperlyHandledDuringSuspendCancel)
{
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    // VPD requests VPS to notify APP_STOP to EFW servicesV
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::FwStopCompleteVps);

    // VPD notifies APP_STOP to Native services
    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);

    // VPD notifies APP_STOP to LogDog daemon
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    // notify VPS about FwRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwRestartCompleteVps);

    // notify VPS about AppRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppRestartCompleteVps);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownCancelAcceptResponse));
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));
}

TEST_F(
    SuspendCancelTest,
    REF_27740_HandOffResposibilityStartupAnimationAndDiDisplayedIsPrpperlyHandledDuringSuspendCancel)
{
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    // VPD requests VPS to notify APP_STOP to EFW servicesV
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::FwStopCompleteVps);

    // VPD notifies APP_STOP to Native services
    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);

    // VPD notifies APP_STOP to LogDog daemon
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    // notify VPS about FwRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwRestartCompleteVps);

    // notify VPS about AppRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppRestartCompleteVps);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownCancelAcceptResponse));
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));
}

TEST_F(SuspendCancelTest,
       REF_27740_HandOffResposibilityStartupAnimationDisplayedIsPrpperlyHandledDuringSuspendCancel)
{
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    // VPD requests VPS to notify APP_STOP to EFW servicesV
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::FwStopCompleteVps);

    // VPD notifies APP_STOP to Native services
    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);

    // VPD notifies APP_STOP to LogDog daemon
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    // notify VPS about FwRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwRestartCompleteVps);

    // notify VPS about AppRestart
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppRestartCompleteVps);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownCancelAcceptResponse));
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));
}
