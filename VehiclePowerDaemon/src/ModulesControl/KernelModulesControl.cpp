/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "KernelModulesControl.h"

#include <string>

const std::string MODULES_LOAD_PROP_NAME = "vendor.sys.wakeup.request";
const std::string MODULES_LOADED_PROP_VALUE = "0";

const std::string MODULES_UNLOAD_PROP_NAME = "vendor.sys.suspend.request";
const std::string MODULES_UNLOADED_PROP_VALUE = "0";

const std::string MODULES_UNLOADED_PROP_VALUE_SKIP = "";

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

KernelModulesControl::KernelModulesControl(IAndroidInit& androidInit)
    : mAndroidInit{androidInit}
{
}

void KernelModulesControl::loadModules()
{
    // Set trigger propertry value to 0 at first in order to
    // guarantee that script will be triggered even if current
    // value of the property is set to 1 for some reason.
    mAndroidInit.propertySet(MODULES_LOAD_PROP_NAME, "0");
    mAndroidInit.propertySet(MODULES_LOAD_PROP_NAME, "1");
}

bool KernelModulesControl::isModulesLoaded()
{
    std::string propVal;
    mAndroidInit.propertyGet(MODULES_LOAD_PROP_NAME, propVal);

    return propVal == MODULES_LOADED_PROP_VALUE;
}

void KernelModulesControl::unloadModules()
{
    // Set trigger propertry value to 0 at first in order to
    // guarantee that script will be triggered even if current
    // value of the property is set to 1 for some reason.
    mAndroidInit.propertySet(MODULES_UNLOAD_PROP_NAME, "0");
    mAndroidInit.propertySet(MODULES_UNLOAD_PROP_NAME, "1");
}

bool KernelModulesControl::isModulesUnloaded()
{
    std::string propVal;
    mAndroidInit.propertyGet(MODULES_UNLOAD_PROP_NAME, propVal);

    return propVal == MODULES_UNLOADED_PROP_VALUE;
}

bool KernelModulesControl::skipModulesUnloadedCheck()
{
    std::string propVal;
    mAndroidInit.propertyGet(MODULES_UNLOAD_PROP_NAME, propVal);

    return propVal == MODULES_UNLOADED_PROP_VALUE_SKIP;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
