/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STATEHOLDER_H
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STATEHOLDER_H

#include "EVpdState.h"
#include "IStateHolder.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StateHolder : public IStateHolder {
public:
    StateHolder();

    virtual common::EVpdState getVpdState() const override;
    virtual void setVpdState(common::EVpdState vpdState) override;

private:
    common::EVpdState mCurrentState;

    inline int currentStateInt() const { return static_cast<int>(mCurrentState); }
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STATEHOLDER_H
