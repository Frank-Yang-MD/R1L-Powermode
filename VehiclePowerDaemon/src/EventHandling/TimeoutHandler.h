/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMEOUTHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMEOUTHANDLER_H_

#include "IEventHandler.h"
#include "ITimeoutNotification.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This adapter handles events of Timeout
 */
class TimeoutHandler final : public IEventHandler {
public:
    TimeoutHandler(ITimeoutNotification&, ITimeoutNotification&);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    ITimeoutNotification& mStateMachine;
    ITimeoutNotification& mResetTimerHandler;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_TIMEOUTHANDLER_H_
