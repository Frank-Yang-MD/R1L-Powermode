/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ANDROIDINIT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ANDROIDINIT_H_

#include "IAndroidInit.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class AndroidInit : public IAndroidInit {
public:
    int propertySet(const std::string&, const std::string&) const override;
    int propertyGet(const std::string&, std::string&) const override;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ANDROIDINIT_H_
