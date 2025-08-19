/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ETimerType.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

static const char* timerTypeToString[] = {
    "None",
    "ADTRTimer",
    "WDTRTimer",
};

const char* toString(const ETimerType timerType)
{
    return timerTypeToString[static_cast<uint8_t>(timerType)];
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
