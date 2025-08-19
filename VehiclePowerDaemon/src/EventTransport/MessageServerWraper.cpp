/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

// LCOV_EXCL_START
// This is excluded from a unit test coverage report because this class
// only delegate all calls to MessageServer class

#include "MessageServerWraper.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

MessageServerWraper::MessageServerWraper(MessageServer<common::EVpdMsgId>& messageServer,
                                         IExecutor& executor)
    : mMessageServer{messageServer}
    , mExecutor{executor}
{
}

void MessageServerWraper::start()
{
    mExecutor.execute([this]() {
        if (!mMessageServer.start()) {
            MLOGE(LogFuncId::VPD, LogId::VpdMessageServer_start_failed);
        }
    });
}

void MessageServerWraper::stop() { mMessageServer.stop(); }

void MessageServerWraper::sendNotification(const SessionID sessionId,
                                           const common::EVpdNotifications notification)
{
    mMessageServer.sendMessage(sessionId, common::EVpdMsgId::Notification, notification);
}

void MessageServerWraper::initialize(std::function<void(SessionID)> connectHandler,
                                     std::function<void(SessionID)> disconnectHandler)
{
    mMessageServer.initialize(connectHandler, disconnectHandler);
}

void MessageServerWraper::setMessageHandler(
    std::function<void(SessionID, common::EVpdEvent, int)> handler)
{
    mMessageServer.setMessageHandler(common::EVpdMsgId::Event, handler);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

// LCOV_EXCL_STOP
