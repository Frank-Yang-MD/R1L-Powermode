/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSUBSCRIPTIONNOTIFICATION_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSUBSCRIPTIONNOTIFICATION_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "ISubscriptionNotification.h"

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
#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSUBSCRIPTIONNOTIFICATION_H
