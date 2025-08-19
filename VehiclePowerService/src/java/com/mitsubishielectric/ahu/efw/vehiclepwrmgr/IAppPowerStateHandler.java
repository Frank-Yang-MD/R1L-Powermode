/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr;

public interface IAppPowerStateHandler {
    void onAppRestart();

    void onAppResume();

    void onAppStop();

    void onFwRestart();

    void onFwResume();

    void onFwStop();

    void onAppStartColdboot();
}
