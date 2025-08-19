/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INITIALSTATE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INITIALSTATE_H_

#include "ICpuComControl.h"
#include "IVpdRestartHolder.h"
#include "IWakeLockManager.h"
#include "IWakeUpEventDetector.h"
#include "VpdStateBase.h"

#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ITimerManager;

/**
 * Initial state.
 */
class InitialState : public VpdStateBase {
    static std::unique_ptr<VpdStateBase> sInstance;
    IWakeLockManager& mWakeLockManager;
    IVpdRestartHolder& mVpdRestartHolder;
    ICpuCommControl& mCpuComControl;
    IWakeUpEventDetector& mWakeUpEventDetector;
    ITimerManager& mTimerManager;

public:
    static void initInstance(IVpdStateMachine* sm,
                             IWakeLockManager& lm,
                             IVpdRestartHolder& vrh,
                             ICpuCommControl& ccc,
                             IWakeUpEventDetector& wed,
                             ITimerManager& tm);

    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    InitialState(IVpdStateMachine* sm,
                 IWakeLockManager& lm,
                 IVpdRestartHolder& vrh,
                 ICpuCommControl& ccc,
                 IWakeUpEventDetector& wed,
                 ITimerManager& tm);

    virtual void onTimeout() override;

    virtual inline std::string stateName() const override { return std::string("InitialState"); }
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_INITIALSTATE_H_
