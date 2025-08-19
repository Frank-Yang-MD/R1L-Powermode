/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORVPSSTARTCOMPLETE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORVPSSTARTCOMPLETE_H_

#include "INotifyHelper.h"
#include "IPowerStateHolder.h"
#include "ISubscriptionManager.h"
#include "IVcpuActions.h"
#include "VpdStateBase.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IAndroidInit;

/**
 *  Wait for 'start complete' notification from Client
 */
class WaitForVpsStartComplete : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine* sm,
                             INotifyHelper& pNotifyHelper,
                             IPowerStateHolder& powerStateHolder,
                             const IAndroidInit& androidInit,
                             IVcpuActions& va);

    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    void onEnter() override;
    void onExit() override;
    void onStartCompleteVps() override;
    void onStartFailedVps() override;
    virtual inline std::string stateName() const override
    {
        return std::string("WaitForVpsStartComplete");
    }

private:
    INotifyHelper& mNotifyHelper;
    IPowerStateHolder& mPowerStateHolder;
    const IAndroidInit& mAndroidInit;
    IVcpuActions& mVcpuActions;

    WaitForVpsStartComplete(IVpdStateMachine* sm,
                            INotifyHelper&,
                            IPowerStateHolder& powerStateHolder,
                            const IAndroidInit& androidInit,
                            IVcpuActions& va);

    static std::unique_ptr<VpdStateBase> sInstance;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITFORVPSSTARTCOMPLETE_H_
