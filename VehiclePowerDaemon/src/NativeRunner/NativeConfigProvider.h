/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVECONFIGPROVIDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVECONFIGPROVIDER_H_

#include "INativeConfigProvider.h"
#include "IVehicleConfigProvider.h"

#include <string>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class NativeConfigProvider : public INativeConfigProvider {
public:
    NativeConfigProvider(IVehicleConfigProvider&);

    std::vector<std::string> loadConfig() override;

private:
    static bool isValidLine(const std::string&);

    IVehicleConfigProvider& mConfigProvider;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVECONFIGPROVIDER_H_
