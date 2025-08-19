/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INATIVERUNNER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INATIVERUNNER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class INativeRunner {
public:
    // LCOV_EXCL_START
    virtual ~INativeRunner() = default;
    // LCOV_EXCL_STOP

    virtual void run() = 0;
    virtual bool isDone() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INATIVERUNNER_H_
