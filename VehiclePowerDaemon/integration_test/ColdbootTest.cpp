/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpdIntegrationTest.h"

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::impl;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

class ColdbootTest : public VpdIntegrationTest {
public:
};

// Reference: Android PF system design. Figure 6.1-35 Module start sequence in Coldboot
TEST_F(ColdbootTest, REF_659_NormalColdBoot)
{
    RecordProperty("linksToDb", "REF-638,REF-662,REF-752,REF-753");

    {
        testing::InSequence s;

        // VPD starts wake up event detection
        EXPECT_CALL(mWakeUpEventDetector, start()).Times(1);

        // VPD acquires wakelock
        EXPECT_CALL(mWakeLockManager, acquireWakeLock()).Times(1);

        // VPD triggers init to start CpuComDaemon
        EXPECT_CALL(mAndroidInit, propertySet("vendor.cpucomdaemon", "start")).Times(1);

        // And then subscribes to it and starts listening for messages
        EXPECT_CALL(*mCpuCom, subscribe(testing::An<common::CpuCommand>(),
                                        testing::An<cpucom::ICpuCom::OnCommand>()))
            .Times(2);
        EXPECT_CALL(*mCpuCom, connect()).Times(1);

        // VPD triggers init to start NativeDaemons
        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpstart", "start")).Times(1);
        EXPECT_CALL(mAndroidInit, propertySet("vendor.svc_csxe", "start")).Times(1);

        // VPD sends start complete and main start complete to V-CPU
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
        EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(1);

        // VPD sets property to indicate that coldboot is complete
        EXPECT_CALL(mAndroidInit, propertySet("vendor.melco.boot_completed", "1")).Times(1);
        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpd_started", "1")).Times(1);

        // VPD starts to send WDT reset request to V-CPU
        EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(1);
    }

    startDaemon();
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
    stopDaemon();
}

TEST_F(ColdbootTest, REF_753_NormalColdBootConditionInConfigNotSatisfied)
{
    defineVehicleConfigValue("PROXI_SDARS_PRESENCE", (uint8_t)0);
    {
        testing::InSequence s;

        // VPD starts wake up event detection
        EXPECT_CALL(mWakeUpEventDetector, start()).Times(1);

        // VPD acquires wakelock
        EXPECT_CALL(mWakeLockManager, acquireWakeLock()).Times(1);

        // VPD triggers init to start CpuComDaemon
        EXPECT_CALL(mAndroidInit, propertySet("vendor.cpucomdaemon", "start")).Times(1);

        // And then subscribes to it and starts listening for messages
        EXPECT_CALL(*mCpuCom, subscribe(testing::An<common::CpuCommand>(),
                                        testing::An<cpucom::ICpuCom::OnCommand>()))
            .Times(2);
        EXPECT_CALL(*mCpuCom, connect()).Times(1);

        // VPD triggers init to start NativeDaemons
        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpstart", "start")).Times(1);

        // But VPD doesn't trigger to start svc_csxe, because of the condition in config is equal to
        // false
        EXPECT_CALL(mAndroidInit, propertySet("vendor.svc_csxe", "start")).Times(0);

        // VPD sends start complete and main start complete to V-CPU
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);
        EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(1);

        // VPD sets property to indicate that coldboot is complete
        EXPECT_CALL(mAndroidInit, propertySet("vendor.melco.boot_completed", "1")).Times(1);
        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpd_started", "1")).Times(1);

        // VPD starts to send WDT reset request to V-CPU
        EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(1);
    }

    startDaemon();
    coldBoot();
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
    stopDaemon();
}

TEST_F(ColdbootTest, REF_659_VpdRestart)
{
    {
        testing::InSequence s;

        // VPD starts wake up event detection
        EXPECT_CALL(mWakeUpEventDetector, start()).Times(1);

        // After restart VPD doesn't acquire wakelock, because it already has done
        EXPECT_CALL(mWakeLockManager, acquireWakeLock()).Times(0);

        // VPD doesn't trigger init to start CpuComDaemon
        EXPECT_CALL(mAndroidInit, propertySet("vendor.cpucomdaemon", "start")).Times(0);

        // but subscribes to it and starts listening for messages
        EXPECT_CALL(*mCpuCom, subscribe(testing::An<common::CpuCommand>(),
                                        testing::An<cpucom::ICpuCom::OnCommand>()))
            .Times(2);
        EXPECT_CALL(*mCpuCom, connect()).Times(1);

        // VPD doesn't trigger init to start NativeDaemons
        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpstart", "start")).Times(0);
        EXPECT_CALL(mAndroidInit, propertySet("vendor.svc_csxe", "start")).Times(0);
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(0);
        EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(0);
        EXPECT_CALL(mAndroidInit, propertySet("vendor.melco.boot_completed", "1")).Times(0);

        // VPD sets property to indicate that boot is complete
        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpd_started", "1")).Times(1);

        // And starts to send WDT reset request to V-CPU
        EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(1);
    }

    EXPECT_CALL(mMessageServer,
                sendNotification(kEikDaemonSessionId, EVpdNotifications::AppStartColdboot))
        .Times(1);
    EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(0);

    defineProperty("vendor.vpd_started", "1");
    startDaemon();

    ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));

    // EIK daemon subscribes to VPD
    sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::SubscribeNative);

    // VPD starts sending WDTR requests to V-CPU
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));
    stopDaemon();
}

TEST_F(ColdbootTest, REF_659_VpsSendsStartFailed)
{
    {
        testing::InSequence s;

        // VPD starts wake up event detection
        EXPECT_CALL(mWakeUpEventDetector, start()).Times(1);

        // VPD acquires wakelock
        EXPECT_CALL(mWakeLockManager, acquireWakeLock()).Times(1);

        // VPD triggers init to start CpuComDaemon
        EXPECT_CALL(mAndroidInit, propertySet("vendor.cpucomdaemon", "start")).Times(1);

        // And then subscribes to it and starts listening for messages
        EXPECT_CALL(*mCpuCom, subscribe(testing::An<common::CpuCommand>(),
                                        testing::An<cpucom::ICpuCom::OnCommand>()))
            .Times(2);
        EXPECT_CALL(*mCpuCom, connect()).Times(1);

        // VPD triggers init to start NativeDaemons
        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpstart", "start")).Times(1);
        EXPECT_CALL(mAndroidInit, propertySet("vendor.svc_csxe", "start")).Times(1);

        // VPD sends start complete
        EXPECT_CALL(*mCpuCom, send(kStartCompleteNotificationCommand, _, _)).Times(1);

        // VPD sends self-reboot command to V-CPU
        EXPECT_CALL(*mCpuCom, send(kSelfRebootCommand, _)).Times(1);

        // VPD doesn't send main start complete to V-CPU
        EXPECT_CALL(*mCpuCom, send(kMainStartCompleteCommand, _)).Times(0);

        // VPD starts to send WDT reset request to V-CPU
        EXPECT_CALL(*mCpuCom, send(kWdtrRequestCommand, _)).Times(testing::AtLeast(1));

        // VPD doesn't set property to indicate that coldboot is complete
        EXPECT_CALL(mAndroidInit, propertySet("vendor.melco.boot_completed", "1")).Times(0);

        // VPD doesn't set property to indicate that VPD boot is complete
        EXPECT_CALL(mAndroidInit, propertySet("vendor.vpd_started", "1")).Times(0);
    }

    startDaemon();
    startCpuCom();
    startNativeDaemons();
    startVps(false);

    // VPD sends self-reboot requests to V-CPU
    ASSERT_TRUE(waitForCpuComMessage(kSelfRebootCommand));
    ASSERT_TRUE(waitForCpuComMessage(kWdtrRequestCommand, kWdtrTimeoutMs));

    stopDaemon();
}

TEST_F(ColdbootTest, REF_27740_HandOffResponibility)
{
    startDaemon();
    startCpuCom();
    startNativeDaemons();
    startVps(true);

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId, EVpdEvent::handOffResponsibility);
    ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, EVpdNotifications::HandOffResponsibility));

    stopDaemon();
}

TEST_F(ColdbootTest, REF_27740_HandOffResponsibilityStartupAnimationAndDiDisplayed)
{
    startDaemon();
    startCpuCom();
    startNativeDaemons();
    startVps(true);

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

    stopDaemon();
}

TEST_F(ColdbootTest, REF_27740_HandOffResponsibilityStartupAnimationDisplayed)
{
    startDaemon();
    startCpuCom();
    startNativeDaemons();
    startVps(true);

    sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                 EVpdEvent::handOffResponsibilityStartupAnimationDisplayed);
    ASSERT_TRUE(waitForVpdNotification(
        kVpsSessionId, EVpdNotifications::HandOffResponsibilityStartupAnimationDisplayed));

    stopDaemon();
}
