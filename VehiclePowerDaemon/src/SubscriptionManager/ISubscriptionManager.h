/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCR_MAN_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCR_MAN_H_

#include <cstddef>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Interface for SubscriptionManager
 */

class ISubscriptionManager {
public:
    // LCOV_EXCL_START
    virtual ~ISubscriptionManager() = default;
    // LCOV_EXCL_START
    /**
     * Add native subscriber by passing its clientID
     */
    virtual void add(int clientID) = 0;
    /**
     * Retrieve native subscriber by index (in order of adding)
     */
    virtual std::vector<int> getNativeSubscribers() const = 0;
    /**
     * Remove native subscriber by ID
     */
    virtual void remove(int id) = 0;
    /**
     * Retrieve VPS clientID
     */
    virtual int getVpsID() const = 0;
    /**
     * Register VPS clientID
     */
    virtual void setVpsID(int vpsClientID) = 0;
    /**
     * Retrieve Logdog clientID
     */
    virtual int getLogID() const = 0;
    /**
     * Register Logdog clientID
     */
    virtual void setLogID(int vpsClientID) = 0;
    /**
     * Retrieve Logdog clientID
     */
    virtual int getBootDaemonID() const = 0;
    /**
     * Register Logdog clientID
     */
    virtual void setBootDaemonID(int bootDaemonClientID) = 0;
    /**
     * Remove subscriber by ID
     */
    virtual void disconnect(int id) = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SUBSCR_MAN_H_
