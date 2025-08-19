/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VCPUPOWERSTATECHANGEHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VCPUPOWERSTATECHANGEHANDLER_H_

#include "IEventHandler.h"

#include <IPowerStateChangeListener.h>
#include <IPowerStateHolder.h>

#include <utility>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * @brief The VcpuPowerStateChangeHandler class is intended to process
 * any power state related changes from VCPU, convert them into inner
 * power state and pass to corresponding listener
 */
class VcpuPowerStateChangeHandler : public IEventHandler {
public:
    VcpuPowerStateChangeHandler(IPowerStateChangeListener&,
                                IPowerStateChangeListener&,
                                IPowerStateChangeListener&);

    /**
     * @brief Process new VPD state
     */
    virtual void Process(int, common::EVpdEvent) override;

private:
    std::pair<PowerState, bool> vpdEventToPowerState(common::EVpdEvent) const;

    IPowerStateChangeListener& mStateMachine;
    IPowerStateChangeListener& mPowerStateHolder;
    IPowerStateChangeListener& mPowerStateChangeListener;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VCPUPOWERSTATECHANGEHANDLER_H_
