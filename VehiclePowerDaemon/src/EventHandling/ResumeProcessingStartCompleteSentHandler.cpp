/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ResumeProcessingStartCompleteSentHandler.h"

#include "IResumeProcessingStartCompleteSent.h"
#include "VehiclePowerDaemonLog.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

ResumeProcessingStartCompleteSentHandler::ResumeProcessingStartCompleteSentHandler(
    IResumeProcessingStartCompleteSent& resumeProcessingStartCompleteSent)
    : mResumeProcessingStartCompleteSent{resumeProcessingStartCompleteSent}
{
}

void ResumeProcessingStartCompleteSentHandler::Process(int, common::EVpdEvent event)
{
    if (common::EVpdEvent::ResumeProcessingStartCompleteSentSuccess == event) {
        mResumeProcessingStartCompleteSent.onResumeProcessingStartCompleteSentSuccess();
    }
    else if (common::EVpdEvent::ResumeProcessingStartCompleteSentFailure == event) {
        mResumeProcessingStartCompleteSent.onResumeProcessingStartCompleteSentFailure();
    }
    else {
        MLOGW(LogFuncId::VPD,
              LogId::ResumeProcessingStartCompleteSentHandler_Process_Wrong_event_index, event);
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
