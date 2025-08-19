/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CLIENTACTIONS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CLIENTACTIONS_H_

#include "EVpdState.h"
#include "IClientActions.h"
#include "IStateSender.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ClientActions : public IClientActions {
public:
    ClientActions(IStateSender& stateSender);
    void notifySystemStateChange(int clientID, common::EVpdState state) override;

private:
    IStateSender& mStateSender;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CLIENTACTIONS_H_
