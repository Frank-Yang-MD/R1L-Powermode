/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTOPCOMPLETELOG_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTOPCOMPLETELOG_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IStopCompleteLog {
public:
    // LCOV_EXCL_START
    virtual ~IStopCompleteLog() = default;
    // LCOV_EXCL_STOP

    virtual void onStopCompleteLog() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTOPCOMPLETELOG_H_
