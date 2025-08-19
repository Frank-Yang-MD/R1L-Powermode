/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NotificationReceiver.h"
#include "EVpdNotifications.h"
#include "LibVehiclePwrLog.h"

#include "IInternalCallback.h"

#include <unistd.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

NotificationReceiver::NotificationReceiver()
    : mInternalCallback{nullptr}
{
}

void NotificationReceiver::setInternalCallback(IInternalCallback* inInternalCallback)
{
    mInternalCallback = inInternalCallback;
}

void NotificationReceiver::onNotificationReceived(common::EVpdNotifications vpdNotification)
{
    MLOGI(LogFuncId::VPL, LogId::NotificationReceiver_onNotificationReceived, getpid(),
          static_cast<int>(vpdNotification));

    const std::lock_guard<std::mutex> lock{mMutex};
    sendNotifications();

    onNotificationReceivedInternal(vpdNotification, false);
}

void NotificationReceiver::addHandler(common::EVpdNotifications vpdNotification,
                                      IVpdNotificationHandler* handler)
{
    mHandlers[vpdNotification] = handler;
}

bool NotificationReceiver::onNotificationReceivedInternal(common::EVpdNotifications vpdNotification,
                                                          bool internal)
{
    MLOGI(LogFuncId::VPL, LogId::NotificationReceiver_onNotificationReceivedInternal, getpid(),
          internal, static_cast<int>(vpdNotification));

    bool processed = false;

    if (mHandlers.count(vpdNotification)) {
        processed = mHandlers[vpdNotification]->onVpdNotification(vpdNotification);
        if (!processed) {
            if (!internal) {
                mReceivedNotifications.emplace_back(vpdNotification);
                MLOGI(LogFuncId::VPL,
                      LogId::NotificationReceiver_onNotificationReceivedInternal_not_processed,
                      getpid(), static_cast<int>(vpdNotification));
            }
        }
        else {
            MLOGI(LogFuncId::VPL,
                  LogId::NotificationReceiver_onNotificationReceivedInternal_processed, getpid(),
                  static_cast<int>(vpdNotification));
        }
    }

    return processed;
}

void NotificationReceiver::onConnect()
{
    if (!mInternalCallback) {
        MLOGI(LogFuncId::VPL, LogId::NotificationReceiver_onConnect_Native_callback_is_not_set);
    }
    else {
        mInternalCallback->onConnect();
    }
}

void NotificationReceiver::onDisconnect()
{
    if (!mInternalCallback) {
        MLOGI(LogFuncId::VPL, LogId::NotificationReceiver_onDisconnect_Native_callback_is_not_set);
    }
    else {
        mInternalCallback->onDisconnect();
    }
}

void NotificationReceiver::processReceivedNotifications()
{
    MLOGI(LogFuncId::VPL, LogId::NotificationReceiver_processReceivedNotifications, getpid());
    const std::lock_guard<std::mutex> lock{mMutex};
    sendNotifications();
}

void NotificationReceiver::sendNotifications()
{
    for (auto it = std::cbegin(mReceivedNotifications); it != std::cend(mReceivedNotifications);) {
        if (onNotificationReceivedInternal(*it, true)) {
            it = mReceivedNotifications.erase(it);
        }
        else {
            it++;
        }
    }
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
