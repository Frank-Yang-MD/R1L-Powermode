/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUMEPROCESSINGSTARTCOMPLETESENTHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUMEPROCESSINGSTARTCOMPLETESENTHANDLER_H_

#include "IEventHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IResumeProcessingStartCompleteSent;

class ResumeProcessingStartCompleteSentHandler final : public IEventHandler {
public:
    ResumeProcessingStartCompleteSentHandler(
        IResumeProcessingStartCompleteSent& resumeProcessingStartCompleteSent);

    virtual void Process(int clientId, common::EVpdEvent event) override;

private:
    IResumeProcessingStartCompleteSent& mResumeProcessingStartCompleteSent;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUMEPROCESSINGSTARTCOMPLETESENTHANDLER_H_
