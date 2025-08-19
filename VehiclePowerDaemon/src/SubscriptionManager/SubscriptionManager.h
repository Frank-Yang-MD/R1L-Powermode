/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCR_MAN_IMPL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCR_MAN_IMPL_H_

#include "IDisconnectNotification.h"
#include "ISubscriptionManager.h"
#include "ISubscriptionNotification.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class SubscriptionManager : public ISubscriptionManager {
public:
    SubscriptionManager();

    void setSubscriptionNotification(ISubscriptionNotification* notification);
    void setDisconnectNotification(IDisconnectNotification* disconnectNotification);
    void add(int clientID) override;
    std::vector<int> getNativeSubscribers() const override;
    void remove(int clientID) override;
    int getVpsID() const override;
    void setVpsID(int vpsClientID) override;
    int getLogID() const override;
    void setLogID(int vpsClientID) override;
    int getBootDaemonID() const override;
    void setBootDaemonID(int bootDaemonClientID) override;
    void disconnect(int clientID) override;

    static const int voidId = -1;  //!< An invalid subscriber's descriptor

private:
    /**
     * Subscription Notification object which must be passed in ctr
     */
    ISubscriptionNotification* mSubNotification;

    IDisconnectNotification* mDisconnectNotification;
    /**
     * Internal structure to keep track of native subscribers
     */
    std::vector<int> mSubscribers;
    int mVps;    //!< Variable holding VPS descriptor
    int mLog;    //!< Variable holding Logdog descriptor
    int mBootd;  //!< Variable holding Logdog descriptor
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCR_MAN_IMPL_H_
