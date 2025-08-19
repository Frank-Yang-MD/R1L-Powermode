/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMOUNT_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMOUNT_H

#include "VpdStateBase.h"
#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class INotifyHelper;

/**
 * @brief The WaitForMount class
 */
class WaitForMount : public VpdStateBase {
public:
    static void initInstance(IVpdStateMachine* sm, INotifyHelper& notifyHelper);
    static void uninitInstance() { sInstance.reset(nullptr); }
    static VpdStateBase* getInstance() { return sInstance.get(); }

    virtual void onEnter() override;

    virtual inline std::string stateName() const override { return std::string{"WaitForMount"}; }

private:
    WaitForMount(IVpdStateMachine* sm, INotifyHelper& notifyHelper);

    static std::unique_ptr<VpdStateBase> sInstance;

    INotifyHelper& mNotifyHelper;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_WAITFORMOUNT_H
