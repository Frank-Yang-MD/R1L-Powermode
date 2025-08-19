/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_KERNELMODULESCONTROL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_KERNELMODULESCONTROL_H_

#include "IKernelModulesControl.h"

#include <IAndroidInit.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * @brief The KernelModulesControl class
 *
 * Control kernel modules loading/unloading by triggering
 * corresponding properties. Also checks scrpits execution status
 * using properties.
 */
class KernelModulesControl final : public IKernelModulesControl {
public:
    KernelModulesControl(IAndroidInit&);

    virtual void loadModules() override;
    virtual bool isModulesLoaded() override;

    virtual void unloadModules() override;
    virtual bool isModulesUnloaded() override;

    virtual bool skipModulesUnloadedCheck() override;

private:
    IAndroidInit& mAndroidInit;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_KERNELMODULESCONTROL_H_
