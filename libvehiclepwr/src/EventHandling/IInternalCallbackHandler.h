/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IINTERNALCALLBACKHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IINTERNALCALLBACKHANDLER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class IInternalCallback;

/**
 * IInternalCallbackHandler - is an interface to handle internal callback.
 */
class IInternalCallbackHandler {
public:
    virtual ~IInternalCallbackHandler() = default;

    /**
     * Set an object to be used for internal callbacks.
     * @param inInternalCallback This object will get events when they occur.
     */
    virtual void setInternalCallback(IInternalCallback* inCallback) = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IINTERNALCALLBACKHANDLER_H_
