/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.RUNNER_JAVA_CREAT_BUILD;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import android.content.Context;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.IJavaRunner;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IEventReceiver;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IInfoContainer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import java.util.concurrent.Executors;

/** Creator of JavaRunner */
public final class JavaRunnerCreator {
    private final Context context;
    private final IBinder binder;
    private final IEventReceiver eventReceiver;
    private final IInfoContainer mContainer;
    private final IStartupInfo mStartupInfo;
    /**
     * Constructor.
     *
     * @param context android context
     * @param binder android binder
     * @param eventReceiver event trigger
     */
    public JavaRunnerCreator(
            Context context,
            IBinder binder,
            IEventReceiver eventReceiver,
            IInfoContainer container,
            IStartupInfo startupInfo) {
        if (context == null) {
            throw new IllegalArgumentException("context can't be null");
        }

        if (binder == null) {
            throw new IllegalArgumentException("binder can't be null");
        }

        if (eventReceiver == null) {
            throw new IllegalArgumentException("eventReceiver can't be null");
        }

        if (startupInfo == null) {
            throw new IllegalArgumentException("startupInfo can't be null");
        }

        this.context = context;
        this.binder = binder;
        this.eventReceiver = eventReceiver;
        mContainer = container;
        mStartupInfo = startupInfo;
    }

    /**
     * Create JavaRunner
     *
     * @param event when java apps/service starts this event will add to queue
     * @param reader config reader object
     * @return JavaRunner object
     */
    public IJavaRunner create(IncomingEvents event, IScriptReader reader) {
        if (event == null) {
            throw new IllegalArgumentException("event can't be null");
        }
        if (reader == null) {
            throw new IllegalArgumentException("reader can't be null");
        }
        MLog.i(VPS_FUNCTION_ID, RUNNER_JAVA_CREAT_BUILD, event.name());

        return new JavaRunner(
                reader,
                new ScriptExecutor(getFactory(context, binder), eventReceiver, event),
                Executors.newSingleThreadExecutor());
    }

    private IScriptCommandFactory getFactory(Context context, IBinder binder) {
        ScriptCommandFactory factory = new ScriptCommandFactory();

        factory.addCreator(new SleepCommandCreator(new JsonCommandParser()));
        factory.addCreator(
                new StartCommandCreator(
                        context, binder, new JsonCommandParser(), mContainer, mStartupInfo));
        factory.addCreator(
                new WaitCommandCreator(new JsonCommandParser(), mStartupInfo, mContainer));
        return factory;
    }
}
