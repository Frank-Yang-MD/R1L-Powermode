/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

public interface ISubscriberNotification {
    void notifyAppRestart();

    void notifyAppResume();

    void notifyAppStart();

    void notifyAppStop();
}
