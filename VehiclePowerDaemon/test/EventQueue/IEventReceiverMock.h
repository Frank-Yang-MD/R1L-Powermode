/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IEVENTRECEIVERMOCK_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IEVENTRECEIVERMOCK_H_

#include <gmock/gmock.h>

#include "IEventReceiver.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IEventReceiverMock : public IEventReceiver {
public:
    MOCK_METHOD2(onEventReceive, void(int, common::EVpdEvent));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IEVENTRECEIVERMOCK_H_
