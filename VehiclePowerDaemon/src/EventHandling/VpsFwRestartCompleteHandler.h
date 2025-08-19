/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSFWRESTARTCOMPLETEHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSFWRESTARTCOMPLETEHANDLER_H_

#include "IEventHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IFwRestartCompleteNotification;

/**
 * @brief The VpsFwRestartCompleteHandler class is intended for handle
 * notification from VPS that fw restart is complete
 */
class VpsFwRestartCompleteHandler : public IEventHandler {
public:
    VpsFwRestartCompleteHandler(IFwRestartCompleteNotification&);

    void Process(int, common::EVpdEvent) override;

private:
    IFwRestartCompleteNotification& mIFwRestartCompleteNotification;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSFWRESTARTCOMPLETEHANDLER_H_
