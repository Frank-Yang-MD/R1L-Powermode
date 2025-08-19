/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

// LCOV_EXCL_START
// This is excluded from a unit test coverage report because it is hard to test.

#include "VpdMessageClient.h"

#include "LibVehiclePwrLog.h"

#include "EVpdEvent.h"
#include "VpdConst.h"

#include <Executors.h>
#include <Socket.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using com::mitsubishielectric::ahu::common::PausableSingleThreadExecutor;
using com::mitsubishielectric::ahu::common::SingleThreadExecutor;
using com::mitsubishielectric::ahu::common::Socket;

VpdMessageClient::VpdMessageClient(IDataReceiverEx& dataReceiverEx)
    : mDataReceiverEx{dataReceiverEx}
    , mMessenger{common::SERVER_NAME, std::make_unique<Socket>(),
                 std::make_unique<PausableSingleThreadExecutor>(),
                 std::make_unique<SingleThreadExecutor>(), true}
{
    auto onConnectionClosedHandler = [this]() { mDataReceiverEx.onDisconnect(); };

    auto onConnectionResumedHandler = [this]() { mDataReceiverEx.onConnect(); };

    mMessenger.initialize(onConnectionClosedHandler, onConnectionResumedHandler);

    mMessenger.setMessageHandler(common::EVpdMsgId::Notification,
                                 &VpdMessageClient::onVpdNotificationReceived, this);
}

void VpdMessageClient::start()
{
    if (mMessenger.connect()) {
        mDataReceiverEx.onConnect();
    }
    else {
        MLOGE(LogFuncId::VPL, LogId::VpdMessageClient_could_not_connect);
    }
}

void VpdMessageClient::sendVpdEvent(common::EVpdEvent vpdEvent)
{
    mMessenger.sendMessage(common::EVpdMsgId::Event, vpdEvent, getpid()).wait();
}

void VpdMessageClient::onVpdNotificationReceived(common::EVpdNotifications vpdNotification)
{
    mDataReceiverEx.onNotificationReceived(vpdNotification);
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

// LCOV_EXCL_STOP
