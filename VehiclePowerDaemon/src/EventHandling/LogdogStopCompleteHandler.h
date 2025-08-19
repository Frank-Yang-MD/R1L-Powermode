/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LOGSTOPCOMPLETEHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LOGSTOPCOMPLETEHANDLER_H_

#include "IEventHandler.h"
#include "IStopCompleteLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This adapter handles events of Log Stop Complete
 */
class LogdogStopCompleteHandler final : public IEventHandler {
public:
    LogdogStopCompleteHandler(IStopCompleteLog& inIStopCompleteLog);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    IStopCompleteLog& mIStopCompleteLog;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LOGSTOPCOMPLETEHANDLER_H_
