/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSFWRESUMECOMPLETEHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSFWRESUMECOMPLETEHANDLER_H_

#include "IEventHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IFwResumeCompleteNotification;

/**
 * @brief The VpsFwResumeCompleteHandler class is intended for handle
 * notification from VPS that fw resume is complete
 */
class VpsFwResumeCompleteHandler : public IEventHandler {
public:
    VpsFwResumeCompleteHandler(IFwResumeCompleteNotification&);

    void Process(int, common::EVpdEvent) override;

private:
    IFwResumeCompleteNotification& mIFwResumeCompleteNotification;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSFWRESUMECOMPLETEHANDLER_H_
