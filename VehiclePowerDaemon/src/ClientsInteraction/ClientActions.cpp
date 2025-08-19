/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ClientActions.h"
#include "IStateSender.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

ClientActions::ClientActions(IStateSender& stateSender)
    : mStateSender(stateSender)
{
}

void ClientActions::notifySystemStateChange(int clientID, common::EVpdState state)
{
    MLOGI(LogFuncId::VPD, LogId::ClientActions_notifySystemStateChange_client_id_state, clientID,
          static_cast<int>(state));
    mStateSender.sendState(clientID, state);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
