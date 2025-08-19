/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPDMESSAGESERVER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPDMESSAGESERVER_H_

#include "IEventReceiver.h"
#include "IVpdNotificationSender.h"

#include "EVpdMsgId.h"

#include <Executors.h>
#include <messenger/MessageServer.h>

#include <memory>
#include <mutex>
#include <unordered_map>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using com::mitsubishielectric::ahu::common::IExecutor;
using com::mitsubishielectric::ahu::common::MessageServer;
using com::mitsubishielectric::ahu::common::SingleThreadExecutor;

using SessionID = MessageServer<common::EVpdMsgId>::SessionID;

class IMessageServer;

class VpdMessageServer final : public IVpdNotificationSender {
public:
    VpdMessageServer(IEventReceiver&, IMessageServer&);
    ~VpdMessageServer();

    void start();

    virtual void sendNotification(int, common::EVpdNotifications) override;

private:
    void onClientConnected(SessionID);
    void onClientDisconnected(SessionID);
    void onEventReceived(SessionID, common::EVpdEvent, int);

private:
    IEventReceiver& mEventReceiver;
    IMessageServer& mMessageServer;

    std::thread mServerThread;
    std::mutex mMutex;
    std::unordered_map<int, SessionID> mPidToSessionId;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPDMESSAGESERVER_H_
