/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONDITION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONDITION_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ICondition {
public:
    // LCOV_EXCL_START
    virtual ~ICondition() = default;
    // LCOV_EXCL_STOP
    virtual bool isSatisfied() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ICONDITION_H_
