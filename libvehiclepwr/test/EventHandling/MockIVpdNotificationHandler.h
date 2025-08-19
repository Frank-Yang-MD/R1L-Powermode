/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKIVPDNOTIFICATIONHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKIVPDNOTIFICATIONHANDLER_H_

#include <gmock/gmock.h>

#include "IVpdNotificationHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class MockIVpdNotificationHandler : public IVpdNotificationHandler {
public:
    MOCK_METHOD1(onVpdNotification, bool(common::EVpdNotifications));
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKIVPDNOTIFICATIONHANDLER_H_
