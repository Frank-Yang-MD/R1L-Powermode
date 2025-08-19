package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.broadcastsender;

import static com.mitsubishielectric.ahu.efw.lib.common.Const.ACCESS_MELCO_SERVICES;
import static com.mitsubishielectric.ahu.efw.lib.common.Const.MELCO_BOOT_COMPLETE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.SEND_BROADCAST_MELCO_BOOT_COMPLETE;
import static com.mitsubishielectric.ahu.efw.vehiclepwrmgr.VPSLogMsg.VPS_FUNCTION_ID;

import android.content.Context;
import android.content.Intent;
import android.os.Process;
import android.os.UserHandle;
import com.mitsubishielectric.ahu.efw.lib.logdogcommonclasslib.MLog;
import java.util.Objects;

/** This class is responsible of sending melco boot complete broadcast */
public final class BroadcastMelcoBootCompleteSender implements IBroadcastSender {
    private final Context mContext;
    private final Intent mIntent = new Intent();
    private final UserHandle mUserHandle = Process.myUserHandle();

    public BroadcastMelcoBootCompleteSender(Context context) {
        mContext =
                Objects.requireNonNull(
                        context, "VPS.BroadcastMelcoBootCompleteSender(): " + "Service is NULL.");
    }

    @Override
    public void sendBroadcast() {
        MLog.i(VPS_FUNCTION_ID, SEND_BROADCAST_MELCO_BOOT_COMPLETE, MELCO_BOOT_COMPLETE);
        mIntent.setAction(MELCO_BOOT_COMPLETE);
        mContext.sendBroadcastAsUser(mIntent, mUserHandle, ACCESS_MELCO_SERVICES);
    }
}
