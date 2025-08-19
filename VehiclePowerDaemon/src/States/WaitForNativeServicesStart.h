/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORNATIVESERVICESSTART_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORNATIVESERVICESSTART_H_

#include "INotifyHelper.h"
#include "ITimerManager.h"
#include "IVcpuActions.h"
#include "Timer.h"
#include "VpdStateBase.h"

#include "INativeRunner.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class WaitForNativeServicesStart : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*,
                             IVcpuActions&,
                             INativeRunner&,
                             ITimer&,
                             ITimerManager&,
                             INotifyHelper&);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    void onEnter() override;
    void onExit() override;
    void onTimeout() override;

    virtual inline std::string stateName() const override
    {
        return std::string("WaitForNativeServicesStart");
    }

private:
    WaitForNativeServicesStart(IVpdStateMachine*,
                               IVcpuActions&,
                               INativeRunner&,
                               ITimer&,
                               ITimerManager&,
                               INotifyHelper&);

    IVcpuActions& mCpuComActions;
    INativeRunner& mNativeServiceRunner;
    ITimer& mCheckStartNativeTimer;
    ITimerManager& mTimerManager;
    INotifyHelper& mNotifyHelper;

    static std::unique_ptr<VpdStateBase> sInstance;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORNATIVESERVICESSTART_H_
