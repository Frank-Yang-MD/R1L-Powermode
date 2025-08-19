/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "AndroidInit.h"
#include "VehiclePowerDaemonLog.h"

#include <cutils/properties.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

int AndroidInit::propertySet(const std::string& propertyName,
                             const std::string& propertyValue) const
{
    MLOGI(LogFuncId::VPD, LogId::AndroidInit_propertySet_Setting_property,
          "'" + propertyName + "', value '" + propertyValue + "'");
    return property_set(propertyName.c_str(), propertyValue.c_str());
}

int AndroidInit::propertyGet(const std::string& propertyName, std::string& propertyValue) const
{
    char value[PROPERTY_VALUE_MAX] = {'\0'};
    int returnCode = property_get(propertyName.c_str(), value, nullptr);
    propertyValue = value;
    return returnCode;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
