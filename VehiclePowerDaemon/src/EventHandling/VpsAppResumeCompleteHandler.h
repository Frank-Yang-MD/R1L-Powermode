/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSAPPRESUMECOMPLETEHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSAPPRESUMECOMPLETEHANDLER_H_

#include "IEventHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IAppResumeCompleteNotification;

/**
 * @brief The VpsAppResumeCompleteHandler class is intended for handle
 * notification from VPS that app resume is complete
 */
class VpsAppResumeCompleteHandler : public IEventHandler {
public:
    VpsAppResumeCompleteHandler(IAppResumeCompleteNotification&);

    void Process(int, common::EVpdEvent) override;

private:
    IAppResumeCompleteNotification& mIAppResumeCompleteNotification;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSAPPRESUMECOMPLETEHANDLER_H_
