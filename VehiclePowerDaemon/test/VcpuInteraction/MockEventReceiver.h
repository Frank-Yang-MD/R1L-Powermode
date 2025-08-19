/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKEVENTRECEIVER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKEVENTRECEIVER_H_

#include <IEventReceiver.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockEventReceiver : public IEventReceiver {
public:
    MOCK_METHOD2(onEventReceive, void(int, common::EVpdEvent));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKEVENTRECEIVER_H_
