/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.vpdnative;

public interface IVpdNativeAdapter {

    void vpdStartComplete();

    void vpdStartFailed();

    void vpdAppStopComplete();

    void vpdFwRestartComplete();

    void vpdFwResumeComplete();

    void vpdAppRestartComplete();

    void vpdAppResumeComplete();

    void vpdFwStopComplete();

    void vpdStopFailed();

    void hmiIsReady();

    void unmountComplete();

    void endEarlyHmi();

    void forcedCompletionEarlyServices();
}
