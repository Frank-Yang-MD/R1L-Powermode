/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IBOOTDCALLBACKHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IBOOTDCALLBACKHANDLER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class IBootDaemonCallback;

/**
 * IBootdCallbackHandler - is an interface to handle set IBootDaemonCallback.
 */
class IBootdCallbackHandler {
public:
    virtual ~IBootdCallbackHandler() = default;

    /**
     * Set IBootDaemonCallback.
     */
    virtual void setBootDaemonCallback(IBootDaemonCallback*) = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IBOOTDCALLBACKHANDLER_H_
