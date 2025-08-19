/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "AndroidPmAdapter.h"
#include "VpmBridgeDaemonLog.h"

#include <binder/IBinder.h>
#include <binder/IServiceManager.h>
#include <binder/Parcel.h>
#include <powermanager/PowerManager.h>
#include <utils/SystemClock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace vpmbridgedaemon {

bool AndroidPmAdapter::acquireAndroidPowerManager()
{
    bool result = true;

    if (mAndroidPowerManager == nullptr) {
        if (acquireAndroidPowerManagerBinder()) {
            mAndroidPowerManager = interface_cast<IPowerManager>(mAndroidPowerManagerBinder);

            if (mAndroidPowerManager == nullptr) {
                result = false;
                MLOGW(LogFuncId::VPBD, LogId::AndroidPmAdapter_Cannot_cast_to_Android_PM_instance);
            }
            else {
                MLOGI(LogFuncId::VPBD, LogId::AndroidPmAdapter_Android_PM_instance_obtained);
            }
        }
    }

    return result;
}

bool AndroidPmAdapter::acquireAndroidPowerManagerBinder()
{
    bool result = true;

    if (mAndroidPowerManagerBinder == nullptr) {
        const String16 serviceName("power");
        mAndroidPowerManagerBinder = defaultServiceManager()->checkService(serviceName);

        if (mAndroidPowerManagerBinder == nullptr) {
            result = false;
            MLOGW(LogFuncId::VPBD, LogId::AndroidPmAdapter_Android_PM_binder_is_null);
        }
    }

    return result;
}

void AndroidPmAdapter::retryCall(std::function<status_t()> func)
{
    status_t ret = android::UNKNOWN_ERROR;
    while (ret != android::OK) {
        if (acquireAndroidPowerManager()) {
            ret = func();

            if (ret != android::OK) {
                MLOGW(LogFuncId::VPBD, LogId::AndroidPmAdapter_Request_to_Android_PM_failed);
                mAndroidPowerManager = nullptr;
                mAndroidPowerManagerBinder = nullptr;
            }
        }
        else {
            MLOGW(LogFuncId::VPBD, LogId::AndroidPmAdapter_Cannot_obtain_Android_PM_instance);
        }
    }
}

void AndroidPmAdapter::shutdown()
{
    MLOGI(LogFuncId::VPBD, LogId::AndroidPmAdapter_Request_to_Android_PM, __func__);
    retryCall([this] { return mAndroidPowerManager->shutdown(false, String16(""), true); });
}

void AndroidPmAdapter::suspend()
{
    MLOGI(LogFuncId::VPBD, LogId::AndroidPmAdapter_Request_to_Android_PM, __func__);
    retryCall([this] {
        const int64_t uptime = uptimeMillis();
        return mAndroidPowerManager->goToSleep(uptime, GO_TO_SLEEP_REASON_POWER_BUTTON,
                                               GO_TO_SLEEP_FLAG_NO_DOZE);
    });
}

void AndroidPmAdapter::reboot()
{
    MLOGI(LogFuncId::VPBD, LogId::AndroidPmAdapter_Request_to_Android_PM, __func__);
    retryCall([this] { return mAndroidPowerManager->reboot(false, String16(""), true); });
}

void AndroidPmAdapter::rebootRecovery()
{
    MLOGI(LogFuncId::VPBD, LogId::AndroidPmAdapter_Request_to_Android_PM, __func__);
    retryCall([this] { return mAndroidPowerManager->reboot(false, String16("recovery"), true); });
}

void AndroidPmAdapter::wakeUp()
{
    const auto reason = String16("android.policy:POWER");
    MLOGI(LogFuncId::VPBD, LogId::AndroidPmAdapter_Request_to_Android_PM, __func__);
    retryCall([&reason, this] {
        const auto uptime = uptimeMillis();
        return wakeUp(uptime, reason);
    });
}

status_t AndroidPmAdapter::wakeUp(int64_t time, const String16& reason)
{
    /*
     * Build transaction manually because Android PM native interface doesn't
     * have inteface to notify wake up from third-party app.
     */

    Parcel data, reply;
    data.writeInterfaceToken(mAndroidPowerManager->getInterfaceDescriptor());
    data.writeInt64(time);
    data.writeString16(reason);

    return mAndroidPowerManagerBinder->transact(IPowerManager::WAKE_UP, data, &reply, 0U);
}

void AndroidPmAdapter::forceSuspend()
{
    if (!acquireAndroidPowerManager()) {
        MLOGW(LogFuncId::VPBD, LogId::AndroidPmAdapter_Cannot_obtain_Android_PM_instance, __func__);
        return;
    }

    // This transaction ID must be kept in sync with the method order from
    // IPowerManager.aidl.
    const int FORCE_SUSPEND = IBinder::FIRST_CALL_TRANSACTION + 32;

    MLOGI(LogFuncId::VPBD, LogId::AndroidPmAdapter_Request_to_Android_PM, __func__);

    /*
     * Build transaction manually because Android PM native interface doesn't
     * have inteface to force suspend from third-party app.
     */
    Parcel data, reply;
    data.writeInterfaceToken(mAndroidPowerManager->getInterfaceDescriptor());

    mAndroidPowerManagerBinder->transact(FORCE_SUSPEND, data, &reply, 0U);
}

}  // namespace vpmbridgedaemon
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
