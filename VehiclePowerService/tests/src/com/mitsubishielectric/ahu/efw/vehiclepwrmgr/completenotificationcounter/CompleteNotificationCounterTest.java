/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter;

import static junit.framework.Assert.assertFalse;
import static junit.framework.Assert.assertTrue;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.Mockito.mock;
import static org.mockito.Mockito.never;
import static org.mockito.Mockito.times;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.verifyZeroInteractions;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IEventReceiver;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import java.util.concurrent.ThreadLocalRandom;
import junit.framework.Assert;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class CompleteNotificationCounterTest {
    private IEventReceiver mockEventReceiver = null;
    private IStartupInfo mockIStartupInfo = null;
    private CompleteNotificationCounter mCompleteCounter = null;

    @Before
    public void beforeTest() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        mockEventReceiver = mock(IEventReceiver.class);
        mockIStartupInfo = mock(IStartupInfo.class);
        mCompleteCounter =
                new CompleteNotificationCounter(
                        mockEventReceiver, IncomingEvents.APP_RESTART, mockIStartupInfo);
    }

    @Test
    public void onCreateInstance() {
        Assert.assertNotNull(mockEventReceiver);
        Assert.assertNotNull(mCompleteCounter);
    }

    @Test
    public void onCompleteProcessingNotSubscribed() {
        int pid = ThreadLocalRandom.current().nextInt();

        mCompleteCounter.onSubscribe(String.valueOf(pid + 1));
        mCompleteCounter.reset();
        mCompleteCounter.onComplete(String.valueOf(pid));

        verify(mockEventReceiver, never()).onReceiveEvent(any());
    }

    @Test
    public void onCompleteProcessingSubscribedAndSendComplete() {
        int pid = ThreadLocalRandom.current().nextInt();

        mCompleteCounter.onSubscribe(String.valueOf(pid));
        mCompleteCounter.reset();
        mCompleteCounter.onComplete(String.valueOf(pid));

        verify(mockEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_RESTART);
    }

    @Test
    public void onCompleteProcessingSubscribedAndSendCompleteTwice() {
        int pid = ThreadLocalRandom.current().nextInt();

        mCompleteCounter.onSubscribe(String.valueOf(pid));
        mCompleteCounter.reset();
        mCompleteCounter.onComplete(String.valueOf(pid));
        mCompleteCounter.onComplete(String.valueOf(pid));

        verify(mockEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_RESTART);
    }

    @Test
    public void clearCompleteEmptySubscribers() {
        mCompleteCounter.reset();

        verify(mockEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_RESTART);
    }

    @Test
    public void completeAndUnsubscribeSame() {
        int pid_app1 = ThreadLocalRandom.current().nextInt();
        int pid_app2 = ThreadLocalRandom.current().nextInt();

        mCompleteCounter.onSubscribe(String.valueOf(pid_app1));
        mCompleteCounter.onSubscribe(String.valueOf(pid_app2));

        mCompleteCounter.reset();

        mCompleteCounter.onComplete(String.valueOf(pid_app1));
        mCompleteCounter.onUnsubscribe(String.valueOf(pid_app1));
        verify(mockEventReceiver, times(0)).onReceiveEvent(IncomingEvents.APP_RESTART);
    }

    @Test
    public void subscribeTwoUnsubscribeOne() {
        int pid_app1 = ThreadLocalRandom.current().nextInt();
        int pid_app2 = ThreadLocalRandom.current().nextInt();

        mCompleteCounter.onSubscribe(String.valueOf(pid_app1));
        mCompleteCounter.onSubscribe(String.valueOf(pid_app2));

        mCompleteCounter.reset();
        mCompleteCounter.onUnsubscribe(String.valueOf(pid_app1));

        verifyZeroInteractions(mockEventReceiver);
    }

    @Test
    public void subscribeTwoUnsubscribeTwo() {
        int pid_app1 = ThreadLocalRandom.current().nextInt();
        int pid_app2 = ThreadLocalRandom.current().nextInt();

        mCompleteCounter.onSubscribe(String.valueOf(pid_app1));
        mCompleteCounter.onSubscribe(String.valueOf(pid_app2));
        mCompleteCounter.onUnsubscribe(String.valueOf(pid_app1));
        mCompleteCounter.onUnsubscribe(String.valueOf(pid_app2));

        verify(mockEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_RESTART);
    }

    @Test
    public void subscribeTwoStopOneUnsubscribeOne() {
        int pid_app1 = ThreadLocalRandom.current().nextInt();
        int pid_app2 = ThreadLocalRandom.current().nextInt();

        mCompleteCounter.onSubscribe(String.valueOf(pid_app1));
        mCompleteCounter.onSubscribe(String.valueOf(pid_app2));

        mCompleteCounter.reset();
        mCompleteCounter.onComplete(String.valueOf(pid_app1));
        mCompleteCounter.onUnsubscribe(String.valueOf(pid_app2));

        verify(mockEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_RESTART);
    }

    @Test
    public void isAllReportedComplete() {
        int pid1 = ThreadLocalRandom.current().nextInt();
        int pid2 = ThreadLocalRandom.current().nextInt();

        mCompleteCounter.onSubscribe(String.valueOf(pid1));
        mCompleteCounter.onSubscribe(String.valueOf(pid2));

        assertFalse(mCompleteCounter.isComplete());

        mCompleteCounter.onUnsubscribe(String.valueOf(pid1));

        assertFalse(mCompleteCounter.isComplete());

        mCompleteCounter.onUnsubscribe(String.valueOf(pid2));

        assertTrue(mCompleteCounter.isComplete());
    }

    @Test
    public void completeDoneCheck() {
        int pid1 = ThreadLocalRandom.current().nextInt();
        int pid2 = ThreadLocalRandom.current().nextInt();
        int pid3 = ThreadLocalRandom.current().nextInt();

        mCompleteCounter.onSubscribe(String.valueOf(pid1));
        mCompleteCounter.onSubscribe(String.valueOf(pid2));

        mCompleteCounter.reset();
        mCompleteCounter.onComplete(String.valueOf(pid1));
        mCompleteCounter.onComplete(String.valueOf(pid2));

        mCompleteCounter.onSubscribe(String.valueOf(pid3));
        verify(mockEventReceiver, times(1)).onReceiveEvent(IncomingEvents.APP_RESTART);
        assertTrue(mCompleteCounter.isComplete());
    }
}
