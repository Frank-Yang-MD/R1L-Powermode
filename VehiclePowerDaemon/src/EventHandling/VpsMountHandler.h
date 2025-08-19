/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPSMOUNTHANDLER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPSMOUNTHANDLER_H

#include "IEventHandler.h"
#include "IVpsMount.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This adapter handles mount events of Vps
 */
class VpsMountHandler final : public IEventHandler {
public:
    VpsMountHandler(IVpsMount& inIVpsMount);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    IVpsMount& mIVpsMount;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_VPSMOUNTHANDLER_H
