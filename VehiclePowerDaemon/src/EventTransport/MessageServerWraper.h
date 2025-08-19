/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#pragma once

#include "EVpdMsgId.h"
#include "IMessageServer.h"

#include "libMelcoCommon.h"
#include <messenger/MessageServer.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using com::mitsubishielectric::ahu::common::IExecutor;
using com::mitsubishielectric::ahu::common::MessageServer;

class MessageServerWraper final : public IMessageServer {  // LCOV_EXCL_LINE
    // exclude all possible destructors, that cannot be covered by unit-test
public:
    MessageServerWraper(MessageServer<common::EVpdMsgId>& messageServer, IExecutor& executor);

    void start() override;
    void stop() override;

    void sendNotification(const SessionID, const common::EVpdNotifications) override;
    void initialize(std::function<void(SessionID)>, std::function<void(SessionID)>) override;
    void setMessageHandler(std::function<void(SessionID, common::EVpdEvent, int)>) override;

private:
    MessageServer<common::EVpdMsgId>& mMessageServer;
    IExecutor& mExecutor;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
