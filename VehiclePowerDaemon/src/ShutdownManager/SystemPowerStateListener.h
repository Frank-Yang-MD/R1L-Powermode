/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_SHUTDOWNCANCELLISTENER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_SHUTDOWNCANCELLISTENER_H_

#include "IPowerStateChangeListener.h"
#include "ITimerManager.h"
#include "IVcpuActions.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * This class change onTimeout event handler depending on system power state
 */
class SystemPowerStateListener : public IPowerStateChangeListener {
public:
    SystemPowerStateListener(IVcpuActions&, ITimerManager&);
    void onPowerStateChange(PowerState) override;

private:
    IVcpuActions& mVcpuActions;
    ITimerManager& mTimerManager;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_SHUTDOWNCANCELLISTENER_H_
