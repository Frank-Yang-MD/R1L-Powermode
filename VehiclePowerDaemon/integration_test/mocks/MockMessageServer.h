/*
 * COPYRIGHT (C) 2022 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#pragma once

#include "IMessageServer.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockMessageServer : public IMessageServer {
public:
    MOCK_METHOD(void, start, ());
    MOCK_METHOD(void, stop, ());
    MOCK_METHOD(void, sendNotification, (const SessionID, const common::EVpdNotifications));
    MOCK_METHOD(void, initialize, (std::function<void(SessionID)>, std::function<void(SessionID)>));
    MOCK_METHOD(void, setMessageHandler, (std::function<void(SessionID, common::EVpdEvent, int)>));
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
