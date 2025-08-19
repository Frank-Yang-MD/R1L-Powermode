/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_POLL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_POLL_H_

#include <memory>

#include "IClock.h"
#include "ITimeoutControl.h"
#include "IWakeLockManager.h"
#include "IWakeUpEventDetector.h"
#include "Timer.h"
#include "VpdStateBase.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IShutdownAction;

/**
 * State PollingWakeUp
 */
class PollingWakeUp : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*,
                             IWakeUpEventDetector&,
                             IWakeLockManager&,
                             IShutdownAction&);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual void onWakeUp() override;
    virtual void onExit() override;

    virtual inline std::string stateName() const override { return std::string("PollingWakeUp"); }

private:
    [[maybe_unused]] IWakeUpEventDetector& mWakeUpEventDetector;
    IWakeLockManager& mWakeLockManager;
    IShutdownAction& mShutdownAction;
    static std::unique_ptr<VpdStateBase> sInstance;

    PollingWakeUp(IVpdStateMachine*, IWakeUpEventDetector&, IWakeLockManager&, IShutdownAction&);
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_POLL_H_
