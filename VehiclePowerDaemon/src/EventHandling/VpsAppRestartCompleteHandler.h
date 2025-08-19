/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSAPPRESTARTCOMPLETEHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSAPPRESTARTCOMPLETEHANDLER_H_

#include "IEventHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IAppRestartCompleteNotification;

/**
 * @brief The VpsAppRestartCompleteHandler class is intended for handle
 * notification from VPS that app restart is complete
 */
class VpsAppRestartCompleteHandler : public IEventHandler {
public:
    VpsAppRestartCompleteHandler(IAppRestartCompleteNotification&);

    void Process(int, common::EVpdEvent) override;

private:
    IAppRestartCompleteNotification& mIAppRestartCompleteNotification;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPSAPPRESTARTCOMPLETEHANDLER_H_
