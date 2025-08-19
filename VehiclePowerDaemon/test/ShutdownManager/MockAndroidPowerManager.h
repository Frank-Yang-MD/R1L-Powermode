/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKANDROIDPOWERMANAGER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKANDROIDPOWERMANAGER_H

#include <gmock/gmock.h>

#include <IAndroidPowerManager.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockAndroidPowerManager : public IAndroidPowerManager {
public:
    MOCK_METHOD0(shutdown, void());
    MOCK_METHOD0(suspend, void());
    MOCK_METHOD0(reboot, void());
    MOCK_METHOD0(rebootRecovery, void());
    MOCK_METHOD0(wakeUp, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKANDROIDPOWERMANAGER_H
