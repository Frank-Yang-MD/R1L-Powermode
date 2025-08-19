/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ISubscriptionNotification.h"
#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockSubscriptionNotification : public ISubscriptionNotification {
public:
    MOCK_METHOD1(onSubscribeNative, void(int));
    MOCK_METHOD1(onSubscribeVps, void(int));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
