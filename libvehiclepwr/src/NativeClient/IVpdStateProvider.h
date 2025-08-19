/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPDSTATEPROVIDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPDSTATEPROVIDER_H_

#include "EVpdState.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

/**
 * IVpdStateProvider - is an interface to obtain current VPD state.
 */
class IVpdStateProvider {
public:
    virtual ~IVpdStateProvider() = default;

    /**
     * Get current VPD state.
     */
    virtual common::EVpdState getVpdState() = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPDSTATEPROVIDER_H_
