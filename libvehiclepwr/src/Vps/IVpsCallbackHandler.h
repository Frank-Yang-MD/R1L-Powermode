/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPSCALLBACKHANDLER_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPSCALLBACKHANDLER_H_

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class IVpsCallback;

/**
 * IVpsCallbackHandler - is a interface to handle set IVpsCallback.
 */
class IVpsCallbackHandler {
public:
    virtual ~IVpsCallbackHandler() = default;

    /**
     * Set IVpsCallback.
     */
    virtual void setVpsCallback(IVpsCallback*) = 0;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_IVPSCALLBACKHANDLER_H_
