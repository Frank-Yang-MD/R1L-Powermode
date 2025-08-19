/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

// LCOV_EXCL_START
// This is excluded from a unit test coverage report because it's only interaction code.

#include "ClientInteraction.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using common::EVpdState;

ClientInteraction::ClientInteraction(IVpdNotificationSender& vpdNotificationSender)
    : mStateSender{vpdNotificationSender}
    , mClientActions{mStateSender}
{
}

IClientActions& ClientInteraction::getClientActions() { return mClientActions; }

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

// LCOV_EXCL_STOP
