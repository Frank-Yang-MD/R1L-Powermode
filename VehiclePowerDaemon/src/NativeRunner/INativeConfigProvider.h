/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INATIVECONFIGPROVIDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INATIVECONFIGPROVIDER_H_

#include <string>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class INativeConfigProvider {
public:
    // LCOV_EXCL_START
    virtual ~INativeConfigProvider() = default;
    // LCOV_EXCL_STOP

    virtual std::vector<std::string> loadConfig() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INATIVECONFIGPROVIDER_H_
