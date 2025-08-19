/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKNOTIFYHELPER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKNOTIFYHELPER_H_

#include <gmock/gmock.h>

#include "INotifyHelper.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockNotifyHelper : public INotifyHelper {
public:
    MOCK_METHOD1(notifyAllNative, void(common::EVpdState vpdState));
    MOCK_METHOD1(notifyVps, void(common::EVpdState vpdState));
    MOCK_METHOD1(notifyVps, void(common::EVpdNotifications vpdNotification));
    MOCK_METHOD1(notifyLog, void(common::EVpdState vpdState));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKNOTIFYHELPER_H_
