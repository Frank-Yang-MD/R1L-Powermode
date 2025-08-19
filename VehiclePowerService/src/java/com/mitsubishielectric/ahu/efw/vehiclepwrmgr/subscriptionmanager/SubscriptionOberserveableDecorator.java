/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.subscriptionmanager;

import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter.ISubscriptionNotification;
import java.util.List;
import java.util.Objects;

/**
 * SubscriptionOberserveableDecorator. It helps reduce boilerplate code needed to call
 * SubscriptionNotifications
 */
public class SubscriptionOberserveableDecorator implements ISubscriptionNotification {
    private final List<ISubscriptionNotification> mSubscriptionNotificationList;

    public SubscriptionOberserveableDecorator(
            List<ISubscriptionNotification> subscriptionNotificationList) {
        this.mSubscriptionNotificationList =
                Objects.requireNonNull(
                        subscriptionNotificationList,
                        "subscriptionNotificationList must not be null");
    }

    @Override
    public void onSubscribe(String className) {
        mSubscriptionNotificationList.forEach(
                subscriptionNotification -> subscriptionNotification.onSubscribe(className));
    }

    @Override
    public void onUnsubscribe(String className) {
        mSubscriptionNotificationList.forEach(
                subscriptionNotification -> subscriptionNotification.onUnsubscribe(className));
    }
}
