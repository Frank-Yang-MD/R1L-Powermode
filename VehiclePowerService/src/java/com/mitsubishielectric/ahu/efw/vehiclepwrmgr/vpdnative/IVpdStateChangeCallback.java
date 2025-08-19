/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative;

public interface IVpdStateChangeCallback {
    void onStateChange(int stateNum);

    void onFwRestart();

    void onAppRestart();

    void onFwResume();

    void onAppResume();

    void onFwStop();

    void onCompletionTimeout();

    void requestMount();

    void requestUnmount();
}
