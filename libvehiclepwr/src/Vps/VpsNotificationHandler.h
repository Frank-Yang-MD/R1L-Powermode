/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPSNOTIFICATIONHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPSNOTIFICATIONHANDLER_H_

#include "IVpdNotificationHandler.h"
#include "IVpsCallbackHandler.h"

#include "IVpsCallback.h"

#include <mutex>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

/**
 * VpsNotificationHandler - handles VPS notifications received from VPD.
 */
class VpsNotificationHandler final
    : public IVpdNotificationHandler
    , public IVpsCallbackHandler {
public:
    virtual bool onVpdNotification(common::EVpdNotifications) override;
    virtual void setVpsCallback(IVpsCallback*) override;

private:
    std::mutex mMutex;

    IVpsCallback* mVpsCallback = nullptr;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPSNOTIFICATIONHANDLER_H_
