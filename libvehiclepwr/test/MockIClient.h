/*
 * COPYRIGHT (C) 2021 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKICLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKICLIENT_H_

#include <gmock/gmock.h>

#include "IClient.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class MockIClient : public IClient {
public:
    // IVpdEventSender interface
    MOCK_METHOD1(sendVpdEvent, void(common::EVpdEvent));

    // IInternalCallbackHandler interface
    MOCK_METHOD1(setInternalCallback, void(IInternalCallback*));

    // IConnectionControl interface
    MOCK_METHOD0(startConnection, void());

    // IVpdStateProvider interface
    MOCK_METHOD0(getVpdState, common::EVpdState());

    // IBootdCallbackHandler interface
    MOCK_METHOD1(setBootDaemonCallback, void(IBootDaemonCallback*));

    // INativeCallbackHandler interface
    MOCK_METHOD1(setNativeCallback, void(INativeCallback*));

    // IVpsCallbackHandler interface
    MOCK_METHOD1(setVpsCallback, void(IVpsCallback*));
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKICLIENT_H_
