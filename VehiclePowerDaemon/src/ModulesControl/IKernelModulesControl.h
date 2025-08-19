/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IKERNELMODULESCONTROL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IKERNELMODULESCONTROL_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * @brief Interface to control kernel modules
 */
class IKernelModulesControl {
public:
    // LCOV_EXCL_START
    virtual ~IKernelModulesControl() = default;
    // LCOV_EXCL_STOP

    /**
     * @brief loadModules
     *
     * Loads necessary kernel modules.
     */
    virtual void loadModules() = 0;

    /**
     * @brief isModulesLoaded
     *
     * Checks if all necessary kernels were loaded.
     *
     * @return Returns true if all modules were loaded, false otherwise.
     */
    virtual bool isModulesLoaded() = 0;

    /**
     * @brief unloadModules
     *
     * Unloads some kernel modules in order to prevent blocing trasition
     * to STR state due to bugs in kernel drivers for some devices.
     */
    virtual void unloadModules() = 0;

    /**
     * @brief isModulesUnloaded
     *
     * Checks if all suspend-blocking modules were unloaded.
     *
     * @return Returns true if modules were unloaded, false otherwise.
     */
    virtual bool isModulesUnloaded() = 0;

    /**
     * Use this method to check that ModulesUnloaded
     * verification is correct.
     */
    virtual bool skipModulesUnloadedCheck() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_IKERNELMODULESCONTROL_H_
