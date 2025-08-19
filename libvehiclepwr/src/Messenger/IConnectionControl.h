/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_ICONNECTIONCONTROL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_ICONNECTIONCONTROL_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

/**
 * IConnectionControl interface is intended to control connection of
 * the Client to Vehicle Power Daemon when the Client is ready.
 */
class IConnectionControl {
public:
    virtual ~IConnectionControl() = default;

    /**
     * Start connection of the Client to Vehicle Power Daemon.
     */
    virtual void startConnection() = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_ICONNECTIONCONTROL_H_