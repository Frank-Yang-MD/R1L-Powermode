/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPSSTARTHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPSSTARTHANDLER_H_

#include "IEventHandler.h"
#include "IStartCompleteVps.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This adapter handles events of Vps Start
 */
class VpsStartHandler final : public IEventHandler {
public:
    VpsStartHandler(IStartCompleteVps& inIStartCompleteVps);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    IStartCompleteVps& mIStartCompleteVps;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPSSTARTHANDLER_H_
