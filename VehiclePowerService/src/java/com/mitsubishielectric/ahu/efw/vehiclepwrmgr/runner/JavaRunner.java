/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_JAVA_START_SCRIPT;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_JAVA_START_THREAD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IJavaRunner;
import java.util.List;
import java.util.concurrent.ExecutorService;

/** Start java Apps/Services according to config file */
public final class JavaRunner implements IJavaRunner, Runnable {
    private final List<String> mScripts;
    private final IScriptExecutor mScriptExecutor;
    private final ExecutorService mExecutorService;
    /**
     * Constructor of JavaRunner
     *
     * @param scriptReader config file reader
     * @param scriptExecutor script executor
     * @param executorService executor
     */
    JavaRunner(
            IScriptReader scriptReader,
            IScriptExecutor scriptExecutor,
            ExecutorService executorService) {
        mScripts = scriptReader.read();
        mScriptExecutor = scriptExecutor;
        mExecutorService = executorService;
    }
    /** Start script execution */
    @Override
    public void start() {
        MLog.i(VPS_FUNCTION_ID, RUNNER_JAVA_START_SCRIPT);
        mExecutorService.execute(this);
    }
    /** Execute script commands */
    public void run() {
        MLog.i(VPS_FUNCTION_ID, RUNNER_JAVA_START_THREAD);
        mScriptExecutor.execute(mScripts);
    }
}
