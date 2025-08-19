/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.lib.vehiclepwrmgr;

import android.os.Parcel;
import android.os.Parcelable;

/** Vehicle power service state */
public enum EVpsState implements Parcelable {
    APP_START_NA,
    APP_START_COLDBOOT,
    APP_STOP,
    APP_RESUME,
    APP_RESTART;

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(ordinal());
    }

    public static final Parcelable.Creator<EVpsState> CREATOR =
            new Parcelable.Creator<EVpsState>() {
                @Override
                public EVpsState createFromParcel(final Parcel source) {
                    return EVpsState.values()[source.readInt()];
                }

                @Override
                public EVpsState[] newArray(final int size) {
                    return new EVpsState[size];
                }
            };
}
