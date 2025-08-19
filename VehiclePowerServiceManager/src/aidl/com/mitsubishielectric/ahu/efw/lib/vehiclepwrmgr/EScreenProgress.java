/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr;

import android.os.Parcel;
import android.os.Parcelable;

/** Needed to send to ISM current screen progress */
public enum EScreenProgress implements Parcelable {
    UNKNOWN,
    BLACK_SCREEN,
    STARTUP_ANIMATION,
    DISCLAIMER;

    public static final Creator<EScreenProgress> CREATOR =
            new Creator<EScreenProgress>() {
                @Override
                public EScreenProgress createFromParcel(Parcel source) {
                    return EScreenProgress.values()[source.readInt()];
                }

                @Override
                public EScreenProgress[] newArray(int size) {
                    return new EScreenProgress[size];
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
