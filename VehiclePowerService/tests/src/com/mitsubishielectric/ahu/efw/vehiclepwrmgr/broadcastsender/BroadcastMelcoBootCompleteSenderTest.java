package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.broadcastsender;

import static com.mitsubishielectric.ahu.efw.lib.common.Const.ACCESS_MELCO_SERVICES;
import static org.mockito.ArgumentMatchers.any;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.verify;

import android.app.Service;
import android.content.Intent;
import android.os.Process;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import org.junit.Before;
import org.junit.Test;
import org.junit.runner.RunWith;
import org.mockito.Mock;
import org.powermock.api.support.membermodification.MemberMatcher;
import org.powermock.api.support.membermodification.MemberModifier;
import org.powermock.core.classloader.annotations.SuppressStaticInitializationFor;
import org.powermock.modules.junit4.PowerMockRunner;

@RunWith(PowerMockRunner.class)
@SuppressStaticInitializationFor("com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog")
public class BroadcastMelcoBootCompleteSenderTest {
    @Mock private Service mServiceMock;

    BroadcastMelcoBootCompleteSender uut;

    @Before
    public void setUp() throws Exception {
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "i"));
        MemberModifier.suppress(MemberMatcher.method(MLog.class, "w"));
        uut = new BroadcastMelcoBootCompleteSender(mServiceMock);
    }

    @Test(expected = NullPointerException.class)
    public void BroadcastMelcoBootCompleteSender_nullArg() {
        new BroadcastMelcoBootCompleteSender(null);
    }

    @Test
    public void sendBroadcast() {
        // when
        uut.sendBroadcast();

        // then
        verify(mServiceMock)
                .sendBroadcastAsUser(
                        any(Intent.class), eq(Process.myUserHandle()), eq(ACCESS_MELCO_SERVICES));
    }
}
