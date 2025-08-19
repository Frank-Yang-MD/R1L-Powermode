/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpdMessageServer.h"

#include "VehiclePowerDaemonLog.h"

#include "IMessageServer.h"

#include "VpdConst.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

VpdMessageServer::VpdMessageServer(IEventReceiver& eventReceiver, IMessageServer& messageServer)
    : mEventReceiver{eventReceiver}
    , mMessageServer{messageServer}
{
}

VpdMessageServer::~VpdMessageServer() { mMessageServer.stop(); }

void VpdMessageServer::start()
{
    using namespace std::placeholders;

    mMessageServer.initialize(std::bind(&VpdMessageServer::onClientConnected, this, _1),
                              std::bind(&VpdMessageServer::onClientDisconnected, this, _1));

    mMessageServer.setMessageHandler(
        std::bind(&VpdMessageServer::onEventReceived, this, _1, _2, _3));

    mMessageServer.start();
}

void VpdMessageServer::sendNotification(int pid, common::EVpdNotifications vpdNotification)
{
    SessionID clientSessionId;

    {
        std::lock_guard<std::mutex> lock{mMutex};

        if (mPidToSessionId.count(pid)) {
            clientSessionId = mPidToSessionId[pid];
        }
        else {
            MLOGW(LogFuncId::VPD, LogId::VpdMessageServer_unknown_pid,
                  static_cast<int>(vpdNotification), pid);
            return;
        }
    }

    mMessageServer.sendNotification(clientSessionId, vpdNotification);
}  // LCOV_EXCL_LINE

void VpdMessageServer::onClientConnected(SessionID) {}

void VpdMessageServer::onClientDisconnected(SessionID sessionId)
{
    int disconnectedPid = 0;

    std::unique_lock<std::mutex> lock{mMutex};

    for (auto it = cbegin(mPidToSessionId); it != cend(mPidToSessionId); ++it) {
        if (it->second == sessionId) {
            disconnectedPid = it->first;
            mPidToSessionId.erase(it);
            break;
        }
    }

    lock.unlock();

    if (disconnectedPid) {
        mEventReceiver.onEventReceive(disconnectedPid, common::EVpdEvent::Disconnect);
    }
}

void VpdMessageServer::onEventReceived(SessionID sessionId, common::EVpdEvent vpdEvent, int pid)
{
    {
        std::lock_guard<std::mutex> lock{mMutex};

        if (!mPidToSessionId.count(pid)) {
            mPidToSessionId[pid] = sessionId;
        }
    }

    mEventReceiver.onEventReceive(pid, vpdEvent);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
