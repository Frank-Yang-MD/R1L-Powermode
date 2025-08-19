/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#pragma once

#include "EVpdNotifications.h"
#include "MockAndroidInit.h"
#include "MockConfigReader.h"
#include "MockKernelModlesControl.h"
#include "MockLogDaemonActions.h"
#include "MockMessageServer.h"
#include "MockVpdBridgeAdapter.h"
#include "MockWakeLockManager.h"
#include "MockWakeUpEventDetector.h"
#include "MockWakeUpManager.h"
#include "Queue.h"
#include "VehiclePowerDaemon.h"
#include <mock/mock_CpuCom.h>

#include "EventQueue.h"
#include "VpdMessageServer.h"

#include "Executors.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <utility>
#include <variant>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::_;
using ::testing::InSequence;
using ::testing::NiceMock;
using ::testing::Test;

namespace vpd = com::mitsubishielectric::ahu::vehiclepwrmgr::impl;
namespace vpd_common = com::mitsubishielectric::ahu::vehiclepwrmgr::common;
namespace cpucom = com::mitsubishielectric::ahu::cpucom::v2;
namespace common = com::mitsubishielectric::ahu::common;

class VpdIntegrationTest : public Test {
public:
    static constexpr int VPOWER_APPSTOP_JAVA_TIMEOUT = 700;
    static constexpr int VPOWER_APPSTOP_LOGDOG_TIMEOUT = 500;
    static constexpr int VPOWER_APPSTOP_NATIVE_TIMEOUT = 500;
    static constexpr int VPOWER_APPRESUME_EFW_TIMEOUT = 500;
    static constexpr int VPOWER_APPRESUME_APP_TIMEOUT = 500;
    static constexpr int VPOWER_LOGSAVE_TIMEOUT = 500;

    VpdIntegrationTest()
    {
        // set default values for properties
        defineProperty("init.svc.vendor.bootdaemon", "running");
        defineProperty("init.svc.vendor.extinputkeydaemon", "running");
        defineProperty("init.svc.vendor.svc_csxe", "running");

        // set default values for VehicleConfig
        defineVehicleConfigValue("PROXI_SDARS_PRESENCE", (uint8_t)1);
        defineVehicleConfigValue("VPM.NATIVE_SERVICES_INIT_CONFIG",
                                 "vendor.vpstart: bootdaemon extinputkeydaemon\nvendor.svc_csxe: "
                                 "svc_csxe : PROXI_SDARS_PRESENCE.byte == 1");

        defineVehicleConfigValue("VPOWER_APPSTOP_JAVA_TIMEOUT", VPOWER_APPSTOP_JAVA_TIMEOUT);
        defineVehicleConfigValue("VPOWER_APPSTOP_LOGDOG_TIMEOUT", VPOWER_APPSTOP_LOGDOG_TIMEOUT);
        defineVehicleConfigValue("VPOWER_APPSTOP_NATIVE_TIMEOUT", VPOWER_APPSTOP_NATIVE_TIMEOUT);
        defineVehicleConfigValue("VPOWER_APPRESUME_EFW_TIMEOUT", VPOWER_APPRESUME_EFW_TIMEOUT);
        defineVehicleConfigValue("VPOWER_APPRESUME_APP_TIMEOUT", VPOWER_APPRESUME_APP_TIMEOUT);
        defineVehicleConfigValue("VPOWER_LOGSAVE_TIMEOUT", VPOWER_LOGSAVE_TIMEOUT);

        // capture message handler method
        ON_CALL(mMessageServer, setMessageHandler(_))
            .WillByDefault(testing::Invoke([this](auto handler) { mVpdMessageHandler = handler; }));

        // capture connect and disconnect handlers
        ON_CALL(mMessageServer, initialize(_, _))
            .WillByDefault(testing::Invoke([this](auto connectHandler, auto disconnectHandler) {
                mVpdConnectionHandler = connectHandler;
                mVpdDisconnectionHandler = disconnectHandler;
            }));

        ON_CALL(mMessageServer, sendNotification(_, _))
            .WillByDefault(testing::Invoke([this](auto sessionId, auto vpdNotification) {
                mVpdNotificationQueue.push(std::make_pair(sessionId, vpdNotification));
            }));

        // capture CpuCom message listener
        ON_CALL(*mCpuCom, subscribe(testing::An<common::CpuCommand>(),
                                    testing::An<cpucom::ICpuCom::OnCommand>()))
            .WillByDefault(
                testing::Invoke([this](auto, auto callback) { mCpuComListener = callback; }));

        ON_CALL(mAndroidInit, propertySet(_, _))
            .WillByDefault(testing::Invoke([this](auto propName, auto propValue) -> int {
                mPropertySetQueue.push(std::make_pair(propName, propValue));
                return 0;
            }));

        ON_CALL(mAndroidInit, propertyGet(_, _))
            .WillByDefault(testing::Invoke([this](auto propName, std::string& propValue) -> int {
                propValue = getPropertyValue(propName);
                return propValue.size();
            }));

        ON_CALL(mConfigReader, getInt(_, _))
            .WillByDefault(testing::Invoke([=](const std::string& key, int& value) {
                return getVehicleConfigValue<int>(key, value);
            }));

        ON_CALL(mConfigReader, getByte(_, _))
            .WillByDefault(testing::Invoke([=](const std::string& key, uint8_t& value) {
                return getVehicleConfigValue<uint8_t>(key, value);
            }));

        ON_CALL(mConfigReader, getString(_, _))
            .WillByDefault(testing::Invoke([=](const std::string& key, std::string& value) -> int {
                return getVehicleConfigValue<std::string>(key, value);
            }));

        ON_CALL(*mCpuCom,
                send(testing::An<common::CpuCommand>(), testing::An<std::vector<uint8_t>>()))
            .WillByDefault(testing::Invoke(
                [this](auto command, auto) { mSentCpuMessageQueue.push(command); }));

        ON_CALL(*mCpuCom, send(_, _, _))
            .WillByDefault(testing::Invoke([this](auto command, auto, auto callback) {
                mCpuComDeliveryStatusListener = callback;
                mSentCpuMessageQueue.push(command);
            }));

        ON_CALL(mVpdBridgeDaemonAdapter, suspend()).WillByDefault(testing::Invoke([this]() {
            mSuspend = true;
            mSuspendCondition.notify_all();
        }));

        ON_CALL(mLogDaemonActions, isLogSavingComplete()).WillByDefault(testing::Invoke([this]() {
            mIsLogSavingCalled = true;
            mIsLogSavingCondition.notify_all();
            return true;
        }));

        ON_CALL(mKernelModulesControl, skipModulesUnloadedCheck())
            .WillByDefault(testing::Return(true));
        ON_CALL(mKernelModulesControl, isModulesUnloaded()).WillByDefault(testing::Return(true));

        mVpd = std::make_unique<VehiclePowerDaemon>(
            mVpdBridgeDaemonAdapter, mWakeLockManager, mKernelModulesControl, mLogDaemonActions,
            mConfigReader, mEventQueue, mEventQueue, mVpdMessageServer,
            std::bind(&VpdIntegrationTest::createCpuCom, this), mClock, mAndroidInit,
            mWakeUpEventDetector, mWakeUpManager, kAbnormalPeriod, kWatchdogPeriod,
            kModulesUnloadTimeout);
    }

    void sendVpdEvent(SessionID sessionId, int pid, const vpd_common::EVpdEvent event)
    {
        if (mVpdMessageHandler) {
            mVpdMessageHandler(std::move(sessionId), event, pid);
        }
    }

    void sendCpuMessage(const common::CpuCommand& command, const std::vector<uint8_t>& data)
    {
        mCpuComListener(command, data);
    }

    bool waitForPropertySet(const std::string& propName, const std::string& propValue)
    {
        return mPropertySetQueue.wait(std::make_pair(propName, propValue), kCallWaitTimeout);
    }

    bool waitForCpuComMessage(const common::CpuCommand& command,
                              std::chrono::milliseconds timeout = std::chrono::milliseconds(5000))
    {
        return mSentCpuMessageQueue.wait(command, timeout);
    }

    bool waitForVpdNotification(const SessionID& sessionId,
                                const vpd_common::EVpdNotifications& notification,
                                std::chrono::milliseconds timeout = std::chrono::milliseconds(5000))
    {
        return mVpdNotificationQueue.wait(std::make_pair(sessionId, notification), timeout);
    }

    bool waitForSuspend(std::chrono::milliseconds timeout = std::chrono::milliseconds(5000))
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mSuspendCondition.wait_for(lock, timeout, [this]() { return mSuspend; });
        return mSuspend;
    }

    bool waitForIsSaveLogComplete(
        std::chrono::milliseconds timeout = std::chrono::milliseconds(5000))
    {
        std::unique_lock<std::mutex> lock(mMutex);
        mIsLogSavingCondition.wait_for(lock, timeout, [this]() { return mIsLogSavingCalled; });
        return mIsLogSavingCalled;
    }

    void wakeUp() { mEventQueue.onEventReceive(0, vpd_common::EVpdEvent::WakeUp); }

    void startDaemon()
    {
        mExecutor.execute([this]() {
            mVpdMessageServer.start();
            mVpd->StartDaemon();
        });
    }

    void stopDaemon()
    {
        mVpd->StopDaemon();
        mEventQueue.onEventReceive(0, vpd_common::EVpdEvent::Timeout);
    }

    void startCpuCom()
    {
        // VPD starts CpuComDaemon by setting vendor.cpucomdaemon property
        ASSERT_TRUE(waitForPropertySet("vendor.cpucomdaemon", "start"));

        // CpuComDaemon sends start comletion message to VPD
        sendVpdEvent(kCpuComSessionId, kCpuComId, vpd_common::EVpdEvent::CpuComStarted);
    }

    void startNativeDaemons()
    {
        // VPD starts native daemons by setting vendor.vpstart property
        ASSERT_TRUE(waitForPropertySet("vendor.vpstart", "start"));

        // BootDaemon subscribes to VPS
        sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                     vpd_common::EVpdEvent::SubscribeBootDaemon);

        // EIK daemon subscribes to VPS
        sendVpdEvent(kEikDaemonSessionId, kEikDaemonId, vpd_common::EVpdEvent::SubscribeNative);

        // Logdog subscribes to VPS
        sendVpdEvent(kLogdogSessionId, kLogDogId, vpd_common::EVpdEvent::SubscribeLogdog);

        // Check if VPD notifies to EikDaemon current power state (N/A)
        ASSERT_TRUE(
            waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStartNa));

        // after native daemons are started, VPD sends start completion message to V-CPU
        ASSERT_TRUE(waitForCpuComMessage(kStartCompleteNotificationCommand));

        // Check if VPD notifies to EikDaemon current power state (Coldboot)
        ASSERT_TRUE(waitForVpdNotification(kEikDaemonSessionId,
                                           vpd_common::EVpdNotifications::AppStartColdboot));
    }

    void startVps(bool startSuccess)
    {
        // VPS subscribes to VPD
        sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::SubscribeNative);
        sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::SubscribeVps);

        // Check if VPD notifies to VPS current power state (Coldboot)
        ASSERT_TRUE(
            waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStartColdboot));

        // VPS sends "HMI is ready" message to VPD
        sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::VpsReady);

        // VPD notifies to BootDaemon that HMI is ready
        ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                           vpd_common::EVpdNotifications::HmiEventVpsReady));

        // VPS sends start completion message to VPD
        if (startSuccess) {
            sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::VpsStarted);

            // VPD sends main start completion message to V-CPU
            ASSERT_TRUE(waitForCpuComMessage(kMainStartCompleteCommand));

            // VPD sets vendor.melco.boot_completed and vendor.vpd_started properties
            ASSERT_TRUE(waitForPropertySet("vendor.melco.boot_completed", "1"));
            ASSERT_TRUE(waitForPropertySet("vendor.vpd_started", "1"));
        }
        else {
            sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::StartFailedVps);
        }
    }

    void coldBoot(bool vpsStartSuccessfully = true)
    {
        startCpuCom();
        startNativeDaemons();
        startVps(vpsStartSuccessfully);

        // BootDaemon sends "hand off responsibility with startup animation/DI" message to VPD
        sendVpdEvent(kBootDaemonSessionId, kBootDaemonId,
                     vpd_common::EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed);

        // VPD sends "hand off responsibility with startup animation/DI" message to VPS
        ASSERT_TRUE(waitForVpdNotification(
            kVpsSessionId,
            vpd_common::EVpdNotifications::HandOffResponsibilityStartupAnimationAndDiDisplayed));

        // VPS sends "Request to end Early HMI"
        sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::EndEarlyHmi);

        // VPD sends "Request to end Early HMI" to BootDaemon
        ASSERT_TRUE(waitForVpdNotification(kBootDaemonSessionId,
                                           vpd_common::EVpdNotifications::EndEarlyHmi));
    }

    void stopAppServices(bool timeout)
    {
        // VPD requests VPS tp notify APP_STOP to App services
        ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::AppStop));

        if (!timeout) {
            // VPS sends APP stop complete to VPD
            sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::AppStopCompleteVps);
        }
        else {
            // If stopComplete from java FW services timeout occurs
            // 1. Check if VPD properly measures FW_STOP complete timeout
            const auto kDeltaTime = 10;
            const auto kValueLessThanFwStopCompleteTimeout =
                VPOWER_APPSTOP_JAVA_TIMEOUT / 2 - kDeltaTime;
            ASSERT_FALSE(waitForVpdNotification(
                kVpsSessionId, vpd_common::EVpdNotifications::TimeOutError,
                std::chrono::milliseconds(kValueLessThanFwStopCompleteTimeout)));
            ASSERT_TRUE(waitForVpdNotification(kVpsSessionId,
                                               vpd_common::EVpdNotifications::TimeOutError,
                                               std::chrono::milliseconds(2 * kDeltaTime)));

            // 2. Check if VPD additionally waits for 300ms before sending FwStop event to VPS
            const auto kWaitBeforeSendingFwStopToVps = 300;
            ASSERT_FALSE(waitForVpdNotification(
                kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop,
                std::chrono::milliseconds((kWaitBeforeSendingFwStopToVps - kDeltaTime))));
        }
    }

    void stopEfwServices(bool appStopCompleteTimeout,
                         bool efwStopCompleteTimeout,
                         bool stopProcessingFailure)
    {
        // VPD requests VPS to notify APP_STOP to EFW servicesV
        ASSERT_TRUE(waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::FwStop));

        if (!efwStopCompleteTimeout) {
            if (stopProcessingFailure) {
                // VPS sends stop failed message to VPD
                sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::StopFailedVps);
            }
            else {
                // VPS sends EFW stop complete to VPD
                sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::FwStopCompleteVps);
            }
        }
        else {
            // If stopComplete from java FW services efwStopCompleteTimeout occurs
            // 1. Check if VPD properly measures FW_STOP complete efwStopCompleteTimeout
            const auto kDeltaTime = 10;

            // EFW completion timeout value depends on weather appStopCompleteTimeout occurs or not.
            // If appStopCompleteTimeout occurs then EFW completion timeout is 2 times smaller than
            // VPOWER_APPSTOP_JAVA_TIMEOUT. Also, 300ms is subtracted from EFW completion timeout
            // value because VPD should wait for 300ms before sending FwStop event to VPS if
            // appStopCompleteTimeout occurs.
            const int divider = appStopCompleteTimeout ? 2 : 1;
            const int toSubsctract = appStopCompleteTimeout ? 300 : 0;
            const auto kValueLessThanFwStopCompleteTimeout =
                (VPOWER_APPSTOP_JAVA_TIMEOUT / divider) - kDeltaTime - toSubsctract;
            ASSERT_FALSE(waitForVpdNotification(
                kVpsSessionId, vpd_common::EVpdNotifications::TimeOutError,
                std::chrono::milliseconds(kValueLessThanFwStopCompleteTimeout)));
            ASSERT_TRUE(waitForVpdNotification(kVpsSessionId,
                                               vpd_common::EVpdNotifications::TimeOutError,
                                               std::chrono::milliseconds(1 * kDeltaTime)));

            // 2. Check if VPD additionally waits for 300ms before sending AppStop event to native
            // services
            const auto kWaitBeforeSendingAppStopToNative = 300;
            ASSERT_FALSE(waitForVpdNotification(
                kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop,
                std::chrono::milliseconds((kWaitBeforeSendingAppStopToNative - kDeltaTime))));
        }
    }

    void stopNativeService(bool timeout)
    {
        // VPD notifies APP_STOP to Native services
        ASSERT_TRUE(
            waitForVpdNotification(kEikDaemonSessionId, vpd_common::EVpdNotifications::AppStop));

        if (!timeout) {
            // Native services sends stop complete to VPD
            sendVpdEvent(kEikDaemonSessionId, kEikDaemonId,
                         vpd_common::EVpdEvent::StopCompleteNative);
        }
        else {
            // If stopComplete from native services timeout occurs
            // VPD should additionally wait for 300ms before start polling log saving state
            const auto kTimeBeforePollingLogSaveState = 300;
            const auto kDeltaTime = 10;
            const auto kTimeout =
                VPOWER_APPSTOP_NATIVE_TIMEOUT + kTimeBeforePollingLogSaveState - kDeltaTime;
            ASSERT_FALSE(waitForIsSaveLogComplete(std::chrono::milliseconds(kTimeout)));
            ASSERT_TRUE(waitForIsSaveLogComplete(std::chrono::milliseconds(2 * kDeltaTime)));
        }
    }

    void stopLogdogDaemon(bool timeout)
    {
        // VPD notifies APP_STOP to LogDog daemon
        ASSERT_TRUE(
            waitForVpdNotification(kLogdogSessionId, vpd_common::EVpdNotifications::AppStop));

        if (!timeout) {
            // LogDog daemon sends stop complete to VPD
            sendVpdEvent(kLogdogSessionId, kEikDaemonId, vpd_common::EVpdEvent::StopCompleteLogdog);
        }
    }

    void unmountUsbDevices(bool timeout)
    {
        // VPD requests unmounting USB devices
        ASSERT_TRUE(
            waitForVpdNotification(kVpsSessionId, vpd_common::EVpdNotifications::RequestUnmount));

        if (!timeout) {
            // VPS sends unmount complete to VPD
            sendVpdEvent(kVpsSessionId, kVpsId, vpd_common::EVpdEvent::UnmountComplete);
        }
    }

    void sendShutdownStartNotification(bool callbackResult)
    {
        ASSERT_TRUE(waitForCpuComMessage(kShutdownStartNotification));
        mCpuComDeliveryStatusListener(callbackResult);
    }

    void suspend(bool appStopTimeout,
                 bool efwStopTimeout,
                 bool stopProcessingFailure,
                 bool nativeStopTimeout,
                 bool logdogStopTimeout,
                 bool usbUnmountTimeout,
                 bool shutdownStartCallbackResult)
    {
        // V-CPU sends shutdown request to VPD
        sendCpuMessage(kShutdownRequestCommand, kSuspendRequestData);
        stopAppServices(appStopTimeout);
        stopEfwServices(appStopTimeout, efwStopTimeout, stopProcessingFailure);
        stopNativeService(nativeStopTimeout);
        stopLogdogDaemon(logdogStopTimeout);
        unmountUsbDevices(usbUnmountTimeout);
        sendShutdownStartNotification(shutdownStartCallbackResult);
    }

    void defineProperty(const std::string& name, const std::string& value)
    {
        std::lock_guard<std::mutex> lock(mPropertyValuesMutex);
        mPropertyValues[name] = value;
    }

    std::string getPropertyValue(const std::string& name) const
    {
        std::lock_guard<std::mutex> lock(mPropertyValuesMutex);
        auto it = mPropertyValues.find(name);
        return it == mPropertyValues.end() ? "" : it->second;
    }

    template <class T>
    void defineVehicleConfigValue(const std::string& name, const T& value)
    {
        std::lock_guard<std::mutex> lock(mVehcleConfigMutex);
        std::variant<uint8_t, int, std::string> variantValue = value;
        mVehicleConfigValues[name] = variantValue;
    }

    std::variant<uint8_t, int, std::string> getVehicleConfigValue(const std::string& name) const
    {
        std::lock_guard<std::mutex> lock(mVehcleConfigMutex);
        auto it = mVehicleConfigValues.find(name);
        return it == mVehicleConfigValues.end() ? std::variant<uint8_t, int, std::string>()
                                                : it->second;
    }

    template <class T>
    int getVehicleConfigValue(const std::string& name, T& value) const
    {
        auto variant = getVehicleConfigValue(name);
        if (std::holds_alternative<T>(variant)) {
            value = std::get<T>(variant);
            return 0;
        }
        return -1;
    }

    const SessionID kCpuComSessionId{"CpuCom"};
    const int kCpuComId{1};
    const SessionID kBootDaemonSessionId{"bootdaemon"};
    const int kBootDaemonId{2};
    const SessionID kVpsSessionId{"VPS"};
    const int kVpsId{3};
    const SessionID kEikDaemonSessionId{"EikDaemon"};
    const int kEikDaemonId{4};
    const SessionID kLogdogSessionId{"logdog"};
    const int kLogDogId{5};

    const std::chrono::microseconds kAbnormalPeriod{50ms};
    const std::chrono::microseconds kWatchdogPeriod{90ms};
    const std::chrono::microseconds kModulesUnloadTimeout{300ms};
    const std::chrono::milliseconds kWdtrTimeoutMs = std::chrono::seconds{11};

    const CpuCommand kMainStartCompleteCommand{0x71, 0x87};
    const CpuCommand kStartCompleteNotificationCommand{0x71, 0x81};
    const CpuCommand kWdtrRequestCommand{0x71, 0x06};
    const CpuCommand kSelfRebootCommand{0x85, 0x8F};
    const CpuCommand kShutdownRequestCommand{0x71, 0x02};
    const CpuCommand kShutdownCancelRequestCommand{0x71, 0x05};
    const CpuCommand kShutdownStartNotification{0x71, 0x83};
    const CpuCommand kShutdownCancelAcceptResponse{0x71, 0x85};
    const CpuCommand kShutdownCancelCompleteResponse{0x71, 0xd};
    const CpuCommand kAdtrRequest{0x71, 0x04};
    const std::vector<uint8_t> kSuspendRequestData{0x01};
    const std::vector<uint8_t> kSuspendCancelRequestData{};

    NiceMock<vpd::MockAndroidInit> mAndroidInit;
    NiceMock<vpd::MockConfigReader> mConfigReader;
    NiceMock<cpucom::mock_ICpuCom>* mCpuCom = new NiceMock<cpucom::mock_ICpuCom>();
    NiceMock<vpd::MockKernelModulesControl> mKernelModulesControl;
    NiceMock<vpd::MockLogDaemonActions> mLogDaemonActions;
    NiceMock<vpd::MockMessageServer> mMessageServer;
    NiceMock<vpd::MockVpdBridgeAdapter> mVpdBridgeDaemonAdapter;
    NiceMock<vpd::MockWakeLockManager> mWakeLockManager;
    NiceMock<vpd::MockWakeUpEventDetector> mWakeUpEventDetector;
    NiceMock<vpd::MockWakeUpManager> mWakeUpManager;

    cpucom::ICpuCom::DeliveryStatusCallback mCpuComDeliveryStatusListener;

private:
    const std::chrono::milliseconds kCallWaitTimeout = 5000ms;

    VpdClock mClock;
    vpd::EventQueue mEventQueue;
    vpd::VpdMessageServer mVpdMessageServer{mEventQueue, mMessageServer};
    common::SingleThreadExecutor mExecutor;

    std::function<void(SessionID, vpd_common::EVpdEvent, int)> mVpdMessageHandler;
    std::function<void(SessionID)> mVpdConnectionHandler;
    std::function<void(SessionID)> mVpdDisconnectionHandler;
    cpucom::ICpuCom::OnCommand mCpuComListener;

    Queue<std::pair<std::string, std::string>> mPropertySetQueue;
    Queue<CpuCommand> mSentCpuMessageQueue;
    Queue<std::pair<SessionID, vpd_common::EVpdNotifications>> mVpdNotificationQueue;

    unique_ptr<cpucom::ICpuCom> createCpuCom() { return std::unique_ptr<cpucom::ICpuCom>(mCpuCom); }

    mutable std::mutex mPropertyValuesMutex;
    std::map<std::string, std::string> mPropertyValues;

    mutable std::mutex mVehcleConfigMutex;
    std::map<std::string, std::variant<uint8_t, int, std::string>> mVehicleConfigValues;

    std::mutex mMutex;
    std::condition_variable mSuspendCondition;
    bool mSuspend = false;

    std::condition_variable mWakeUpCondition;

    std::condition_variable mIsLogSavingCondition;
    bool mIsLogSavingCalled = false;

    std::unique_ptr<VehiclePowerDaemon> mVpd;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
