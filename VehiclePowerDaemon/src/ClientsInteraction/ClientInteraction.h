/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CLIENTINTERACTION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CLIENTINTERACTION_H_

#include "ClientActions.h"
#include "EVpdState.h"
#include "IVpdNotificationSender.h"
#include "StateSender.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ClientInteraction {
public:
    ClientInteraction(IVpdNotificationSender&);

    IClientActions& getClientActions();

private:
    StateSender mStateSender;
    ClientActions mClientActions;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CLIENTINTERACTION_H_
