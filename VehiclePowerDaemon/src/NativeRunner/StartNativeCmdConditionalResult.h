/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECMDCONDITIONALRESULT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECMDCONDITIONALRESULT_H_

#include "ICommandResult.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StartNativeCmdConditionalResult : public ICommandResult {
public:
    StartNativeCmdConditionalResult();

    bool isDone() override;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STARTNATIVECMDCONDITIONALRESULT_H_
