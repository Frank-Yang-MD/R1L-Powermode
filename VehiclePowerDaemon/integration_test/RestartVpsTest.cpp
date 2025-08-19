/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "EVpdNotifications.h"
#include "VpdIntegrationTest.h"

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::impl;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

class RestartVpsTest : public VpdIntegrationTest {
public:
    virtual void SetUp() { startDaemon(); }

    virtual void TearDown() { stopDaemon(); }

    inline void vpsConnected(bool isInternalStateAppStop)
    {
        // VPS subscribes to VPD
        sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::SubscribeNative);
        sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::SubscribeVps);

        // Check if VPD notifies to VPS current power state
        if (isInternalStateAppStop) {
            ASSERT_TRUE(
                waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStop));
        }
        else {
            ASSERT_TRUE(waitForVpdNotification(kVpsSessionId,
                                               vpd_common::EVpdNotifications::AppStartColdboot));
            sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::VpsStarted);
        }
    }

    inline void vpsStartCompleteReceivedExpectation()
    {
        ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));

        // VPD sets vendor.melco.boot_completed and vendor.vpd_started properties
        ASSERT_TRUE(waitForPropertySet("vendor.melco.boot_completed", "1"));
        ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));
    }

    inline void vpsStartCompletedNotReceivedExpectation()
    {
        // Check if VPD notifies to VPS current power state (Coldboot)
        ASSERT_TRUE(
            waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStartColdboot));

        // VPS sends "HMI is ready" message to VPD
        sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::VpsReady);

        // VPD notifies to BootDaemon that HMI is ready
        ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                           vpd_common::EVpdNotifications::HmiEventVpsReady));

        // VPS sends start completion message to VPD
        sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::VpsStarted);

        // VPD sends main start completion message to V-CPU
        ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));

        // VPD sets vendor.melco.boot_completed and vendor.vpd_started properties
        ASSERT_TRUE(waitForPropertySet("vendor.melco.boot_completed", "1"));
        ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));
    }
};

TEST_F(RestartVpsTest, REF_31370_VpsDisconnection_during_coldboot)
{
    startCpuCom();
    startNativeDaemons();

    // VPS subscribes to VPD
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::SubscribeVps);

    // Check if VPD notifies to VPS current power state (Coldboot)
    ASSERT_TRUE(
        waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStartColdboot));

    // VPS sends "HMI is ready" message to VPD
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::VpsReady);

    // VPD notifies to BootDaemon that HMI is ready
    ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                       vpd_common::EVpdNotifications::HmiEventVpsReady));

    // disconnect Vps on waiting for startComplete
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    // expect reconnection and coldboot completion
    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31370_VpsDisconnection_handling_in_Normal_Operation)
{
    coldBoot(true);
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    // disconnect VPS on NormalOperation
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    // expect waiting for Vps restart
    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

//*
// * TEST CASES FOR:
// * Vps disconnected DURING restart-resume sequence
// *
TEST_F(RestartVpsTest, REF_31371_VpsDisconnection_handling_during_PollingWakeUp)
{
    coldBoot();
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());

    // disconenct Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    wakeUp();
    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31371_VpsDisconnection_handling_during_WaitForFwResume)
{
    coldBoot();
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());

    wakeUp();
    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31371_VpsDisconnection_handling_during_WaitForAppResume)
{
    coldBoot();
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());

    wakeUp();
    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwResumeCompleteVps);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31371_VpsDisconnection_handling_onSuspendCancel)
{
    coldBoot();

    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);

    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    stopEfwServices(false, false, false);
    stopNativeService(false);

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    stopLogdogDaemon(false);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31371_VpsDisconnection_handling_during_WaitFwRestart)
{
    coldBoot();

    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);

    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    stopEfwServices(false, false, false);
    stopNativeService(false);

    stopLogdogDaemon(false);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart));
    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31371_VpsDisconnection_handling_during_WaitAppRestart)
{
    coldBoot();

    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);

    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    stopEfwServices(false, false, false);
    stopNativeService(false);

    stopLogdogDaemon(false);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart));
    sendVpdEvent(kVpsSessionId, kVpsId, EVpdEvent::FwRestartCompleteVps);

    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

//*
// * TEST CASES FOR:
// * Vps disconnected during shutdown sequence and NOT restarted before RESUME
// *
TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_on_waitForAppStopVps_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    // stopApp
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_on_waitForFwStopVps_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);

    // fwServices stop
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_on_waitNativeAppStopProcessing_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_after_NativeAppStopProcessing_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_during_StopLogdog_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));
    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_during_WaitForUnmount_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);
    stopLogdogDaemon(false);

    ASSERT_TRUE(
        waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestUnmount));
    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    // no unmount Complete as VPS is not connected

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

//*
// * TEST CASES FOR:
// * Vps disconnected during shutdown sequence and NOT restarted before RESTART
// *
TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_on_waitForAppStopVps_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    // stopApp
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_on_waitForFwStopVps_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);

    // fwServices stop
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_on_waitNativeAppStopProcessing_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_after_NativeAppStopProcessing_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_during_StopLogdog_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));
    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsDisconnected_during_WaitForUnmount_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);
    stopLogdogDaemon(false);

    ASSERT_TRUE(
        waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestUnmount));
    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    // no unmount Complete as VPS is not connected

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart,
                                        std::chrono::milliseconds(100)));

    startVps(true);

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

//*
// * TEST CASES FOR:
// * Vps disconnected AND RECONNECTED during shutdown sequence -> RESTART
// *
TEST_F(RestartVpsTest, REF_31368_VpsReconnected_on_waitForAppStopVps_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    // stopApp
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    vpsConnected(false);  // internal state is App_Start_Coldboot

    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart,
                                        std::chrono::milliseconds(100)));

    vpsStartCompleteReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsReconnected_on_waitForFwStopVps_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);

    // fwServices stop
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    vpsConnected(false);  // internal state is App_Start_Coldboot

    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwRestart,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppRestart,
                                        std::chrono::milliseconds(100)));

    vpsStartCompleteReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsReconnected_on_waitNativeAppStopProcessing_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    vpsConnected(true);  // internal state is App_Stop

    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    vpsStartCompletedNotReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsRestarted_after_NativeAppStopProcessing_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));
    vpsConnected(true);  // internal state is App_Stop

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    vpsStartCompletedNotReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsReconnected_during_WaitForUnmount_RestartSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);
    stopLogdogDaemon(false);

    ASSERT_TRUE(
        waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestUnmount));
    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    // cancel shutdown request
    sendCpuMessage(kShutdownCancelRequestCommand, kSuspendCancelRequestData);

    vpsConnected(true);  // internal state is App_Stop

    // no unmount Complete as VPS was not connected

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppRestart));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppRestart));

    vpsStartCompletedNotReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

//*
// * TEST CASES FOR:
// * Vps disconnected AND RECONNECTED during shutdown sequence -> RESUME
// *
TEST_F(RestartVpsTest, REF_31368_VpsReconnected_on_waitForAppStopVps_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    // stopApp
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    vpsConnected(false);  // internal state is App_start_coldboot

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume,
                                        std::chrono::milliseconds(100)));

    vpsStartCompleteReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsReconnected_on_waitForFwStopVps_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);

    // fwServices stop
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    vpsConnected(false);  // internal state is App_start_coldboot

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::FwResume,
                                        std::chrono::milliseconds(100)));
    ASSERT_FALSE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::AppResume,
                                        std::chrono::milliseconds(100)));

    vpsStartCompleteReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsReconnected_on_waitNativeAppStopProcessing_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);

    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    vpsConnected(true);  // internal state is App_stop

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    vpsStartCompletedNotReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsReconnected_after_NativeAppStopProcessing_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);

    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));
    vpsConnected(true);  // internal state is app_stop

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    vpsStartCompletedNotReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsReconnected_during_StopLogdog_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));
    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);

    vpsConnected(true);  // internal state is app_stop

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    vpsStartCompletedNotReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}

TEST_F(RestartVpsTest, REF_31368_VpsReconnected_during_WaitForUnmount_ResumeSequence)
{
    coldBoot();

    // suspend process start
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);
    stopLogdogDaemon(false);

    ASSERT_TRUE(
        waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestUnmount));
    // disconnect Vps
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::Disconnect);
    vpsConnected(true);  // internal state is app_stop

    // no unmount Complete as VPS was not connected

    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();

    ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, EVpdNotifications::AppResume));
    ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId, EVpdNotifications::AppResume));

    vpsStartCompletedNotReceivedExpectation();

    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
}
