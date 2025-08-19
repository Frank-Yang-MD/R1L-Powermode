/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITBEFORESENDINGFWSTOPVPS_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITBEFORESENDINGFWSTOPVPS_H

#include "VpdStateBase.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IVpdStateMachine;
class ITimer;

/**
 *  Wait 300ms for transition to WaitForFwStopVps state.
 */

class WaitBeforeSendingFwStopVps final : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine*, ITimer&);

    static void uninitInstance() { sInstance.reset(nullptr); }

    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;

    virtual void onTimeout() override;

    virtual inline std::string stateName() const override
    {
        return std::string("WaitBeforeSendingFwStopVps");
    }

private:
    WaitBeforeSendingFwStopVps(IVpdStateMachine*, ITimer&);

    static std::unique_ptr<VpdStateBase> sInstance;

    ITimer& mTimer;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_WAITBEFORESENDINGFWSTOPVPS_H
