/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVESTOPCOMPLETEHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVESTOPCOMPLETEHANDLER_H_

#include "IEventHandler.h"
#include "IStopCompleteNative.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/*
 * This adapter handles events of Native Stop Complete
 */
class NativeStopCompleteHandler final : public IEventHandler {
public:
    NativeStopCompleteHandler(IStopCompleteNative& inIStopCompleteNative);

    void Process(int clientId, common::EVpdEvent event) override;

private:
    IStopCompleteNative& mIStopCompleteNative;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_NATIVESTOPCOMPLETEHANDLER_H_
