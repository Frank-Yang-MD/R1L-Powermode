/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SHUTDOWNPROCESSINGSTARTSTATUSRECEIVER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SHUTDOWNPROCESSINGSTARTSTATUSRECEIVER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IEventReceiver;

class ShutdownProcessingStartStatusReceiver final {
public:
    ShutdownProcessingStartStatusReceiver(IEventReceiver&);

    void onDeliveryStatus(bool);

private:
    IEventReceiver& mEventReceiver;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_SHUTDOWNPROCESSINGSTARTSTATUSRECEIVER_H_
