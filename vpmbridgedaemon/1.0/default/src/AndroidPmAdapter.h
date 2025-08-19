/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHAL_ANDROIDPMADAPTER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHAL_ANDROIDPMADAPTER_H

#include <powermanager/IPowerManager.h>

#include <functional>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace vpmbridgedaemon {

using namespace android;

class AndroidPmAdapter final {
public:
    void shutdown();
    void suspend();
    void reboot();
    void rebootRecovery();
    void wakeUp();
    void forceSuspend();

private:
    enum GO_TO_SLEEP_REASON {
        GO_TO_SLEEP_REASON_POWER_BUTTON = 4,
    };

    enum GO_TO_SLEEP_FLAG {
        GO_TO_SLEEP_FLAG_NO_DOZE = 1 << 0,
    };

    status_t wakeUp(int64_t, const String16&);

    bool acquireAndroidPowerManager();
    bool acquireAndroidPowerManagerBinder();

    void retryCall(std::function<status_t()>);

    sp<IBinder> mAndroidPowerManagerBinder;
    sp<IPowerManager> mAndroidPowerManager;
};

}  // namespace vpmbridgedaemon
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPMHAL_ANDROIDPMADAPTER_H
