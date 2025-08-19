/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKINATIVECLIENT_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKINATIVECLIENT_H_

#include "INativeClient.h"

#include <gmock/gmock.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

class MockINativeClient : public INativeClient {
public:
    MOCK_METHOD0(GetVPDState, common::EVpdState());
    MOCK_METHOD0(stopProcessingComplete, void());
    MOCK_METHOD1(subscribe, void(INativeCallback* callback));
    MOCK_METHOD0(unsubscribe, void());
};

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_LIB_MOCKINATIVECLIENT_H_
