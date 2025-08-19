/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKVPDNOTIFICATIONSENDER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKVPDNOTIFICATIONSENDER_H

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "EVpdNotifications.h"
#include "IVpdNotificationSender.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockVpdNotificationSender : public IVpdNotificationSender {
public:
    MOCK_METHOD2(sendNotification, void(int, common::EVpdNotifications));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKVPDNOTIFICATIONSENDER_H
