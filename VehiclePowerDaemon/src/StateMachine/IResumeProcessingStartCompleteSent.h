/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IRESUMEPROCESSINGSTARTCOMPLETESENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IRESUMEPROCESSINGSTARTCOMPLETESENT_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Needed to create new state SendShutdownProcessingStart
 */
class IResumeProcessingStartCompleteSent {
public:
    // LCOV_EXCL_START
    virtual ~IResumeProcessingStartCompleteSent() = default;
    // LCOV_EXCL_STOP

    /**
     *  Called from Event Handler after calling SendShutdownProcessingStart
     */
    virtual void onResumeProcessingStartCompleteSentSuccess() = 0;
    virtual void onResumeProcessingStartCompleteSentFailure() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IRESUMEPROCESSINGSTARTCOMPLETESENT_H_
