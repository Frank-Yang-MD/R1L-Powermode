/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAKEUPHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAKEUPHANDLER_H_

#include "IEventHandler.h"
#include "IWakeUpNotification.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This handles wake up event from kernel
 */
class WakeUpHandler final : public IEventHandler {
public:
    WakeUpHandler(IWakeUpNotification& wakeUpNotification);

    void Process(int, common::EVpdEvent) override;

private:
    IWakeUpNotification& mWakeUpNotification;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAKEUPHANDLER_H_
