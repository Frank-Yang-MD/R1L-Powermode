/*
 * COPYRIGHT (C) 2023 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "VpdStateMachineContext.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

void VpdStateMachineContext::setVpsDisconnected(const bool disconnected)
{
    mIsVpsDisconnected = disconnected;
}

bool VpdStateMachineContext::isVpsDisconnected() { return mIsVpsDisconnected; }

void VpdStateMachineContext::setVpsStartCompleted(bool startCompleted)
{
    mIsVpsStartCompleted = mIsVpsDisconnected && startCompleted;
}

bool VpdStateMachineContext::isVpsStartCompleted()
{
    return mIsVpsStartCompleted;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
