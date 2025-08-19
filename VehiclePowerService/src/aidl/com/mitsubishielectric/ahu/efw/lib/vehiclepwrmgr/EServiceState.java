/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr;

import android.os.Parcel;
import android.os.Parcelable;

public enum EServiceState implements Parcelable {
    NOT_STARTED,
    STARTED,
    STOPPED;

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(ordinal());
    }

    @Override
    public int describeContents() {
        return 0;
    }

    public static final Creator<EServiceState> CREATOR =
            new Creator<EServiceState>() {
                @Override
                public EServiceState createFromParcel(Parcel source) {
                    return EServiceState.values()[source.readInt()];
                }

                @Override
                public EServiceState[] newArray(int size) {
                    return new EServiceState[size];
                }
            };
}
