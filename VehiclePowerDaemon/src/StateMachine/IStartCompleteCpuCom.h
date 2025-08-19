/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTARTCOMPLETECPUCOM_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTARTCOMPLETECPUCOM_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IStartCompleteCpuCom {
public:
    // LCOV_EXCL_START
    virtual ~IStartCompleteCpuCom() = default;
    // LCOV_EXCL_STOP

    virtual void onStartCompleteCpuCom() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISTARTCOMPLETECPUCOM_H_
