/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKWAKEUPMANAGER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKWAKEUPMANAGER_H

#include <gmock/gmock.h>

#include <IWakeUpManager.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockWakeUpManager : public IWakeUpManager {
public:
    MOCK_METHOD0(wakeUp, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKWAKEUPMANAGER_H
