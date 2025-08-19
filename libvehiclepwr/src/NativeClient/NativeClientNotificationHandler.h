/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_NATIVECLIENTNOTIFICATIONHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_NATIVECLIENTNOTIFICATIONHANDLER_H_

#include "INativeCallbackHandler.h"
#include "IVpdNotificationHandler.h"
#include "IVpdStateProvider.h"

#include "INativeCallback.h"

#include "EVpdState.h"

#include <mutex>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

/**
 * NativeClientNotificationHandler - interface to handle received notification from VPD.
 */
class NativeClientNotificationHandler final
    : public IVpdNotificationHandler
    , public INativeCallbackHandler
    , public IVpdStateProvider {
public:
    virtual bool onVpdNotification(common::EVpdNotifications) override;
    virtual void setNativeCallback(INativeCallback*) override;
    virtual common::EVpdState getVpdState() override;

private:
    std::mutex mMutex;

    INativeCallback* mNativeCallback = nullptr;
    common::EVpdState mVpdState = common::EVpdState::APP_START_NA;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_NATIVECLIENTNOTIFICATIONHANDLER_H_
