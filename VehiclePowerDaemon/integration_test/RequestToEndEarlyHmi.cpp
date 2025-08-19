/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "EVpdNotifications.h"
#include "VpdIntegrationTest.h"

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::impl;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

class RequestToEndEarlyHmiTest : public VpdIntegrationTest {
public:
    virtual void SetUp() { startDaemon(); }

    virtual void TearDown() { stopDaemon(); }
};

TEST_F(RequestToEndEarlyHmiTest, REF_27787_Coldboot)
{
    startCpuCom();
    startNativeDaemons();

    // VPS subscribes to VPD
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::SubscribeVps);

    // Check if VPD notifies to VPS current power state (Coldboot)
    ASSERT_TRUE(
        waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStartColdboot));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    // VPS sends "HMI is ready" message to VPD
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::VpsReady);

    // VPD notifies to BootDaemon that HMI is ready
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       vpd_common::EVpdNotifications::HmiEventVpsReady));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    // VPS sends start completion message to VPD
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::VpsStarted);

    // VPD sends main start completion message to V-CPU
    ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));

    // VPD sets vendor.melco.boot_completed and vendor.vpd_started properties
    ASSERT_TRUE(waitForPropertySet("vendor.melco.boot_completed", "1"));
    ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    // BootDaemon sends "hand off responsibility with startup animation/DI" message to VPD
    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 vpd_common::EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);

    // VPD sends "hand off responsibility with startup animation/DI" message to VPS
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId,
        vpd_common::EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    // VPS sends "Request to end Early HMI"
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::EndEarlyHmi);

    // VPD sends "Request to end Early HMI" to BootDaemon
    ASSERT_TRUE(
        waitForVpdNotification(kBootDaemonSessionId, vpd_common::EVpdNotifications::EndEarlyHmi));
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));
}

TEST_F(RequestToEndEarlyHmiTest, REF_27787_Suspend)
{
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    // VPS sends APP stop complete to VPD
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::AppStopCompleteVps);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::FwStopCompleteVps);

    // VPD notifies APP_STOP to Native services
    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);

    // VPD notifies APP_STOP to LogDog daemon
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    // LogDog daemon sends stop complete to VPD
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    // VPD requests unmounting USB devices
    ASSERT_TRUE(
        waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestUnmount));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    // VPS sends unmount complete to VPD
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::UnmountComplete);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    mCpuComDeliveryStatusListener(true);

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    ASSERT_TRUE(waitForSuspend());
}

TEST_F(RequestToEndEarlyHmiTest, REF_27787_Resume)
{
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());

    wakeUp();

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwResumeCompleteVps);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::AppResumeCompleteVps);

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand));

    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::ForcedCompletionEarlyServices);
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       EVpdNotifications::ForcedCompletionEarlyServices));
}
