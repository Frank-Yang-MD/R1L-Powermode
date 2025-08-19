/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "EVpdNotifications.h"
#include "VpdIntegrationTest.h"

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::impl;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

class SuspendTest : public VpdIntegrationTest {
public:
    virtual void SetUp() { startDaemon(); }

    virtual void TearDown() { stopDaemon(); }
};

// Reference: Figure 6.1-26 State transition event to Apps/Services (Shutdown/Reboot/Suspend)
TEST_F(SuspendTest, REF_27236_NormalSuspend)
{
    RecordProperty("linksToDb", "REF-655,REF-656,REF-667,REF-748,REF-754,REF-22740,REF-27228");
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // No timeouts
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

// Reference: Figure 6.1-27 State transition event to Apps/Services (Shutdown/Reboot/Suspend)
TEST_F(SuspendTest, REF_27228_CheckIfWdtrAndAdtrRequestAreSentProperlyDuringSuspend)
{
    coldBoot();

    // wait for at least once WDTR request is sent
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    // V-CPU sends shutdown request to VPD
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);

    // After Shutdown request is received, wait for at least once ADTR request is sent
    ASSERT_TRUE(waitForCpuComMessage(kAdtrRequest));

    stopAppServices(false);
    stopEfwServices(false, false, false);
    stopNativeService(false);
    stopLogdogDaemon(false);
    unmountUsbDevices(false);
    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));
    mCpuComDeliveryStatusListener(true);

    // check if VPD stops sending ADTR after shutdown notification has been sent
    this_thread::sleep_for(kAbnormalPeriod * 10);
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(0);

    ASSERT_TRUE(waitForSuspend());
}

// Reference: https://600.sandamelco.jp/JIRA2/browse/CACAQ-21303
TEST_F(SuspendTest, REF_22740_IfEmergencyColdbootPropertyIsSetVpdSendsSelfReboot)
{
    defineProperty("vendor.emerg.coldboot", "BOOTD");

    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);
    }
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
}

TEST_F(SuspendTest, REF_27231_ShutdownRequestReceivedBeforeTransitionToNormalOperation)
{
    startCpuCom();
    startNativeDaemons();
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);

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

    {
        testing::InSequence seq;
        EXPECT_CALL(mAndroidInit, propertySet("vendor.melco.boot_completed", "1")).Times(1);

        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);

        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(0);
    EXPECT_CALL(mAndroidInit, propertySet("vendor.vpd_started", _)).Times(0);

    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::VpsStarted);

    // No timeouts
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

// Figure 6.1-27 State transition event to Apps/Services (Shutdown/Reboot/Suspend(Timeout))
TEST_F(SuspendTest, REF_27236_SuspendAppStopCompleteTimeout)
{
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::TimeOutError))
            .Times(1);
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AtLeast(1));
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // AppStopComplete timeout from APP services
    suspend(true, false, false, false, false, false, true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

// Figure 6.1-27 State transition event to Apps/Services (Shutdown/Reboot/Suspend(Timeout))
TEST_F(SuspendTest, REF_27236_SuspendEfwStopCompleteTimeout)
{
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::TimeOutError))
            .Times(1);
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AtLeast(1));
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // AppStopComplete timeout from EFW services
    suspend(false, true, false, false, false, false, true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

// Figure 6.1-27 State transition event to Apps/Services (Shutdown/Reboot/Suspend(Timeout))
TEST_F(SuspendTest, REF_27236_SuspendAppAndEfwStopCompleteTimeout)
{
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::TimeOutError))
            .Times(1);
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::TimeOutError))
            .Times(1);
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AtLeast(1));
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // AppStopComplete timeout from both APP and EFW services
    suspend(true, true, false, false, false, false, true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

// Figure 6.1-27 State transition event to Apps/Services (Shutdown/Reboot/Suspend(Timeout))
TEST_F(SuspendTest, REF_27236_SuspendNativeServiceStopCompleteTimeout)
{
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AtLeast(1));
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // AppStopComplete timeout from Native services
    suspend(false, false, false, true, false, false, true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

// Figure 6.1-27 State transition event to Apps/Services (Shutdown/Reboot/Suspend(Timeout))
TEST_F(SuspendTest, REF_27236_SuspendLogdogDaemonStopCompleteTimeout)
{
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AtLeast(1));
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // AppStopComplete timeout from Logdog daemon
    suspend(false, false, false, false, true, false, true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

// Figure 6.1-27 State transition event to Apps/Services (Shutdown/Reboot/Suspend(Timeout))
TEST_F(SuspendTest, REF_27236_SuspendLogdogLogsaveTimeout)
{
    ON_CALL(mLogDaemonActions, isLogSavingComplete()).WillByDefault(testing::Return(false));

    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AtLeast(1));
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    suspend(false, false, false, false, false, false, true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

// Figure 6.1-27 State transition event to Apps/Services (Shutdown/Reboot/Suspend(Timeout))
TEST_F(SuspendTest, REF_27236_SuspendUnmountTimeout)
{
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::TimeOutError))
            .Times(1);
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AtLeast(1));
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // Unmount timeout from Native services
    suspend(false, false, false, false, false, true, true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

// Figure 6.1-28 State transition event to Apps/Services (Shutdown/Reboot/Suspend (Service helth
// error))
TEST_F(SuspendTest, REF_27236_SuspendVpsStopProcessingFailed)
{
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded()).Times(0);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }

    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());

    // VPS stop processing failed
    suspend(false, false, true, false, false, false, true);

    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

TEST_F(SuspendTest, REF_27528_SuspendDoesNotSkipModulesUnloadedAndModulesAreLoaded)
{
    // IKernelModulesControl::skipModulesUnloadedCheck() always returns false.
    ON_CALL(mKernelModulesControl, skipModulesUnloadedCheck())
        .WillByDefault(testing::Return(false));

    // IKernelModulesControl::isModulesLoaded() returns true.
    ON_CALL(mKernelModulesControl, isModulesLoaded()).WillByDefault(testing::Return(true));

    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // No timeouts
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

TEST_F(SuspendTest, REF_27528_SuspendVpdWaitsForLoadingModulesBeforUnload)
{
    // IKernelModulesControl::skipModulesUnloadedCheck() always returns false.
    ON_CALL(mKernelModulesControl, skipModulesUnloadedCheck())
        .WillByDefault(testing::Return(false));

    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);

        // VPD waits for modules to be loaded
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded())
            .Times(4)
            .WillOnce(testing::Return(false))
            .WillOnce(testing::Return(false))
            .WillOnce(testing::Return(false))
            .WillRepeatedly(testing::Return(true));

        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // No timeouts
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

TEST_F(SuspendTest, REF_27528_SuspendAfterLoadingModulesTimeoutOccursVpdStartsUnloadModules)
{
    // IKernelModulesControl::skipModulesUnloadedCheck() always returns false.
    ON_CALL(mKernelModulesControl, skipModulesUnloadedCheck())
        .WillByDefault(testing::Return(false));

    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);

        // Simulate kernel modules loading timeout
        EXPECT_CALL(mKernelModulesControl, isModulesLoaded())
            .WillRepeatedly(testing::Return(false));

        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);
        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // No timeouts
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

TEST_F(SuspendTest, REF_27528_SuspendAfterUnloadingModulesTimeoutVpdSendsShutdownProcessingStart)
{
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    {
        testing::InSequence seq;
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer, sendNotification(kVpsSessionId, EVpdNotifications::FwStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mLogDaemonActions, isLogSavingComplete()).Times(testing::AtLeast(1));
        EXPECT_CALL(mMessageServer, sendNotification(kLogdogSessionId, EVpdNotifications::AppStop))
            .Times(1);
        EXPECT_CALL(mMessageServer,
                    sendNotification(kVpsSessionId, EVpdNotifications::RequestUnmount))
            .Times(1);
        EXPECT_CALL(mKernelModulesControl, skipModulesUnloadedCheck()).Times(1);
        EXPECT_CALL(mKernelModulesControl, unloadModules()).Times(1);

        // Simulate kernel modules unloading timeout
        EXPECT_CALL(mKernelModulesControl, isModulesUnloaded())
            .WillRepeatedly(testing::Return(false));

        EXPECT_CALL(*mCpuCom, send(kShutdownStartNotification, _, _)).Times(1);
        EXPECT_CALL(mWakeLockManager, releaseWakeLock()).Times(1);
        EXPECT_CALL(mWakeLockManager, disableWakeLocks()).Times(1);
        EXPECT_CALL(mVpdBridgeDaemonAdapter, suspend()).Times(1);

        // VPD sends "Start complete notification" after wakes up
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
    }
    EXPECT_CALL(*mCpuCom, send(kAdtrRequest, _)).Times(testing::AnyNumber());
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);
    EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(0);

    // No timeouts
    suspend(false, false, false, false, false, false, true);
    ASSERT_TRUE(waitForSuspend());
    wakeUp();
}

TEST_F(SuspendTest, REF_27740_HandOffResponibilityIsProperlyHandledOnEverySuspendPhase)
{
    coldBoot(true);
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    // handle HandOffResponsibility in NormalOperation
    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    // Send shutdown request
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    // VPS sends stopCompleteApp
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::AppStopCompleteVps);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    // VPS sends stopCompleteEfw
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::FwStopCompleteVps);
    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);
    ASSERT_TRUE(
        waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestUnmount));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    // VPS sends Unmount complete
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::UnmountComplete);
    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    // CpuCom sends delivery status
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
}

TEST_F(
    SuspendTest,
    REF_27740_HandOffResponibilityStartupAnimationAndDiDisplayedIsProperlyHandledOnEverySuspendPhase)
{
    coldBoot(true);
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    // handle handOffResponsibilityStartupAnimationAndDiDisplayed in NormalOperation
    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    // Send shutdown request
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    // VPS sends stopCompleteApp
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::AppStopCompleteVps);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    // VPS sends stopCompleteEfw
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::FwStopCompleteVps);
    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);
    ASSERT_TRUE(
        waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestUnmount));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    // VPS sends Unmount complete
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::UnmountComplete);
    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    // CpuCom sends delivery status
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
}

TEST_F(SuspendTest,
       REF_27740_HandOffResponibilityStartupAnimationDisplayedIsProperlyHandledOnEverySuspendPhase)
{
    coldBoot(true);
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    // handle HandOffResponibilityStartupAnimationDisplayed in NormalOperation
    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    // Send shutdown request
    sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    // VPS sends stopCompleteApp
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::AppStopCompleteVps);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    // VPS sends stopCompleteEfw
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::FwStopCompleteVps);
    ASSERT_TRUE(
        waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    // Native services send stopProcessingComplete
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteNative);
    ASSERT_TRUE(waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    // Logdog sends stopProcessingComplete
    sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);
    ASSERT_TRUE(
        waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestUnmount));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    // VPS sends Unmount complete
    sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::UnmountComplete);
    ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    // CpuCom sends delivery status
    mCpuComDeliveryStatusListener(true);

    ASSERT_TRUE(waitForSuspend());
}
