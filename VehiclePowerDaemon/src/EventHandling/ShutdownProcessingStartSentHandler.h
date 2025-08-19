/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SHUTDOWNPROCESSINGSTARTSENTHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SHUTDOWNPROCESSINGSTARTSENTHANDLER_H_

#include "IEventHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IShutdownProcessingStartSent;

class ShutdownProcessingStartSentHandler final : public IEventHandler {
public:
    ShutdownProcessingStartSentHandler(IShutdownProcessingStartSent& shutdownProcessingStartSent);

    virtual void Process(int clientId, common::EVpdEvent event) override;

private:
    IShutdownProcessingStartSent& mShutdownProcessingStartSent;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SHUTDOWNPROCESSINGSTARTSENTHANDLER_H_
