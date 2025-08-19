/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_RESTARTNATIVE_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_RESTARTNATIVE_H_

#include "VpdStateBase.h"

#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class INotifyHelper;
class IStateHolder;

/**
 * @brief The RestartNative class
 */
class RestartNative : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine* sm, INotifyHelper& nh, IStateHolder& sh);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;

    virtual inline std::string stateName() const override { return std::string{"RestartNative"}; }

private:
    RestartNative(IVpdStateMachine* sm, INotifyHelper& nh, IStateHolder& sh);

    static std::unique_ptr<VpdStateBase> sInstance;

    INotifyHelper& mNotifyHelper;
    IStateHolder& mStateHolder;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_RESTARTNATIVE_H_
