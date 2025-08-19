/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUMEPROCESSINGSTARTCOMPLETESTATUSRECEIVER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUMEPROCESSINGSTARTCOMPLETESTATUSRECEIVER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class IEventReceiver;

class ResumeProcessingStartCompleteStatusReceiver final {
public:
    ResumeProcessingStartCompleteStatusReceiver(IEventReceiver&);

    void onDeliveryStatus(bool);

private:
    IEventReceiver& mEventReceiver;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_RESUMEPROCESSINGSTARTCOMPLETESTATUSRECEIVER_H_
