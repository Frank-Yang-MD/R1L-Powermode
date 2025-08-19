/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "SubscriptionManager.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

SubscriptionManager::SubscriptionManager()
    : mSubNotification{nullptr}
    , mDisconnectNotification{nullptr}
    , mVps{SubscriptionManager::voidId}
    , mLog{SubscriptionManager::voidId}
    , mBootd{SubscriptionManager::voidId}
{
}

void SubscriptionManager::setSubscriptionNotification(ISubscriptionNotification* notification)
{
    mSubNotification = notification;
}

void SubscriptionManager::setDisconnectNotification(IDisconnectNotification* disconnectNotification)
{
    mDisconnectNotification = disconnectNotification;
}

void SubscriptionManager::add(int clientID)
{
    MLOGI(LogFuncId::VPD, LogId::SubscriptionManager_add_Adding_new_subscriber_client_id, clientID);
    std::vector<int>::iterator it;
    for (it = mSubscribers.begin(); it != mSubscribers.end(); ++it) {
        if (*it == clientID) {
            return;
        }
    }
    mSubscribers.push_back(clientID);

    if (mSubNotification) {
        mSubNotification->onSubscribeNative(clientID);
    }
    else {
        MLOGW(LogFuncId::VPD, LogId::SubscriptionManager_add_Subscription_Notification_is_not_set);
    }
}

std::vector<int> SubscriptionManager::getNativeSubscribers() const
{
    std::vector<int> nativeSubscribers;
    const int vps_id = getVpsID();
    const int log_id = getLogID();
    const int bootdaemon_id = getBootDaemonID();
    std::copy_if(mSubscribers.begin(), mSubscribers.end(), std::back_inserter(nativeSubscribers),
                 [&](int subscriber) {
                     return subscriber != vps_id && subscriber != log_id &&
                            subscriber != bootdaemon_id;
                 });
    return nativeSubscribers;
}

void SubscriptionManager::remove(int id)
{
    MLOGI(LogFuncId::VPD, LogId::SubscriptionManager_remove_Removing_subscriber_client_id, id);
    std::vector<int>::iterator it;
    for (it = mSubscribers.begin(); it != mSubscribers.end(); ++it) {
        if (*it == id) {
            mSubscribers.erase(it);
            break;
        }
    }
}

int SubscriptionManager::getVpsID() const { return mVps; }

void SubscriptionManager::setVpsID(int vpsClientID)
{
    MLOGW(LogFuncId::VPD, LogId::SubscriptionManager_setVpsID_Set_VPS_id_to, vpsClientID);
    if (mVps != SubscriptionManager::voidId && vpsClientID != SubscriptionManager::voidId) {
        MLOGI(LogFuncId::VPD, LogId::SubscriptionManager_setVpsID_VPS_subscription_was_already_set);
    }
    mVps = vpsClientID;

    if (vpsClientID != SubscriptionManager::voidId) {
        if (mSubNotification) {
            mSubNotification->onSubscribeVps(vpsClientID);
        }
        else {
            MLOGW(LogFuncId::VPD,
                  LogId::SubscriptionManager_setVpsID_Subscription_Notification_is_not_set);
        }
    }
}

int SubscriptionManager::getLogID() const { return mLog; }

void SubscriptionManager::setLogID(int logClientID)
{
    if (mLog != SubscriptionManager::voidId && logClientID != SubscriptionManager::voidId) {
        MLOGI(LogFuncId::VPD,
              LogId::SubscriptionManager_setLogID_Logdog_subscription_was_already_set);
    }
    mLog = logClientID;
}

int SubscriptionManager::getBootDaemonID() const { return mBootd; }

void SubscriptionManager::setBootDaemonID(int bootDaemonClientID)
{
    if (mBootd != SubscriptionManager::voidId &&
        bootDaemonClientID != SubscriptionManager::voidId) {
        MLOGI(LogFuncId::VPD,
              LogId::SubscriptionManager_setLogID_Bootdaemon_subscription_was_already_set);
    }
    mBootd = bootDaemonClientID;
}

void SubscriptionManager::disconnect(int clientID)
{
    MLOGI(LogFuncId::VPD,
          LogId::SubscriptionManager_disconnect_Subscriber_has_disconnected_client_id, clientID);
    remove(clientID);
    if (clientID == mVps) {
        if (mDisconnectNotification) {
            mDisconnectNotification->onDisconnectVps();
        }
        setVpsID(voidId);
    }
    else if (clientID == mLog) {
        setLogID(voidId);
    }
    else if (clientID == mBootd) {
        setBootDaemonID(voidId);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
