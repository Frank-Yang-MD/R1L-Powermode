/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUME_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUME_H_

#include <memory>

#include "EVpdState.h"
#include "INotifyHelper.h"
#include "IPowerStateHolder.h"
#include "IWakeUpManager.h"
#include "VpdStateBase.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class Resume : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*, IPowerStateHolder& powerStateHolder);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual inline std::string stateName() const override { return std::string("Resume"); }

private:
    Resume(IVpdStateMachine*, IPowerStateHolder& powerStateHolder);
    static std::unique_ptr<VpdStateBase> sInstance;
    IPowerStateHolder& mPowerStateHolder;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUME_H_
