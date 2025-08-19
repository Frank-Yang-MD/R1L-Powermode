/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <vector>

#include "BootDaemonEventHandler.h"
#include "EventHandlingMocks.h"
#include "HmiEventsHandler.h"
#include "MockVpdNotificationSender.h"
#include <CpuStartHandler.h>
#include <DisconnectHandler.h>
#include <LogdogStopCompleteHandler.h>
#include <LogdogSubscriptionHandler.h>
#include <LogdogUnsubscriptionHandler.h>
#include <MockPowerStateHolder.h>
#include <NativeStopCompleteHandler.h>
#include <ResumeProcessingStartCompleteSentHandler.h>
#include <ShutdownProcessingStartSentHandler.h>
#include <SubscriptionHandler.h>
#include <TimeoutHandler.h>
#include <UnsubscriptionHandler.h>
#include <VcpuPowerStateChangeHandler.h>
#include <VpsStartHandler.h>
#include <VpsStopCompleteHandler.h>
#include <VpsSubscriptionHandler.h>
#include <VpsUnsubscriptionHandler.h>

using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using namespace testing;

// Contain all event from EVpdEvent enum
std::vector<EVpdEvent> events{
    EVpdEvent::Unsupported,

    EVpdEvent::SubscribeNative,
    EVpdEvent::UnsubscribeNative,
    EVpdEvent::StopCompleteNative,

    EVpdEvent::CpuComStarted,

    EVpdEvent::SubscribeVps,
    EVpdEvent::UnsubscribeVps,
    EVpdEvent::VpsStarted,
    EVpdEvent::StartFailedVps,
    EVpdEvent::AppStopCompleteVps,
    EVpdEvent::FwStopCompleteVps,

    EVpdEvent::SubscribeLogdog,
    EVpdEvent::UnsubscribeLogdog,
    EVpdEvent::StopCompleteLogdog,

    EVpdEvent::Timeout,
    EVpdEvent::Disconnect,

    EVpdEvent::Shutdown,
    EVpdEvent::Suspend,
    EVpdEvent::Reboot,
    EVpdEvent::RebootRecovery,
    EVpdEvent::ShutdownCancel,
    EVpdEvent::ShutdownProcessingStartSent,
    EVpdEvent::ResumeProcessingStartCompleteSentSuccess,
    EVpdEvent::ResumeProcessingStartCompleteSentFailure,
};

class EventHandlingTest : public Test {
protected:
    MockStartCompleteCpuCom mStartCompleteCpuCom;
    MockStartCompleteVps mStartCompleteVps;
    MockStopCompleteNative mStopCompleteNative;
    MockStopCompleteLog mStopCompleteLog;
    MockSubscriptionManager mSubscriptionManager;
    MockStopCompleteVps mStopCompleteVps;
    MockVpdNotificationSender mVpdNotificationSender;

    MockTimeoutNotification mStateMachine_TimeoutNotification;
    MockTimeoutNotification mPowerStateHolder_TimeoutNotification;

    MockPowerStateChangeListener mPowerStateHolder_PowerStateChangeListener;
    MockPowerStateChangeListener mStateMachine_PowerStateChangeListener;
    MockPowerStateChangeListener mPowerStateChangeListener_PowerStateChangeListener;

    MockShutdownProcessingStartSent mShutdownProcessingStartSent;
    MockResumeProcessingStartCompleteSent mResumeProcessingStartCompeteSent;

    CpuStartHandler mCpuStartHandler{mStartCompleteCpuCom};
    VpsStartHandler mVpsStartHandler{mStartCompleteVps};
    NativeStopCompleteHandler mNativeStopCompleteHandler{mStopCompleteNative};
    LogdogStopCompleteHandler mLogStopCompleteHandler{mStopCompleteLog};
    SubscriptionHandlerLogdog mLogSubscriptionHandler{mSubscriptionManager};
    UnsubscriptionHandlerLogdog mLogUnsubscriptionHandler{mSubscriptionManager};
    TimeoutHandler mTimeoutHandler{mStateMachine_TimeoutNotification,
                                   mPowerStateHolder_TimeoutNotification};
    SubscriptionHandler mSubscriptionHandler{mSubscriptionManager};
    UnsubscriptionHandler mUnsubscriptionHandler{mSubscriptionManager};
    VpsSubscriptionHandler mVpsSubscriptionHandler{mSubscriptionManager};
    VpsUnsubscriptionHandler mVpsUnsubscriptionHandler{mSubscriptionManager};
    DisconnectHandler mDisconnectHandler{mSubscriptionManager};
    VpsStopCompleteHandler mVpsStopCompleteHandler{mStopCompleteVps};
    VcpuPowerStateChangeHandler mVcpuPowerStateChangeHandler{
        mStateMachine_PowerStateChangeListener, mPowerStateHolder_PowerStateChangeListener,
        mPowerStateChangeListener_PowerStateChangeListener};
    ShutdownProcessingStartSentHandler mShutdownProcessingStartSentHandler{
        mShutdownProcessingStartSent};
    ResumeProcessingStartCompleteSentHandler mResumeProcessingStartCompleteSentHandler{
        mResumeProcessingStartCompeteSent};
    BootDaemonEventHandler mBootDaemonEventHandler{mVpdNotificationSender, mSubscriptionManager};
    HmiEventsHandler mHmiEventsHandler{mVpdNotificationSender, mSubscriptionManager};
};

/**
 * Test for CpuStartHandler.
 * If incoming event is CpuComStarted then onStartComplete shuld be called once.
 * In other case should be printed error message to the error stream.
 */
TEST_F(EventHandlingTest, CpuStartHandler_StartCompleteCpuComCall)
{
    auto helperCall = [this](EVpdEvent event, int times) {
        EXPECT_CALL(mStartCompleteCpuCom, onStartCompleteCpuCom()).Times(times);
        // 0 - workaround. Process get first argument int, set 0 due to
        // -Werror compiler flag is set
        mCpuStartHandler.Process(0, event);
    };

    for (const auto& event : events) {
        if (event == EVpdEvent::CpuComStarted) {
            helperCall(event, 1);  // expected call - once
        }
        else {
            helperCall(event, 0);  // expected call - never
        }
    }
}

/**
 * Test for VpsStartHandler.
 * If incoming event is VpsStarted then onStartCompleteVps should be called once
 * In other case should be printed error message to the error stream
 */
TEST_F(EventHandlingTest, VpsStartHandler_StartCompleteVpsCall)
{
    auto helperCall = [this](EVpdEvent event, int completeTimes, int failTimes) {
        EXPECT_CALL(mStartCompleteVps, onStartCompleteVps()).Times(completeTimes);
        EXPECT_CALL(mStartCompleteVps, onStartFailedVps()).Times(failTimes);
        // 0 - workaround. Process get first argument int, set 0 due to
        // -Werror compiler flag is set
        mVpsStartHandler.Process(0, event);
    };

    for (const auto& event : events) {
        if (event == EVpdEvent::VpsStarted) {
            helperCall(event, 1, 0);  // expected call - once
        }
        else if (event == EVpdEvent::StartFailedVps) {
            helperCall(event, 0, 1);
        }
        else {
            helperCall(event, 0, 0);  // expected call - never
        }
    }
}

TEST_F(EventHandlingTest, VcpuShutdownRequestHandler_SuspendRequestVcpuCall)
{
    auto vpdEvent = EVpdEvent::Suspend;
    auto expectedPowerState = PowerState::SUSPEND;

    Sequence seq;

    EXPECT_CALL(mPowerStateHolder_PowerStateChangeListener, onPowerStateChange(expectedPowerState))
        .Times(1)
        .InSequence(seq);
    EXPECT_CALL(mStateMachine_PowerStateChangeListener, onPowerStateChange(expectedPowerState))
        .Times(1)
        .InSequence(seq);

    mVcpuPowerStateChangeHandler.Process(0, vpdEvent);
}

TEST_F(EventHandlingTest, VcpuShutdownRequestHandler_ShutdownRequestVcpuCall)
{
    auto vpdEvent = EVpdEvent::Shutdown;
    auto expectedPowerState = PowerState::SHUTDOWN;

    Sequence seq;

    EXPECT_CALL(mPowerStateHolder_PowerStateChangeListener, onPowerStateChange(expectedPowerState))
        .Times(1)
        .InSequence(seq);
    EXPECT_CALL(mStateMachine_PowerStateChangeListener, onPowerStateChange(expectedPowerState))
        .Times(1)
        .InSequence(seq);

    mVcpuPowerStateChangeHandler.Process(0, vpdEvent);
}

TEST_F(EventHandlingTest, VcpuShutdownRequestHandler_RebootRequestVcpuCall)
{
    auto vpdEvent = EVpdEvent::Reboot;
    auto expectedPowerState = PowerState::REBOOT;

    Sequence seq;

    EXPECT_CALL(mPowerStateHolder_PowerStateChangeListener, onPowerStateChange(expectedPowerState))
        .Times(1)
        .InSequence(seq);
    EXPECT_CALL(mStateMachine_PowerStateChangeListener, onPowerStateChange(expectedPowerState))
        .Times(1)
        .InSequence(seq);

    mVcpuPowerStateChangeHandler.Process(0, vpdEvent);
}

TEST_F(EventHandlingTest, VcpuShutdownRequestHandler_RebootRecoveryRequestVcpuCall)
{
    auto vpdEvent = EVpdEvent::RebootRecovery;
    auto expectedPowerState = PowerState::REBOOT_RECOVERY;

    Sequence seq;

    EXPECT_CALL(mPowerStateHolder_PowerStateChangeListener, onPowerStateChange(expectedPowerState))
        .Times(1)
        .InSequence(seq);
    EXPECT_CALL(mStateMachine_PowerStateChangeListener, onPowerStateChange(expectedPowerState))
        .Times(1)
        .InSequence(seq);

    mVcpuPowerStateChangeHandler.Process(0, vpdEvent);
}

TEST_F(EventHandlingTest, VcpuShutdownRequestHandler_SuspendCancelRequestVcpuCall)
{
    auto vpdEvent = EVpdEvent::ShutdownCancel;
    auto expectedPowerState = PowerState::SUSPEND_CANCEL;

    Sequence seq;

    EXPECT_CALL(mPowerStateHolder_PowerStateChangeListener, onPowerStateChange(expectedPowerState))
        .Times(1)
        .InSequence(seq);
    EXPECT_CALL(mStateMachine_PowerStateChangeListener, onPowerStateChange(expectedPowerState))
        .Times(1)
        .InSequence(seq);

    mVcpuPowerStateChangeHandler.Process(0, vpdEvent);
}

TEST_F(EventHandlingTest, VcpuShutdownRequestHandler_IncorrectVpdState)
{
    auto notRelatedToShutdownEvents = events;

    const std::vector<EVpdEvent> shutdownRelatedEvents = {
        EVpdEvent::Shutdown,       EVpdEvent::Suspend,        EVpdEvent::Reboot,
        EVpdEvent::RebootRecovery, EVpdEvent::ShutdownCancel,
    };

    notRelatedToShutdownEvents.erase(
        std::remove_if(std::begin(notRelatedToShutdownEvents), std::end(notRelatedToShutdownEvents),
                       [&shutdownRelatedEvents](const auto& event) {
                           return std::find(std::cbegin(shutdownRelatedEvents),
                                            std::cend(shutdownRelatedEvents),
                                            event) != std::cend(shutdownRelatedEvents);
                       }),
        std::end(notRelatedToShutdownEvents));

    for (const auto& event : notRelatedToShutdownEvents) {
        EXPECT_CALL(mPowerStateHolder_PowerStateChangeListener, onPowerStateChange(_)).Times(0);
        EXPECT_CALL(mStateMachine_PowerStateChangeListener, onPowerStateChange(_)).Times(0);

        mVcpuPowerStateChangeHandler.Process(0, event);
    }
}

/**
 * Test for NativeStopCompleteHandler.
 * If incoming event is StopCompleteNative then onStopCompleteNative
 * should be called once. In other case should be printed error message
 * to the error stream
 */
TEST_F(EventHandlingTest, NativeStopCompleteHandler_StopCompleteNativeCall)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mStopCompleteNative, onStopCompleteNative(id)).Times(times);
        mNativeStopCompleteHandler.Process(id, event);
    }};

    constexpr int id{123};

    for (const auto& event : events) {
        if (event == EVpdEvent::StopCompleteNative) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

/**
 * Test for LogStopCompleteHandler.
 * If incoming event is StopCompleteLog then onStopCompleteLog
 * should be called once. In other case should be printed error message
 * to the error stream
 */
TEST_F(EventHandlingTest, LogStopCompleteHandler_StopCompleteLogCall)
{
    auto helperCall{[this](EVpdEvent event, int times) {
        EXPECT_CALL(mStopCompleteLog, onStopCompleteLog()).Times(times);
        // 0 - workaround. Process get first argument int, set 0 due to
        // -Werror compiler flag is set
        mLogStopCompleteHandler.Process(0, event);
    }};

    for (const auto& event : events) {
        if (event == EVpdEvent::StopCompleteLogdog) {
            helperCall(event, 1);  // expected call - once
        }
        else {
            helperCall(event, 0);  // expected call - never
        }
    }
}

/**
 * Test for LogdogSubscriptionHandler.
 * If incoming event is SubscribeLogdog then "setLogID(id)" should be called once.
 * In other case should be printed error message to the error stream
 */
TEST_F(EventHandlingTest, LogdogSubscriptionHandler_SubscriptionManagerCall)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mSubscriptionManager, setLogID(id)).Times(times);
        mLogSubscriptionHandler.Process(id, event);
    }};

    constexpr int id{123};

    for (const auto& event : events) {
        if (event == EVpdEvent::SubscribeLogdog) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

/**
 * Test for LogdogUnsubscriptionHandler.
 * If incoming event is UnsubscribeLogdog then "setLogID(id)" should be called once.
 * In other case should be printed error message to the error stream
 */
TEST_F(EventHandlingTest, LogdogUnsubscriptionHandler_SubscriptionManagerCall)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mSubscriptionManager, setLogID(-1)).Times(times);
        mLogUnsubscriptionHandler.Process(id, event);
    }};

    constexpr int id{123};
    constexpr int false_id{1};

    EXPECT_CALL(mSubscriptionManager, getLogID()).WillRepeatedly(Return(id));

    helperCall(EVpdEvent::UnsubscribeLogdog, 0, false_id);  // expected call - never

    for (const auto& event : events) {
        if (event == EVpdEvent::UnsubscribeLogdog) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

/**
 * Test for TimeoutHandler.
 * If incoming event is Timeout then "onTimeout" should be called once.
 * In other case should be printed error message to the error stream
 */
TEST_F(EventHandlingTest, TimeoutHandler_TimeoutNotificationCall)
{
    auto helperCall{[this](EVpdEvent event, int times) {
        EXPECT_CALL(mStateMachine_TimeoutNotification, onTimeout()).Times(times);
        EXPECT_CALL(mPowerStateHolder_TimeoutNotification, onTimeout()).Times(times);
        // 0 - workaround. Process get first argument int, set 0 due to
        // -Werror compiler flag is set
        mTimeoutHandler.Process(0, event);
    }};

    for (const auto& event : events) {
        if (event == EVpdEvent::Timeout) {
            helperCall(event, 1);  // expected call - once
        }
        else {
            helperCall(event, 0);  // expected call - never
        }
    }
}

/**
 * Test for SubscriptionHandler.
 * If incoming event is SubscribeNative then "add(id)" should be called once.
 * In other case should be printed error message to the error stream
 */
TEST_F(EventHandlingTest, SubscriptionHandler_SubscriptionManagerCall)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mSubscriptionManager, add(id)).Times(times);
        mSubscriptionHandler.Process(id, event);
    }};

    constexpr int id{123};

    for (const auto& event : events) {
        if (event == EVpdEvent::SubscribeNative) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

/**
 * Test for UnsubscriptionHandler.
 * If incoming event is UnsubscribeNative then "remove(id)" should be called once.
 * In other case should be printed error message to the error stream
 */
TEST_F(EventHandlingTest, UnsubscriptionHandler_SubscriptionManagerCall)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mSubscriptionManager, remove(id)).Times(times);
        mUnsubscriptionHandler.Process(id, event);
    }};

    constexpr int id{123};

    for (const auto& event : events) {
        if (event == EVpdEvent::UnsubscribeNative) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

/**
 * Test for VpsSubscriptionHandler.
 * If incoming event is SubscribeVps then "setVpsID(id)" should be called once.
 * In other case should be printed error message to the error stream
 */
TEST_F(EventHandlingTest, VpsSubscriptionHandler_SubscriptionManagerCall)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mSubscriptionManager, setVpsID(id)).Times(times);
        mVpsSubscriptionHandler.Process(id, event);
    }};

    constexpr int id{123};

    for (const auto& event : events) {
        if (event == EVpdEvent::SubscribeVps) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

/**
 * Test for VpsUnsubscriptionHandler.
 * If incoming event is UnsubscribeVps then "setVpsID(id)" should be called once.
 * In other case should be printed error message to the error stream
 */
TEST_F(EventHandlingTest, VpsUnsubscriptionHandler_SubscriptionManagerCall)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mSubscriptionManager, setVpsID(-1)).Times(times);
        EXPECT_CALL(mSubscriptionManager, remove(id)).Times(times);
        mVpsUnsubscriptionHandler.Process(id, event);
    }};

    constexpr int id{123};
    constexpr int false_id{1};

    EXPECT_CALL(mSubscriptionManager, getVpsID()).WillRepeatedly(Return(id));

    helperCall(EVpdEvent::UnsubscribeVps, 0, false_id);  // expected call - never

    for (const auto& event : events) {
        if (event == EVpdEvent::UnsubscribeVps) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

/**
 * Test for DisconnectHandler.
 * If incoming event is Disconnect then "disconnect" method should be called once.
 * In other case an error message should be printed to the error stream
 */
TEST_F(EventHandlingTest, VpsDisconnectHandler_SubscriptionManagerCall)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mSubscriptionManager, disconnect(id)).Times(times);
        mDisconnectHandler.Process(id, event);
    }};

    constexpr int id{123};

    for (const auto& event : events) {
        if (event == EVpdEvent::Disconnect) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

/**
 * Test for VpsStopCompleteHandler.
 * If incoming event is AppStopCompleteVps then "onAppStopCompleteVps"
 * should be called once. Else if incoming event is FwStopCompleteVps then
 * "onFwStopCompleteVps" should be called once. Else if incoming event is
 * StopFailedVps then "onStopFailedVps" should be called once. In other
 * case should be printed error message to the error stream.
 */
TEST_F(EventHandlingTest, VpsStopCompleteHandler_StopCompleteVpsCall)
{
    auto helperCall{
        [this](EVpdEvent event, int appStoptimes, int fwStopTimes, int stopFailedTimes) {
            EXPECT_CALL(mStopCompleteVps, onAppStopCompleteVps()).Times(appStoptimes);
            EXPECT_CALL(mStopCompleteVps, onFwStopCompleteVps()).Times(fwStopTimes);
            EXPECT_CALL(mStopCompleteVps, onStopFailedVps()).Times(stopFailedTimes);
            mVpsStopCompleteHandler.Process(0, event);
        }};

    for (const auto& event : events) {
        if (event == EVpdEvent::AppStopCompleteVps) {
            helperCall(event, 1, 0, 0);
        }
        else if (event == EVpdEvent::FwStopCompleteVps) {
            helperCall(event, 0, 1, 0);
        }
        else if (event == EVpdEvent::StopFailedVps) {
            helperCall(event, 0, 0, 1);
        }
        else {
            helperCall(event, 0, 0, 0);  // expected call - never
        }
    }
}
TEST_F(EventHandlingTest, BootDaemonEventHandler_Test)
{
    static constexpr int mBootdSocketId{10};

    auto helperCall{[this](EVpdEvent event, int handOff, int handOffStartupAnimation,
                           int handOffStartupAnimationAndDi) {
        ON_CALL(mSubscriptionManager, getBootDaemonID()).WillByDefault(Return(mBootdSocketId));
        EXPECT_CALL(mVpdNotificationSender, sendNotification(_, _)).Times(handOff);
        EXPECT_CALL(mVpdNotificationSender, sendNotification(_, _)).Times(handOffStartupAnimation);
        EXPECT_CALL(mVpdNotificationSender, sendNotification(_, _))
            .Times(handOffStartupAnimationAndDi);
        mBootDaemonEventHandler.Process(mBootdSocketId, event);
    }};

    for (const auto& event : events) {
        if (event == EVpdEvent::handOffResponsibility) {
            helperCall(event, 1, 0, 0);
        }
        else if (event == EVpdEvent::handOffResponsibilityStartupAnimationDisplayed) {
            helperCall(event, 0, 1, 0);
        }
        else if (event == EVpdEvent::handOffResponsibilityStartupAnimationAndDiDisplayed) {
            helperCall(event, 0, 0, 1);
        }
        else {
            helperCall(event, 0, 0, 0);  // expected call - never
        }
    }
}
TEST_F(EventHandlingTest, HmiEventsHandler_Test)
{
    static constexpr int mVpsSocketId{10};

    auto helperCall{
        [this](EVpdEvent event, int VpsReady, int EndEarlyHmi, int ForcedCompletionEarlyServices) {
            ON_CALL(mSubscriptionManager, getVpsID()).WillByDefault(Return(mVpsSocketId));
            EXPECT_CALL(mVpdNotificationSender, sendNotification(_, _)).Times(VpsReady);
            EXPECT_CALL(mVpdNotificationSender, sendNotification(_, _)).Times(EndEarlyHmi);
            EXPECT_CALL(mVpdNotificationSender, sendNotification(_, _))
                .Times(ForcedCompletionEarlyServices);
            mHmiEventsHandler.Process(mVpsSocketId, event);
        }};

    for (const auto& event : events) {
        if (event == EVpdEvent::VpsReady) {
            helperCall(event, 1, 0, 0);
        }
        else if (event == EVpdEvent::EndEarlyHmi) {
            helperCall(event, 0, 1, 0);
        }
        else if (event == EVpdEvent::ForcedCompletionEarlyServices) {
            helperCall(event, 0, 0, 1);
        }
        else {
            helperCall(event, 0, 0, 0);  // expected call - never
        }
    }
}

/**
 * Test for ShutdownProcessingStartSentHandler.
 * If incoming event is ShutdownProcessingStartSent then "onShutdownProcessingStartSent" method
 * should be called once. In other case an error message should be printed to the error stream
 */
TEST_F(EventHandlingTest, ShutdownProcessingStartSentHandler_onShutdownProcessingStartSent_call)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mShutdownProcessingStartSent, onShutdownProcessingStartSent()).Times(times);
        mShutdownProcessingStartSentHandler.Process(id, event);
    }};

    constexpr int id{0};

    for (const auto& event : events) {
        if (event == EVpdEvent::ShutdownProcessingStartSent) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

/**
 * Test for ResumeProcessingStartCompleteSentHandler.
 * If incoming event is ResumeProcessingStartCompleteSentSuccess then
 * "onResumeProcessingStartCompleteSentSuccess" method should be called once. In other case an error
 * message should be printed to the error stream
 */
TEST_F(EventHandlingTest,
       ResumeProcessingStartCompleteSentHandler_onResumeProcessingStartCompleteSentSuccess_call)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mResumeProcessingStartCompeteSent, onResumeProcessingStartCompleteSentSuccess())
            .Times(times);
        mResumeProcessingStartCompleteSentHandler.Process(id, event);
    }};

    constexpr int id{0};

    for (const auto& event : events) {
        if (event == EVpdEvent::ResumeProcessingStartCompleteSentSuccess) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

/**
 * Test for ResumeProcessingStartCompleteSentHandler.
 * If incoming event is ResumeProcessingStartCompleteSentFailure then
 * "onResumeProcessingStartCompleteSentFailure" method should be called once. In other case an error
 * message should be printed to the error stream
 */
TEST_F(EventHandlingTest,
       ResumeProcessingStartCompleteSentHandler_onResumeProcessingStartCompleteSentFailure_call)
{
    auto helperCall{[this](EVpdEvent event, int times, int id) {
        EXPECT_CALL(mResumeProcessingStartCompeteSent, onResumeProcessingStartCompleteSentFailure())
            .Times(times);
        mResumeProcessingStartCompleteSentHandler.Process(id, event);
    }};

    constexpr int id{0};

    for (const auto& event : events) {
        if (event == EVpdEvent::ResumeProcessingStartCompleteSentFailure) {
            helperCall(event, 1, id);  // expected call - once
        }
        else {
            helperCall(event, 0, id);  // expected call - never
        }
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
