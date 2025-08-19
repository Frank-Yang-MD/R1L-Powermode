/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.subscriptionmanager;

import static org.mockito.Mockito.verify;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ISubscriptionNotification;
import java.util.ArrayList;
import java.util.List;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.mockito.junit.MockitoJUnitRunner;

@RunWith(MockitoJUnitRunner.class)
public class SubscriptionOberserveableDecoratorTest {
    @Mock private ISubscriptionNotification mISubscriberNotificationMock;

    private List<ISubscriptionNotification> mList = new ArrayList<>();
    private SubscriptionOberserveableDecorator mSubscriptionOberserveableDecorator;

    @Before
    public void setUp() {
        mSubscriptionOberserveableDecorator = new SubscriptionOberserveableDecorator(mList);
        mList.add(mISubscriberNotificationMock);
    }

    @Test
    public void onSubscribe() {
        mSubscriptionOberserveableDecorator.onSubscribe("TestClass");
        verify(mISubscriberNotificationMock).onSubscribe("TestClass");
    }

    @Test
    public void onUnsubscribe() {
        mSubscriptionOberserveableDecorator.onUnsubscribe("TestClass");
        verify(mISubscriberNotificationMock).onUnsubscribe("TestClass");
    }
}
