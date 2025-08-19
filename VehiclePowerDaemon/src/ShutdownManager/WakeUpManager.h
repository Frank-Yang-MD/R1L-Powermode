/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAKEUPMANAGER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAKEUPMANAGER_H_

#include "IAndroidPowerManager.h"
#include "IWakeUpManager.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WakeUpManager : public IWakeUpManager {
public:
    WakeUpManager(IAndroidPowerManager&);
    virtual ~WakeUpManager() = default;

    virtual void wakeUp() override;

private:
    IAndroidPowerManager& mAndroidPowerManager;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAKEUPMANAGER_H_
