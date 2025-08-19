/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

package com.mitsubishielectric.ahu.efw.vehiclepwrmgr.eventqueue;

/** Internal events for Vehicle Power Service */
public enum IncomingEvents {
    /** APP_RESTART notification from Vehicle Power Daemon */
    APP_RESTART,

    /** APP_RESUME notification from Vehicle Power Daemon */
    APP_RESUME,

    /** APP_START_COLDBOOT notification from Vehicle Power Daemon */
    APP_START_COLDBOOT,

    /** APP_STOP notification from Vehicle Power Daemon */
    APP_STOP,

    /** APP_SERVICES_STOP_COMPLETE notification from Vehicle Power Daemon */
    APP_SERVICES_STOP_COMPLETE,

    /** FW_RESUME notification from Vehicle Power Daemon */
    FW_RESUME,

    /** Request to restart FW services from Vehicle Power Daemon */
    FW_RESTART,

    /** Request to stop FW services from Vehicle Power Daemon */
    FW_STOP,

    /** COMPLETION_TIMEOUT notification from Vehicle Power Daemon */
    COMPLETION_TIMEOUT,

    /** All services started notification */
    NORMAL_EFW_SERVICES_STARTED,

    /** All apps started notification */
    LATE_APP_SERVICES_STARTED,

    /** Early apps started notification */
    NORMAL_APP_SERVICES_STARTED,

    /** Late FW services started notification */
    LATE_EFW_SERVICES_STARTED,

    /** EFW services stop complete notification */
    EFW_SERVICES_STOP_COMPLETE,

    /** EFW app services resume complete notification */
    EFW_APP_SERVICES_RESUME_COMPLETE,

    /** EFW services resume complete notification */
    EFW_SERVICES_RESUME_COMPLETE,

    /** EFW app services restart complete notification */
    EFW_APP_SERVICES_RESTART_COMPLETE,

    /** EFW services restart complete notification */
    EFW_SERVICES_RESTART_COMPLETE,

    /** Request to unmount all mounted volumes */
    REQUEST_UNMOUNT,

    /** Request to mount all previously unmounted volumes */
    REQUEST_MOUNT,
}
