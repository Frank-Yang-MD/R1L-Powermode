/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTARTCOMPLETEVPS_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTARTCOMPLETEVPS_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IStartCompleteVps {
public:
    // LCOV_EXCL_START
    virtual ~IStartCompleteVps() = default;
    // LCOV_EXCL_STOP

    virtual void onStartCompleteVps() = 0;
    virtual void onStartFailedVps() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTARTCOMPLETEVPS_H_
