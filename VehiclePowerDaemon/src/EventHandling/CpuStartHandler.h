/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CPUSTARTHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CPUSTARTHANDLER_H_

#include "IEventHandler.h"
#include "IStartCompleteCpuCom.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This adapter handles events of CPU Start
 */
class CpuStartHandler final : public IEventHandler {
public:
    CpuStartHandler(IStartCompleteCpuCom& inIStartCompleteCpuCom);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    IStartCompleteCpuCom& mIStartCompleteCpuCom;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CPUSTARTHANDLER_H_
