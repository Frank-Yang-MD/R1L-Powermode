/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <memory>

#include "MockIClient.h"

#include "CpuComClient.h"

using namespace std;
using namespace com::mitsubishielectric::ahu::vehiclepwrmgr::common;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace lib {

using testing::NiceMock;

class CpuComClientTest : public testing::Test {
protected:
    CpuComClientTest()
        : mClientMock{std::make_unique<NiceMock<MockIClient>>()}
        , mClientMockPtr{mClientMock.get()}
        , mCpuComClient{std::move(mClientMock)}
    {
    }

    ~CpuComClientTest() { testing::Mock::VerifyAndClearExpectations(mClientMockPtr); }

    std::unique_ptr<NiceMock<MockIClient>> mClientMock;
    NiceMock<MockIClient>* mClientMockPtr;
    CpuComClient mCpuComClient;
};

/* Verifies that the function sendVpdEvent() can be called from
   CpuComClient.
*/
TEST_F(CpuComClientTest, sendVpdEventCall)
{
    EXPECT_CALL(*mClientMockPtr, sendVpdEvent(EVpdEvent::CpuComStarted)).Times(1);

    mCpuComClient.CpuComStarted();
}

/* Verifies that the function startConnection() can be called from
   CpuComClient.
*/
TEST_F(CpuComClientTest, startConnectionCall)
{
    EXPECT_CALL(*mClientMockPtr, startConnection()).Times(1);

    mCpuComClient.startConnection();
}

}  // namespace lib
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
