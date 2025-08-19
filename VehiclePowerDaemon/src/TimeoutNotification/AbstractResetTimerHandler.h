/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ABSTRACTRESETTIMERHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ABSTRACTRESETTIMERHANDLER_H_

#include "ITimer.h"
#include "IVcpuResetTimer.h"
#include <IVcpuActions.h>

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class AbstractResetTimerHandler : public IVcpuResetTimer {
public:
    AbstractResetTimerHandler(std::unique_ptr<ITimer>);

    virtual void start() override;
    virtual void onTimeout() override;

protected:
    virtual void performResetAction() = 0;

private:
    std::unique_ptr<ITimer> mTimer;
    bool mIsTimerStarted;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ABSTRACTRESETTIMERHANDLER_H_
