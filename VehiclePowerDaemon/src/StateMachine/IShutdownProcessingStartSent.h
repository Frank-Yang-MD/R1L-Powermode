/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISHUTDOWNPROCESSINGSTARTSENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISHUTDOWNPROCESSINGSTARTSENT_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Needed to deliver onShutdownProcessingStartSent event to state machine
 */
class IShutdownProcessingStartSent {
public:
    // LCOV_EXCL_START
    virtual ~IShutdownProcessingStartSent() = default;
    // LCOV_EXCL_STOP

    /**
     *  Called from Event Handler after calling CpuComClient::sendShutdownProcessingStart
     */
    virtual void onShutdownProcessingStartSent() = 0;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_ISHUTDOWNPROCESSINGSTARTSENT_H_
