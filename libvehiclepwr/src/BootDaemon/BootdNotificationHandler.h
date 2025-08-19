/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_BOOTDNOTIFICATIONHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_BOOTDNOTIFICATIONHANDLER_H_

#include "IBootdCallbackHandler.h"
#include "IVpdNotificationHandler.h"

#include "IBootDaemonCallback.h"

#include <mutex>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

/**
 * BootdNotificationHandler - handles BootDaemon notifications received from VPD.
 */
class BootdNotificationHandler final
    : public IVpdNotificationHandler
    , public IBootdCallbackHandler {
public:
    virtual bool onVpdNotification(common::EVpdNotifications) override;
    virtual void setBootDaemonCallback(IBootDaemonCallback*) override;

private:
    std::mutex mMutex;

    IBootDaemonCallback* mBootdCallback = nullptr;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_BOOTDNOTIFICATIONHANDLER_H_
