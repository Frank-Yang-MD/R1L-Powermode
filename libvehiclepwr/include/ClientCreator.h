/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_CLIENTCREATOR_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_CLIENTCREATOR_H_

#include "IBootDaemonClient.h"
#include "ICpuComClient.h"
#include "INativeCallback.h"
#include "INativeClient.h"
#include "IVpsCallback.h"
#include "IVpsClient.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class ClientCreator {
public:
    /**
     * Create CpuComClient.
     * @return Pointer to ICpuComClient object.
     */
    static ICpuComClient* CreateCpuComClient();
    /**
     * Create NativeClient.
     * @return Pointer to INativeClient object.
     */
    static INativeClient* CreateNativeClient();
    /**
     * Create VpsClient.
     * @return Pointer to IVpsClient object.
     */
    static IVpsClient* CreateVpsClient();
    /**
     * Create LogdogClient.
     * @return Pointer to INativeClient object.
     */
    static INativeClient* CreateLogdogClient();
    /**
     * Create BootDaemonClient.
     * @return Pointer to IBootDaemonClient object.
     */
    static IBootDaemonClient* CreateBootDaemonClient();
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_CLIENTCREATOR_H_
