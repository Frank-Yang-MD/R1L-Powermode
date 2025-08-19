/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTHANDLINGMOCKS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTHANDLINGMOCKS_H_

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "EventQueue.h"
#include <IResumeProcessingStartCompleteSent.h>
#include <IShutdownProcessingStartSent.h>
#include <IStartCompleteCpuCom.h>
#include <IStartCompleteVps.h>
#include <IStopCompleteLog.h>
#include <IStopCompleteNative.h>
#include <IStopCompleteVps.h>
#include <ISubscriptionManager.h>
#include <ITimeoutNotification.h>
#include <IVpsMount.h>
#include <PowerStateHolder.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 *
 * This file containe Mock classes nedded for unit test.
 * Module EventHandling
 *
 */

class MockStartCompleteCpuCom : public IStartCompleteCpuCom {
public:
    MOCK_METHOD0(onStartCompleteCpuCom, void());
};

class MockStartCompleteVps : public IStartCompleteVps {
public:
    MOCK_METHOD0(onStartCompleteVps, void());
    MOCK_METHOD0(onStartFailedVps, void());
};

class MockStopCompleteNative : public IStopCompleteNative {
public:
    MOCK_METHOD1(onStopCompleteNative, void(int));
};

class MockStopCompleteLog : public IStopCompleteLog {
public:
    MOCK_METHOD0(onStopCompleteLog, void());
};

class MockTimeoutNotification : public ITimeoutNotification {
public:
    MOCK_METHOD0(onTimeout, void());
};

class MockSubscriptionManager : public ISubscriptionManager {
public:
    MOCK_METHOD1(add, void(int clientId));
    MOCK_CONST_METHOD0(getNativeSubscribers, std::vector<int>());
    MOCK_METHOD1(remove, void(int id));
    MOCK_CONST_METHOD0(getVpsID, int(void));
    MOCK_METHOD1(setVpsID, void(int clientId));
    MOCK_CONST_METHOD0(getLogID, int(void));
    MOCK_METHOD1(setLogID, void(int clientId));
    MOCK_CONST_METHOD0(getBootDaemonID, int(void));
    MOCK_METHOD1(setBootDaemonID, void(int clientId));
    MOCK_METHOD1(disconnect, void(int));
};

class MockStopCompleteVps : public IStopCompleteVps {
public:
    MOCK_METHOD0(onAppStopCompleteVps, void());
    MOCK_METHOD0(onFwStopCompleteVps, void());
    MOCK_METHOD0(onStopFailedVps, void());
};

class MockEventSource : public IEventSource {
public:
    MOCK_METHOD0(getEvent, std::pair<int, common::EVpdEvent>());
};

class MockVpsMount : public IVpsMount {
public:
    MOCK_METHOD0(onVpsUnmountComplete, void());
};

class MockShutdownProcessingStartSent : public IShutdownProcessingStartSent {
public:
    MOCK_METHOD0(onShutdownProcessingStartSent, void());
};

class MockResumeProcessingStartCompleteSent : public IResumeProcessingStartCompleteSent {
public:
    MOCK_METHOD0(onResumeProcessingStartCompleteSentSuccess, void());
    MOCK_METHOD0(onResumeProcessingStartCompleteSentFailure, void());
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVENTHANDLINGMOCKS_H_
