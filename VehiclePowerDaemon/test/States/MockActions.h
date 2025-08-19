/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKACTIONS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKACTIONS_H_

#include <gmock/gmock.h>

#include "IClientActions.h"
#include "ILogDaemonActions.h"
#include "IShutdownAction.h"
#include "IVcpuActions.h"
#include "IWakeLockManager.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockWakeLockManager : public IWakeLockManager {
public:
    MOCK_METHOD0(acquireWakeLock, void());
    MOCK_METHOD0(releaseWakeLock, void());
    MOCK_METHOD0(enableWakeLocks, void());
    MOCK_METHOD0(disableWakeLocks, void());
};

class MockShutdownAction : public IShutdownAction {
public:
    MOCK_METHOD0(performShutdownAction, void());
};

class MockLogDaemonActions : public ILogDaemonActions {
public:
    MOCK_METHOD0(isLogSavingComplete, bool(void));
};

class MockClientActions : public IClientActions {
public:
    MOCK_METHOD2(notifySystemStateChange, void(int, common::EVpdState));
};

class MockVcpuActions : public IVcpuActions {
public:
    MOCK_METHOD0(sendAdtrRequest, void());
    MOCK_METHOD0(sendStartComplete, void());
    MOCK_METHOD0(sendShutdownCancelAccept, void());
    MOCK_METHOD0(sendShutdownProcessingStart, void());
    MOCK_METHOD0(sendWdtrRequest, void());
    MOCK_METHOD0(sendSelfRebootTrue, void());
    MOCK_METHOD0(sendMainStartComplete, void());
    MOCK_METHOD0(sendShutdownCancelComplete, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKACTIONS_H_
