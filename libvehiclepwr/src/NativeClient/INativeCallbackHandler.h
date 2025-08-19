/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INATIVECALLBACKHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INATIVECALLBACKHANDLER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class INativeCallback;

/**
 * INativeCallbackHandler - is an interface to handle set INativeCallback.
 */
class INativeCallbackHandler {
public:
    virtual ~INativeCallbackHandler() = default;

    /**
     * Set native callback.
     */
    virtual void setNativeCallback(INativeCallback*) = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_INATIVECALLBACKHANDLER_H_
