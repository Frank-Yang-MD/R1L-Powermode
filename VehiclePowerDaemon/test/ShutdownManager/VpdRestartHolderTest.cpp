/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockAndroidInit.h"

#include "VpdRestartHolder.h"

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using namespace testing;

static const std::string VPD_STARTED_PROP = "vendor.vpd_started";

class VpdRestartHolderTest : public Test {
public:
    VpdRestartHolderTest()
        : mVpdRestartHolder{mMockAndroidInit}
    {
    }

    ~VpdRestartHolderTest();

    StrictMock<MockAndroidInit> mMockAndroidInit;

    VpdRestartHolder mVpdRestartHolder;
};

VpdRestartHolderTest::~VpdRestartHolderTest() {}

TEST_F(VpdRestartHolderTest, vpdStarted)
{
    const std::string VPD_STARTED_PROP_VALUE = "1";

    EXPECT_CALL(mMockAndroidInit, propertySet(VPD_STARTED_PROP, VPD_STARTED_PROP_VALUE)).Times(1);

    mVpdRestartHolder.vpdStarted();
}

TEST_F(VpdRestartHolderTest, isVpdRestartedTrue)
{
    const std::string VPD_STARTED_PROP_VALUE = "1";

    EXPECT_CALL(mMockAndroidInit, propertyGet(VPD_STARTED_PROP, _))
        .WillOnce(DoAll(SetArgReferee<1>(VPD_STARTED_PROP_VALUE),
                        Return(VPD_STARTED_PROP_VALUE.length())));

    ASSERT_TRUE(mVpdRestartHolder.isVpdRestarted());
}

TEST_F(VpdRestartHolderTest, isVpdRestartedFalse)
{
    const std::string VPD_STARTED_PROP_VALUE = "";

    EXPECT_CALL(mMockAndroidInit, propertyGet(VPD_STARTED_PROP, _))
        .WillOnce(DoAll(SetArgReferee<1>(VPD_STARTED_PROP_VALUE),
                        Return(VPD_STARTED_PROP_VALUE.length())));

    ASSERT_FALSE(mVpdRestartHolder.isVpdRestarted());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
