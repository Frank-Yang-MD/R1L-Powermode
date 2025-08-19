/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_SUBSCRIBEMANAGERMOCK_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_SUBSCRIBEMANAGERMOCK_H_

#include "ISubscriptionManager.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockISubscriptionManager : public ISubscriptionManager {
public:
    MOCK_METHOD(void, add, (int clientID));
    MOCK_METHOD(std::vector<int>, getNativeSubscribers, (), (const));
    MOCK_METHOD(void, remove, (int id));
    MOCK_METHOD(int, getVpsID, (), (const));
    MOCK_METHOD(void, setVpsID, (int vpsClientID));
    MOCK_METHOD(int, getLogID, (), (const));
    MOCK_METHOD(void, setLogID, (int vpsClientID));
    MOCK_METHOD(int, getBootDaemonID, (), (const));
    MOCK_METHOD(void, setBootDaemonID, (int bootDaemonClientID));
    MOCK_METHOD(void, disconnect, (int id));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_SUBSCRIBEMANAGERMOCK_H_
