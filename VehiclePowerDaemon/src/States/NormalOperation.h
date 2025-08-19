/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NORMALOPERATION_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NORMALOPERATION_H_

#include "VpdStateBase.h"

#include "IPowerStateHolder.h"
#include "IVpdRestartHolder.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IStateHolder;

/**
 *  Wait for 'supend request' from V-CPU
 */
class NormalOperation : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine* sm,
                             IPowerStateHolder&,
                             IVpdRestartHolder&,
                             IStateHolder&);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    void onEnter() override;
    void onPowerStateChange(PowerState) override;
    void onDisconnectVps() override;

    virtual inline std::string stateName() const override { return std::string("NormalOperation"); }

private:
    NormalOperation(IVpdStateMachine* sm, IPowerStateHolder&, IVpdRestartHolder&, IStateHolder&);

    static std::unique_ptr<VpdStateBase> sInstance;

    IPowerStateHolder& mPowerStateHolder;
    IVpdRestartHolder& mVpdRestartHolder;
    IStateHolder& mStateHolder;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NORMALOPERATION_H_
