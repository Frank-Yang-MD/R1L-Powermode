/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "WaitNativeAppStopProcessingComplete.h"
#include "LogDaemonBusy.h"
#include "VehiclePowerDaemonLog.h"
#include "WaitForLogSaving.h"

#include <algorithm>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using timeout_t = std::chrono::microseconds;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;
using namespace std;

std::unique_ptr<VpdStateBase> WaitNativeAppStopProcessingComplete::sInstance =
    std::unique_ptr<VpdStateBase>();

void WaitNativeAppStopProcessingComplete::initInstance(IVpdStateMachine* sm,
                                                       INotifyHelper& notifyHelper,
                                                       ITimer& timer,
                                                       ISubscriptionManager& subscriptionManager,
                                                       IStateHolder& sh,
                                                       IVcpuActions& va)
{
    sInstance.reset(new WaitNativeAppStopProcessingComplete(sm, notifyHelper, timer,
                                                            subscriptionManager, sh, va));
}

WaitNativeAppStopProcessingComplete::WaitNativeAppStopProcessingComplete(
    IVpdStateMachine* sm,
    INotifyHelper& notifyHelper,
    ITimer& timer,
    ISubscriptionManager& subscriptionManager,
    IStateHolder& sh,
    IVcpuActions& va)
    : VpdStateBase(sm)
    , mNotifyHelper{notifyHelper}
    , mSubscriptionManager{subscriptionManager}
    , mTimer{timer}
    , mStopCompleteCount{0}
    , mStateHolder{sh}
    , mVcpuActions{va}
{
}

void WaitNativeAppStopProcessingComplete::onEnter()
{
    mStopCompleteCount = 0;
    mStopCompleteAppIds.clear();
    int subscribersOnStartCount = getNumberOfNativeSubscribers();
    MLOGI(LogFuncId::VPD, LogId::WaitNativeAppStopProcessingComplete_onEnter_Number_of_subscribers,
          subscribersOnStartCount);

    mNotifyHelper.notifyAllNative(EVpdState::APP_STOP);
    mStateHolder.setVpdState(EVpdState::APP_STOP);

    if (subscribersOnStartCount > 0) {
        // Setup Timeout
        mTimer.start();
    }
    else {
        // Force transition to next state
        // if number of native subscribers equals 0
        sInstance->transitionTo(LogDaemonBusy::getInstance());
    }
}

void WaitNativeAppStopProcessingComplete::onTimeout()
{
    MLOGI(LogFuncId::VPD, LogId::WaitNativeAppStopProcessingComplete_onTimeout);
    if (mTimer.isExpired()) {
        MLOGW(LogFuncId::VPD, LogId::WaitNativeAppStopProcessingComplete_Timer_Is_Expired);
        logNoStopCompletes();
        mVcpuActions.sendSelfRebootTrue();
        sInstance->transitionTo(WaitForLogSaving::getInstance());
    }
}

void WaitNativeAppStopProcessingComplete::onStopCompleteNative(int id)
{
    mStopCompleteCount++;
    mStopCompleteAppIds.emplace(id);
    MLOGI(
        LogFuncId::VPD,
        LogId::
            WaitNativeAppStopProcessingComplete_onStopCompleteNative_Number_of_stopped_subscribers,
        mStopCompleteCount);
    if (mStopCompleteCount == getNumberOfNativeSubscribers()) {
        sInstance->transitionTo(LogDaemonBusy::getInstance());
    }
}

int WaitNativeAppStopProcessingComplete::getNumberOfNativeSubscribers() const
{
    return static_cast<int>(getNativeSubscribers().size());
}

std::vector<int> WaitNativeAppStopProcessingComplete::getNativeSubscribers() const
{
    return mSubscriptionManager.getNativeSubscribers();
}

void WaitNativeAppStopProcessingComplete::logNoStopCompletes() const
{
    std::string noStopCompleteFrom;
    for (const auto& subscriber : getNativeSubscribers()) {
        if (mStopCompleteAppIds.find(subscriber) == mStopCompleteAppIds.end()) {
            noStopCompleteFrom += std::to_string(subscriber);
            noStopCompleteFrom += " ";
        }
    }
    MLOGW(LogFuncId::VPD, LogId::WaitNativeAppStopProcessingComplete_onTimeout_NoStopCompleteFrom,
          noStopCompleteFrom);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
