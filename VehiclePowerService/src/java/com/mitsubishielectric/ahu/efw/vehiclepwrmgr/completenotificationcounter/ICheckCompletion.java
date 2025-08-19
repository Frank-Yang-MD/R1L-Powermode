package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.completenotificationcounter;

/** This is interface to get current status of StopCompleteCounter. */
public interface ICheckCompletion {
    /**
     * Indicates if all subscribed App services has reported stopComplete
     *
     * @return true if all subscribers reported, false otherwise
     */
    boolean isComplete();

    /** Logs all subscribers which didn't report complete notification */
    void logNotCompleted();
}
