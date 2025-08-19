/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STATESENDER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STATESENDER_H_

#include "IStateSender.h"
#include "IVpdNotificationSender.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class StateSender : public IStateSender {
public:
    StateSender(IVpdNotificationSender&);

public:
    virtual bool sendState(int clientID, common::EVpdState state) override;

private:
    IVpdNotificationSender& mVpdNotificationSender;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_STATESENDER_H_
