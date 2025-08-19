/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LOGDAEMONBUSY_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LOGDAEMONBUSY_H_

#include <chrono>
#include <memory>
#include <thread>

#include "ILogDaemonActions.h"
#include "IPowerStateHolder.h"
#include "ITimer.h"
#include "IVcpuActions.h"
#include "VpdStateBase.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * State S8
 * Wait until LogDaemon save all logs to storage
 */
class LogDaemonBusy : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*,
                             ILogDaemonActions&,
                             ITimer&,
                             ITimer&,
                             IVcpuActions& vcpuActions);

    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;
    virtual void onTimeout() override;

    virtual inline std::string stateName() const override { return std::string("LogDaemonBusy"); }

private:
    ILogDaemonActions& mLogDogSavingState;
    ITimer& mLogdogCheckSavingStateTimer;
    ITimer& mLogdogSaveStateTimer;
    IVcpuActions& mVcpuActions;

    LogDaemonBusy(IVpdStateMachine*,
                  ILogDaemonActions&,
                  ITimer&,
                  ITimer&,
                  IVcpuActions& vcpuActions);

    static std::unique_ptr<VpdStateBase> sInstance;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LOGDAEMONBUSY_H_
