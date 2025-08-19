/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IANDROIDINIT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IANDROIDINIT_H_

#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IAndroidInit {
public:
    // LCOV_EXCL_START
    virtual ~IAndroidInit() = default;
    // LCOV_EXCL_STOP

    virtual int propertySet(const std::string&, const std::string&) const = 0;
    virtual int propertyGet(const std::string&, std::string&) const = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IANDROIDINIT_H_
