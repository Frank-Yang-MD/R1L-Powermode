/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITANTIVEAPPSTOPPROCESSINGCOMPLETE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITANTIVEAPPSTOPPROCESSINGCOMPLETE_H_

#include "INotifyHelper.h"
#include "IStateHolder.h"
#include "ISubscriptionManager.h"
#include "IVcpuActions.h"
#include "Timer.h"
#include "VpdStateBase.h"

#include <memory>
#include <unordered_set>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * State S7
 * Wait Stop Processing complete events
 * from Native Apps / Services to
 * Vehicle Power Daemon
 */
class WaitNativeAppStopProcessingComplete : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*,
                             INotifyHelper&,
                             ITimer&,
                             ISubscriptionManager&,
                             IStateHolder& sh,
                             IVcpuActions& va);

    static void uninitInstance() { sInstance.reset(nullptr); }

    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;

    virtual void onTimeout() override;

    /**
     * Transition To S9 (LogDaemonBusy)
     * Process stop complete for all native apps and services
     */
    virtual void onStopCompleteNative(int) override;

    virtual inline std::string stateName() const override
    {
        return std::string("WaitNativeAppStopProcessingComplete");
    }

protected:
    WaitNativeAppStopProcessingComplete(IVpdStateMachine*,
                                        INotifyHelper&,
                                        ITimer&,
                                        ISubscriptionManager&,
                                        IStateHolder& sh,
                                        IVcpuActions& va);

private:
    INotifyHelper& mNotifyHelper;

    /**
     *  Get actual number of native subscribers, exclude VPS
     */
    int getNumberOfNativeSubscribers() const;

    std::vector<int> getNativeSubscribers() const;

    void logNoStopCompletes() const;

    static std::unique_ptr<VpdStateBase> sInstance;

    ISubscriptionManager& mSubscriptionManager;

    std::unordered_set<int> mStopCompleteAppIds;

    ITimer& mTimer;

    /**
     * Number of received 'StopComplete' notification from native apps/services
     */
    int mStopCompleteCount;

    IStateHolder& mStateHolder;
    IVcpuActions& mVcpuActions;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITANTIVEAPPSTOPPROCESSINGCOMPLETE_H_
