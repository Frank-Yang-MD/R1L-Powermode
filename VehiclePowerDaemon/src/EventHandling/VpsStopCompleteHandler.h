/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPSSTOPCOMPLETEHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPSSTOPCOMPLETEHANDLER_H_

#include "IEventHandler.h"
#include "IStopCompleteVps.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This adapter handles events of Vps Stop Complete
 */
class VpsStopCompleteHandler final : public IEventHandler {
public:
    VpsStopCompleteHandler(IStopCompleteVps& inIStopCompleteVps);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    IStopCompleteVps& mIStopCompleteVps;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPSSTOPCOMPLETEHANDLER_H_
