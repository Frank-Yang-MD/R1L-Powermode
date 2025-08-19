/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSTATES_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSTATES_H_

#include "VpdStateBase.h"
#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class MockVpdStateBase : public VpdStateBase {
public:
    MockVpdStateBase(IVpdStateMachine* sm)
        : VpdStateBase(sm){};

    MOCK_METHOD0(onEnter, void());
    MOCK_METHOD0(onExit, void());

    MOCK_METHOD1(onSubscribeNative, void(int));
    MOCK_METHOD1(onUnsubscribeNative, void(int));
    MOCK_METHOD1(onStopCompleteNative, void(int));
    MOCK_METHOD0(onStartCompleteCpuCom, void());
    MOCK_METHOD1(onPowerStateChange, void(PowerState));
    MOCK_METHOD1(onSubscribeVps, void(int));
    MOCK_METHOD1(onUnsubscribeVps, void(int));
    MOCK_METHOD0(onStartCompleteVps, void());
    MOCK_METHOD0(onStartFailedVps, void());
    MOCK_METHOD0(onAppStopCompleteVps, void());
    MOCK_METHOD0(onFwStopCompleteVps, void());
    MOCK_METHOD0(onStopCompleteLog, void());
    MOCK_METHOD0(onWakeUp, void());
    MOCK_METHOD0(onTimeout, void());
    MOCK_METHOD0(onStopFailedVps, void());
    MOCK_METHOD0(onFwResumeComplete, void());
    MOCK_METHOD0(onFwRestartComplete, void());
    MOCK_METHOD0(onAppResumeComplete, void());
    MOCK_METHOD0(onAppRestartComplete, void());
    MOCK_METHOD0(onVpsUnmountComplete, void());
    MOCK_METHOD0(onShutdownProcessingStartSent, void());
    MOCK_METHOD0(onResumeProcessingStartCompleteSentSuccess, void());
    MOCK_METHOD0(onResumeProcessingStartCompleteSentFailure, void());
    MOCK_METHOD0(onDisconnectVps, void());

    MOCK_METHOD1(transitionTo, void(VpdStateBase*));
};

class MockSomeState : public MockVpdStateBase {
public:
    MockSomeState(IVpdStateMachine* sm)
        : MockVpdStateBase(sm){};
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_MOCKSTATES_H_
