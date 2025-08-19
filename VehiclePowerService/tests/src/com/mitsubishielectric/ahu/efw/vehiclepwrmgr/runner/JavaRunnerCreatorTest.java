/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */
package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.runner;

import static org.junit.Assert.assertNotNull;
import static org.mockito.Mockito.mock;

import android.content.Context;
import android.os.IBinder;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IEventReceiver;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue.IncomingEvents;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IInfoContainer;
import com.mitsubishielectric.ahu.efw.vehiclepwrmgr.startupinfocontainer.IStartupInfo;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.InjectMocks;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class JavaRunnerCreatorTest {
    @Mock private IEventReceiver mEventReceiver;

    @Mock private Context mContext;

    @Mock private IBinder mBinder;

    @Mock private IInfoContainer mIInfoContainer;

    @Mock private IStartupInfo mIStartupInfo;

    @InjectMocks
    private JavaRunnerCreator mCreator; // = new JavaRunnerCreator(mContext,mBinder,mEventReceiver);

    @Before
    public void setUp() {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
    }

    @Test
    public void create() {
        assertNotNull(mCreator);
        assertNotNull(
                mCreator.create(
                        IncomingEvents.LATE_APP_SERVICES_STARTED, mock(VehicleConfigReader.class)));
    }

    @Test(expected = IllegalArgumentException.class)
    public void createWithNullFileName() {
        mCreator.create(null, mock(VehicleConfigReader.class));
    }

    @Test(expected = IllegalArgumentException.class)
    public void createWithNullEvent() {
        mCreator.create(IncomingEvents.LATE_APP_SERVICES_STARTED, null);
    }

    @Test(expected = IllegalArgumentException.class)
    public void createWithNullContext() {
        new JavaRunnerCreator(null, mBinder, mEventReceiver, mIInfoContainer, mIStartupInfo);
    }

    @Test(expected = IllegalArgumentException.class)
    public void createWithNullBinder() {
        new JavaRunnerCreator(mContext, null, mEventReceiver, mIInfoContainer, mIStartupInfo);
    }

    @Test(expected = IllegalArgumentException.class)
    public void createWithNullEventReceiver() {
        new JavaRunnerCreator(mContext, mBinder, null, mIInfoContainer, mIStartupInfo);
    }

    @Test(expected = IllegalArgumentException.class)
    public void createWithNullStartupInfo() {
        new JavaRunnerCreator(mContext, mBinder, mEventReceiver, mIInfoContainer, null);
    }
}
