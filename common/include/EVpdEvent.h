/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVPDEVENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVPDEVENT_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace common {

enum EVpdEvent {
    Unsupported = 0x00,  // Unsuported type of event

    // Interaction with all native applications
    SubscribeNative,     // Subscription request from native services/apps
    UnsubscribeNative,   // Unsubscription request from native services/apps
    StopCompleteNative,  // Native services/apps stop complete notification

    // CPUCommunication events
    CpuComStarted,  // Start complete notification from CpuCom

    // Interaction with Vehicle Power Service
    SubscribeVps,                   // Subscription request from VPS
    UnsubscribeVps,                 // Unsubscription request from VPS
    VpsStarted,                     // Start complete notification from VPS
    StartFailedVps,                 // Start Failed notification from VPS
    AppStopCompleteVps,             // App services stop complete notification from VPS
    FwResumeCompleteVps,            // FW resume complete from VPS
    FwRestartCompleteVps,           // FW restart complete from VPS
    AppResumeCompleteVps,           // App resume complete from VPS
    AppRestartCompleteVps,          // App restart complete from VPS
    FwStopCompleteVps,              // FW services stop complete notification from VPS
    StopFailedVps,                  // Stop complete notification from VPS
    VpsReady,                       // HMI/VPS ready notification from VPS
    FullHmiDisplaying,              // Full Hmi Displaying notification from VPS
    UnmountComplete,                // Unmount Complete notification from VPS
    EndEarlyHmi,                    // Request to end early HMI from VPS
    ForcedCompletionEarlyServices,  // Request to Forced completion of Early Services from VPS

    // BootDaemon events
    SubscribeBootDaemon,    // Subscription request from BootDaemon
    UnsubscribeBootDaemon,  // Unsubscription request from BootDaemon
    handOffResponsibility,  // Take over responsibility to HMI Main Service from BootDaemon
    handOffResponsibilityStartupAnimationDisplayed,  // Take over responsibility to HMI Main Service
                                                     // with startup animation from BootDaemon
    handOffResponsibilityStartupAnimationAndDiDisplayed,  // Take over responsibility to HMI Main
                                                          // Service with startup animation/DI from
                                                          // BootDaemon

    // Logger events
    SubscribeLogdog,     // Subscription request from Logdog
    UnsubscribeLogdog,   // Unsubscription request from Logdog
    StopCompleteLogdog,  // Logdog stop complete notification

    /*
     *  Internal Vehicle Power Deamon events
     */
    // Internal events
    Timeout,     // Timeout internal event
    Disconnect,  // Disconection event

    // VCPU events
    Shutdown,                                  // Shutdown request from V-CPU
    Suspend,                                   // Suspend request from V-CPU
    Reboot,                                    // Reboot request from V-CPU
    RebootRecovery,                            // Reboot into recovery request from V-CPU
    ShutdownCancel,                            // Shutdown cancel request from V-CPU
    ShutdownProcessingStartSent,               // Sending confirmation response from CpuCom
    ResumeProcessingStartCompleteSentSuccess,  // Sending confirmation response from CpuCom
    ResumeProcessingStartCompleteSentFailure,  // Sending confirmation response from CpuCom

    // Kernel events
    WakeUp,  // System wake up event

    EVPDEVENT_COUNT,
};

inline EVpdEvent convertToVpdEvent(unsigned char ch)
{
    return ch >= EVPDEVENT_COUNT ? Unsupported : static_cast<EVpdEvent>(ch);
}

inline unsigned char convertToChar(EVpdEvent event) { return static_cast<unsigned char>(event); }

}  // namespace common
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_EVPDEVENT_H_
