/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

// LCOV_EXCL_START
// This is excluded from a unit test coverage report because it's hard to test.

#include "EventHandling.h"
#include "BootDaemonEventHandler.h"
#include "BootDaemonSubscriptionHandler.h"
#include "BootDaemonUnsubscriptionHandler.h"
#include "CpuStartHandler.h"
#include "DisconnectHandler.h"
#include "HmiEventsHandler.h"
#include "LogdogStopCompleteHandler.h"
#include "LogdogSubscriptionHandler.h"
#include "LogdogUnsubscriptionHandler.h"
#include "NativeStopCompleteHandler.h"
#include "ResumeProcessingStartCompleteSentHandler.h"
#include "ShutdownProcessingStartSentHandler.h"
#include "SubscriptionHandler.h"
#include "TimeoutHandler.h"
#include "UnsubscriptionHandler.h"
#include "VcpuPowerStateChangeHandler.h"
#include "VehiclePowerDaemonLog.h"
#include "VpsAppRestartCompleteHandler.h"
#include "VpsAppResumeCompleteHandler.h"
#include "VpsFwRestartCompleteHandler.h"
#include "VpsFwResumeCompleteHandler.h"
#include "VpsMountHandler.h"
#include "VpsStartHandler.h"
#include "VpsStopCompleteHandler.h"
#include "VpsSubscriptionHandler.h"
#include "VpsUnsubscriptionHandler.h"
#include "WakeUpHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

EventHandling::EventHandling(IEventSource& inIEventSource,
                             IVpdState& inIVpdState,
                             ISubscriptionManager& inISubscriptionManager,
                             IPowerStateHolder& inIPowerStateHolder,
                             IVpdNotificationSender& inVpdNotificationSender,
                             IEventReceiver& inIEventReceiver,
                             ITimeoutNotification& inTimeoutNotification,
                             IPowerStateChangeListener& powerStateChangeListener)
{
    mEventQueueHandler = make_shared<EventQueueHandler>(inIEventSource, inIEventReceiver);

    ////////// Initialization of Event Handlers ////////////////////////////////////////////////////
    mHandlers[common::EVpdEvent::CpuComStarted] = make_shared<CpuStartHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::VpsStarted] = make_shared<VpsStartHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::StartFailedVps] = make_shared<VpsStartHandler>(inIVpdState);

    const auto powerStateChangeHandler = make_shared<VcpuPowerStateChangeHandler>(
        inIVpdState, inIPowerStateHolder, powerStateChangeListener);

    mHandlers[common::EVpdEvent::Shutdown] = powerStateChangeHandler;
    mHandlers[common::EVpdEvent::Suspend] = powerStateChangeHandler;
    mHandlers[common::EVpdEvent::Reboot] = powerStateChangeHandler;
    mHandlers[common::EVpdEvent::RebootRecovery] = powerStateChangeHandler;
    mHandlers[common::EVpdEvent::ShutdownCancel] = powerStateChangeHandler;

    mHandlers[common::EVpdEvent::StopCompleteNative] =
        make_shared<NativeStopCompleteHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::StopCompleteLogdog] =
        make_shared<LogdogStopCompleteHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::Timeout] =
        make_shared<TimeoutHandler>(inIVpdState, inTimeoutNotification);

    mHandlers[common::EVpdEvent::AppStopCompleteVps] =
        make_shared<VpsStopCompleteHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::FwStopCompleteVps] =
        make_shared<VpsStopCompleteHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::FwResumeCompleteVps] =
        make_shared<VpsFwResumeCompleteHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::FwRestartCompleteVps] =
        make_shared<VpsFwRestartCompleteHandler>(inIVpdState);

    mHandlers[common::EVpdEvent::AppResumeCompleteVps] =
        make_shared<VpsAppResumeCompleteHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::AppRestartCompleteVps] =
        make_shared<VpsAppRestartCompleteHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::UnmountComplete] = make_shared<VpsMountHandler>(inIVpdState);

    mHandlers[common::EVpdEvent::StopFailedVps] = make_shared<VpsStopCompleteHandler>(inIVpdState);

    mHandlers[common::EVpdEvent::Disconnect] =
        make_shared<DisconnectHandler>(inISubscriptionManager);
    mHandlers[common::EVpdEvent::SubscribeNative] =
        make_shared<SubscriptionHandler>(inISubscriptionManager);
    mHandlers[common::EVpdEvent::UnsubscribeNative] =
        make_shared<UnsubscriptionHandler>(inISubscriptionManager);
    mHandlers[common::EVpdEvent::SubscribeVps] =
        make_shared<VpsSubscriptionHandler>(inISubscriptionManager);
    mHandlers[common::EVpdEvent::UnsubscribeVps] =
        make_shared<VpsUnsubscriptionHandler>(inISubscriptionManager);
    mHandlers[common::EVpdEvent::SubscribeLogdog] =
        make_shared<SubscriptionHandlerLogdog>(inISubscriptionManager);
    mHandlers[common::EVpdEvent::UnsubscribeLogdog] =
        make_shared<UnsubscriptionHandlerLogdog>(inISubscriptionManager);
    mHandlers[common::EVpdEvent::SubscribeBootDaemon] =
        make_shared<BootDaemonSubscriptionHandler>(inISubscriptionManager);
    mHandlers[common::EVpdEvent::UnsubscribeBootDaemon] =
        make_shared<BootDaemonUnsubscriptionHandler>(inISubscriptionManager);
    mHandlers[common::EVpdEvent::ShutdownProcessingStartSent] =
        make_shared<ShutdownProcessingStartSentHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::ResumeProcessingStartCompleteSentSuccess] =
        make_shared<ResumeProcessingStartCompleteSentHandler>(inIVpdState);
    mHandlers[common::EVpdEvent::ResumeProcessingStartCompleteSentFailure] =
        make_shared<ResumeProcessingStartCompleteSentHandler>(inIVpdState);

    const auto bootDaemonEventHandler =
        make_shared<BootDaemonEventHandler>(inVpdNotificationSender, inISubscriptionManager);
    mHandlers[common::EVpdEvent::handOffResponsibility] = bootDaemonEventHandler;
    mHandlers[common::EVpdEvent::handOffResponsibilityStartupAnimationDisplayed] =
        bootDaemonEventHandler;
    mHandlers[common::EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed] =
        bootDaemonEventHandler;

    const auto hmiEventsChangeHandler =
        make_shared<HmiEventsHandler>(inVpdNotificationSender, inISubscriptionManager);
    mHandlers[common::EVpdEvent::VpsReady] = hmiEventsChangeHandler;
    mHandlers[common::EVpdEvent::EndEarlyHmi] = hmiEventsChangeHandler;
    mHandlers[common::EVpdEvent::ForcedCompletionEarlyServices] = hmiEventsChangeHandler;

    mHandlers[common::EVpdEvent::WakeUp] = make_shared<WakeUpHandler>(inIVpdState);
    ////////////////////////////////////////////////////////////////////////////////////////////////

    ////////// Initialization of Event Queue Handler ///////////////////////////////////////////////
    for (auto row : mHandlers) {
        mEventQueueHandler->setEventHandler(row.first, row.second.get());
    }
    ////////////////////////////////////////////////////////////////////////////////////////////////
}

void EventHandling::run()
{
    if (mEventQueueHandler == nullptr) {
        MLOGW(LogFuncId::VPD, LogId::EventHandling_run_mEventQueueHandler_is_nullptr);
        return;
    }
    mEventQueueHandler->run();
}

void EventHandling::stop()
{
    if (mEventQueueHandler == nullptr) {
        MLOGW(LogFuncId::VPD, LogId::EventHandling_stop_mEventQueueHandler_is_nullptr);
        return;
    }
    mEventQueueHandler->stop();
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

// LCOV_EXCL_STOP
