/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_ICLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_ICLIENT_H_

#include "IConnectionControl.h"
#include "IInternalCallbackHandler.h"
#include "IVpdEventSender.h"
#include "IVpdStateProvider.h"

#include "IBootdCallbackHandler.h"
#include "INativeCallbackHandler.h"
#include "IVpsCallbackHandler.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

/**
 * IClient interface is used to unite IVpdEventSender, IInternalCallbackHandler and
 * IConnectionControl into one interface and provide it to all clients as a facade for interaction
 * with Vehicle Power Deamon.
 */
class IClient
    : public IVpdEventSender
    , public IInternalCallbackHandler
    , public IConnectionControl
    , public IVpdStateProvider
    , public IBootdCallbackHandler
    , public INativeCallbackHandler
    , public IVpsCallbackHandler {
public:
    virtual ~IClient() override = default;
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_ICLIENT_H_
