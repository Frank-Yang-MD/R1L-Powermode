/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SHUTDOWNMANAGER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SHUTDOWNMANAGER_H_

#include "IAndroidPowerManager.h"
#include "IPowerStateHolder.h"
#include "IShutdownAction.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ShutdownManager : public IShutdownAction {
public:
    ShutdownManager(IAndroidPowerManager&, IPowerStateHolder&);

    virtual void performShutdownAction() override;

private:
    IAndroidPowerManager& mAndroidPowerManager;
    IPowerStateHolder& mPowerStateHolder;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SHUTDOWNMANAGER_H_
