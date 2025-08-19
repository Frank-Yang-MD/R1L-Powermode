/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#pragma once

#include "EVpdEvent.h"
#include "EVpdMsgId.h"
#include "EVpdNotifications.h"
#include <messenger/MessageServer.h>

#include <functional>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using com::mitsubishielectric::ahu::common::MessageServer;
using SessionID = MessageServer<common::EVpdMsgId>::SessionID;

class IMessageServer {
public:
    virtual void start() = 0;
    virtual void stop() = 0;
    virtual void sendNotification(const SessionID, const common::EVpdNotifications) = 0;
    virtual void initialize(std::function<void(SessionID)>, std::function<void(SessionID)>) = 0;
    virtual void setMessageHandler(std::function<void(SessionID, common::EVpdEvent, int)>) = 0;

    // LCOV_EXCL_START
    virtual ~IMessageServer() = default;
    // LCOV_EXCL_STOP
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
