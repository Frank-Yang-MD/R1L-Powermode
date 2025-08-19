/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.CNC_NOT_COMPLETE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.CNC_ON_COMPLETE_ALREADY_UNSUBSCRIBED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.CNC_ON_COMPLETE_FROM;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.CNC_ON_COMPLETE_NOT_SUBSCRIBED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.CNC_ON_RESET;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.CNC_ON_SUBSCRIBE_ACCEPTED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.CNC_ON_SUBSCRIBE_DECLINED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.CNC_ON_UNSUBSCRIBE_ACCEPTED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.CNC_ON_UNSUBSCRIBE_DECLINED;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_EARLY_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.logWarning;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.ICompleteNotification;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IEventReceiver;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import java.util.HashSet;
import java.util.function.Consumer;

/**
 * CompleteNotificationCounter is a general use class for counting complete notifications for the
 * subscribed apps/services.
 */
public final class CompleteNotificationCounter
        implements ICompleteNotification,
                ISubscriptionNotification,
                IResetCounter,
                ICheckCompletion {

    private final IEventReceiver mEventReceiver;
    private final IncomingEvents mEvent;
    private final IStartupInfo mStartupInfo;

    private HashSet<String> mSubscribers = new HashSet<>();
    private HashSet<String> mCompleteNotifications = new HashSet<>();

    private boolean mCompleteDone = false;

    public CompleteNotificationCounter(
            IEventReceiver receiver, IncomingEvents event, IStartupInfo startupInfo) {
        mEventReceiver = receiver;
        mEvent = event;
        mStartupInfo = startupInfo;
    }

    @Override
    public synchronized void onComplete(String className) {
        final Consumer<String> classAdded = this::actOnComplete;

        final Consumer<String> classNotAdded =
                (clName) ->
                        logWarning(
                                CNC_ON_COMPLETE_NOT_SUBSCRIBED,
                                mEvent.name() + "/" + mStartupInfo.getShortClassName(clName));

        completeAction(classAdded, classNotAdded, mSubscribers, className);
    }

    @Override
    public synchronized void onSubscribe(String className) {
        final Consumer<String> classSubscribed =
                (clName) ->
                        logWarning(
                                CNC_ON_SUBSCRIBE_DECLINED,
                                mEvent.name() + "/" + mStartupInfo.getShortClassName(clName));

        final Consumer<String> classNotSubscribed =
                (clName) -> {
                    mSubscribers.add(clName);
                    String logInfo = mEvent.name() + "/" + mStartupInfo.getShortClassName(clName);
                    MLog.i(
                            VPS_EARLY_FUNCTION_ID,
                            CNC_ON_SUBSCRIBE_ACCEPTED,
                            mSubscribers.size(),
                            logInfo);
                    MLog.i(
                            VPS_FUNCTION_ID,
                            CNC_ON_SUBSCRIBE_ACCEPTED,
                            mSubscribers.size(),
                            logInfo);
                };

        completeAction(classSubscribed, classNotSubscribed, mSubscribers, className);
    }

    @Override
    public synchronized void onUnsubscribe(String className) {
        final Consumer<String> classSubscribed =
                (clName) -> {
                    mSubscribers.remove(clName);
                    mCompleteNotifications.remove(clName);
                    MLog.i(
                            VPS_FUNCTION_ID,
                            CNC_ON_UNSUBSCRIBE_ACCEPTED,
                            mSubscribers.size(),
                            mEvent.name() + "/" + mStartupInfo.getShortClassName(clName));
                    if (checkIsComplete()) {
                        mEventReceiver.onReceiveEvent(mEvent);
                        mCompleteDone = true;
                    }
                };

        final Consumer<String> classNotSubscribed =
                (clName) ->
                        logWarning(
                                CNC_ON_UNSUBSCRIBE_DECLINED,
                                mEvent.name() + "/" + mStartupInfo.getShortClassName(clName));

        completeAction(classSubscribed, classNotSubscribed, mSubscribers, className);
    }

    @Override
    public synchronized void reset() {
        MLog.i(VPS_FUNCTION_ID, CNC_ON_RESET, mEvent.name());
        mCompleteDone = false;
        mCompleteNotifications.clear();
        if (checkIsComplete()) {
            mEventReceiver.onReceiveEvent(mEvent);
            mCompleteDone = true;
        }
    }

    @Override
    public synchronized boolean isComplete() {
        return checkIsComplete();
    }

    @Override
    public void logNotCompleted() {
        findDifference(mSubscribers, mCompleteNotifications)
                .forEach(
                        s -> {
                            MLog.i(
                                    VPS_FUNCTION_ID,
                                    CNC_NOT_COMPLETE,
                                    mEvent.name() + "/" + mStartupInfo.getShortClassName(s));
                        });
    }

    private void actOnComplete(String className) {
        final Consumer<String> classComplete =
                (compName) ->
                        logWarning(
                                CNC_ON_COMPLETE_ALREADY_UNSUBSCRIBED,
                                mEvent.name() + "/" + mStartupInfo.getShortClassName(compName));

        final Consumer<String> classNotComplete =
                (compName) -> {
                    mCompleteNotifications.add(compName);
                    MLog.i(
                            VPS_FUNCTION_ID,
                            CNC_ON_COMPLETE_FROM,
                            mSubscribers.size(),
                            mCompleteNotifications.size(),
                            mEvent.name() + "/" + mStartupInfo.getShortClassName(compName));

                    if (checkIsComplete()) {
                        mEventReceiver.onReceiveEvent(mEvent);
                        mCompleteDone = true;
                    }
                };

        completeAction(classComplete, classNotComplete, mCompleteNotifications, className);
    }

    private boolean checkIsComplete() {
        return mCompleteDone || mCompleteNotifications.size() == mSubscribers.size();
    }

    private void completeAction(
            Consumer<String> subscriberExists,
            Consumer<String> subscriberNotExists,
            HashSet<String> hashSet,
            String className) {
        if (hashSet.contains(className)) {
            subscriberExists.accept(className);
        } else {
            subscriberNotExists.accept(className);
        }
    }

    private HashSet<String> findDifference(
            final HashSet<String> subscribed, final HashSet<String> completed) {
        HashSet<String> notCompleted = new HashSet<>(subscribed);
        notCompleted.removeIf(completed::contains);
        return notCompleted;
    }
}
