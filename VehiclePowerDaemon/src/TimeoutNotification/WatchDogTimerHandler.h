/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WATCHDOGTIMERHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WATCHDOGTIMERHANDLER_H_

#include "AbstractResetTimerHandler.h"
#include "ITimer.h"
#include "IVcpuActions.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WatchDogTimerHandler : public AbstractResetTimerHandler {
public:
    WatchDogTimerHandler(std::unique_ptr<ITimer>, IVcpuActions&);

private:
    IVcpuActions& mVcpuActions;
    virtual void performResetAction() override;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WATCHDOGTIMERHANDLER_H_
