/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr;

import android.os.Parcel;
import android.os.Parcelable;

/** Needed to send to ISM current BootDaemon status */
public enum EBootDaemonStatus implements Parcelable {
    ACTIVE,
    FINISH;

    public static final Creator<EBootDaemonStatus> CREATOR =
            new Creator<EBootDaemonStatus>() {
                @Override
                public EBootDaemonStatus createFromParcel(Parcel source) {
                    return EBootDaemonStatus.values()[source.readInt()];
                }

                @Override
                public EBootDaemonStatus[] newArray(int size) {
                    return new EBootDaemonStatus[size];
                }
            };

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(ordinal());
    }

    @Override
    public int describeContents() {
        return 0;
    }
}
