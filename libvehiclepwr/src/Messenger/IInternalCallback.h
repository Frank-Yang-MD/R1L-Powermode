/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INATIVECALLBACKINTERNAL_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INATIVECALLBACKINTERNAL_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

/**
 * IInternalCallback - is a callback to be called the connect or disconnect
 * occur.
 */
class IInternalCallback {
public:
    virtual ~IInternalCallback() = default;

    /**
     * Notify when the connection established.
     */
    virtual void onConnect() = 0;
    /**
     * Notify when the connection lost.
     */
    virtual void onDisconnect() = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INATIVECALLBACKINTERNAL_H_
