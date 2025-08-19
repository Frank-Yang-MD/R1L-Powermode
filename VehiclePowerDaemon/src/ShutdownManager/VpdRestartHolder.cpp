/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpdRestartHolder.h"

#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

static const std::string VPD_STARTED_PROP = "vendor.vpd_started";
static const std::string VPD_STARTED_PROP_VALUE = "1";

VpdRestartHolder::VpdRestartHolder(IAndroidInit& androidInit)
    : mAndroidInit{androidInit}
{
}

void VpdRestartHolder::vpdStarted()
{
    mAndroidInit.propertySet(VPD_STARTED_PROP, VPD_STARTED_PROP_VALUE);
}

bool VpdRestartHolder::isVpdRestarted()
{
    std::string vpdStarted;
    mAndroidInit.propertyGet(VPD_STARTED_PROP, vpdStarted);
    return vpdStarted == VPD_STARTED_PROP_VALUE;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
