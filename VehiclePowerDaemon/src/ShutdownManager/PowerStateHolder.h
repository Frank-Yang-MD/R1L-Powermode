/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_POWERSTATEHOLDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_POWERSTATEHOLDER_H_

#include "IPowerStateHolder.h"

#include <ITimeoutNotification.h>
#include <ITimerCreator.h>
#include <IVcpuActions.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * @brief The PowerStateHolder class is responsible for holding current
 * power state, determine whether current power state is shudown state or not
 * and reset corresponding VCPU time depending or current power state.
 */
class PowerStateHolder : public IPowerStateHolder {
public:
    explicit PowerStateHolder();

    virtual void onPowerStateChange(PowerState) override;

    virtual bool isShutdownState() const override;
    virtual PowerState getPowerState() const override;

private:
    PowerState mPowerState;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_POWERSTATEHOLDER_H_
