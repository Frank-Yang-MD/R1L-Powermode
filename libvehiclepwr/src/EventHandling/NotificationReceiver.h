/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_NOTIFICATIONRECEIVER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_NOTIFICATIONRECEIVER_H_

#include "IDataReceiverEx.h"

#include "IInternalCallbackHandler.h"

#include "INotificationReceiver.h"
#include "IVpdNotificationHandler.h"

#include <mutex>
#include <unordered_map>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class NotificationReceiver final
    : public INotificationReceiver
    , public IInternalCallbackHandler
    , public IDataReceiverEx {
public:
    NotificationReceiver();

    void addHandler(common::EVpdNotifications, IVpdNotificationHandler*) override;
    void processReceivedNotifications() override;

    void setInternalCallback(IInternalCallback* inInternalCallback) final override;

    void onConnect() override;
    void onDisconnect() override;
    void onNotificationReceived(common::EVpdNotifications) override;

private:
    bool onNotificationReceivedInternal(common::EVpdNotifications, bool internal);

    void sendNotifications();

    IInternalCallback* mInternalCallback;

    std::unordered_map<common::EVpdNotifications, IVpdNotificationHandler*> mHandlers;

    std::vector<common::EVpdNotifications> mReceivedNotifications;
    std::mutex mMutex;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_NOTIFICATIONRECEIVER_H_
