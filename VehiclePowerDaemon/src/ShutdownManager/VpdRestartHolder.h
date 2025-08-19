/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPDRESTARTHOLDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPDRESTARTHOLDER_H_

#include "IVpdRestartHolder.h"

#include "IAndroidInit.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * @brief The VpdRestartHolder class holds a flag which whether VPD was started
 * before or not and provides possibility to obtain a value of this flag.
 */
class VpdRestartHolder final : public IVpdRestartHolder {
public:
    VpdRestartHolder(IAndroidInit&);

    virtual void vpdStarted() override;
    virtual bool isVpdRestarted() override;

private:
    IAndroidInit& mAndroidInit;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_IMPL_VPDRESTARTHOLDER_H_
