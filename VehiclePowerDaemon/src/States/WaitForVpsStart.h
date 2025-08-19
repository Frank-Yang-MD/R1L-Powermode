/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORVPSSTART_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORVPSSTART_H_

#include "IStateHolder.h"
#include "ISubscriptionManager.h"
#include "VpdStateBase.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 *  Wait for VehiclePowerService to register callback
 */
class WaitForVpsStart : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine* sm,
                             ISubscriptionManager& subscrMgr,
                             IStateHolder& sh);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    void onEnter() override;
    void onSubscribeVps(int) override;
    virtual inline std::string stateName() const override { return std::string("WaitForVpsStart"); }

private:
    ISubscriptionManager& mSubscrMgr;
    IStateHolder& mStateHolder;
    WaitForVpsStart(IVpdStateMachine* sm, ISubscriptionManager& subscrMgr, IStateHolder& sh);
    static std::unique_ptr<VpdStateBase> sInstance;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORVPSSTART_H_
