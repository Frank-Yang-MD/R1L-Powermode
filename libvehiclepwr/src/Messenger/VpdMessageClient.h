/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPDMESSAGECLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPDMESSAGECLIENT_H_

#include "IDataReceiverEx.h"
#include "IVpdEventSender.h"

#include "EVpdMsgId.h"
#include "EVpdNotifications.h"

#include <UUID.h>
#include <messenger/Messenger.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using com::mitsubishielectric::ahu::common::Messenger;
using com::mitsubishielectric::ahu::common::UUID;

class VpdMessageClient final : public IVpdEventSender {
public:
    VpdMessageClient(IDataReceiverEx&);

    void start();

    virtual void sendVpdEvent(common::EVpdEvent inEVpdEvent) override;

private:
    void onVpdNotificationReceived(common::EVpdNotifications);

private:
    IDataReceiverEx& mDataReceiverEx;
    Messenger<common::EVpdMsgId> mMessenger;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_VPDMESSAGECLIENT_H_
