/*
 * COPYRIGHT (C) 2019 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ParamStringEqualCondition.h"

#include "MockVehicleConfigReader.h"

#include <gtest/gtest.h>
#include <memory>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::Test;

class ParamStringEqualConditionTest : public Test {
protected:
    ~ParamStringEqualConditionTest();

    MockVehicleConfigReader mMockVehicleConfigReader;
    std::unique_ptr<ParamStringEqualCondition> mParamStringEqualCondition;

    const std::string mParamName{"SomeParamName"};
    const std::string mParamValue{"Value"};
};

ParamStringEqualConditionTest::~ParamStringEqualConditionTest() {}

TEST_F(ParamStringEqualConditionTest,
       ParamStringEqualCondition_must_return_false_if_config_reader_return_minus_one)
{
    EXPECT_CALL(mMockVehicleConfigReader, getString(_, _)).WillOnce(Return(-1));

    mParamStringEqualCondition = std::make_unique<ParamStringEqualCondition>(
        mParamName, mParamValue, mMockVehicleConfigReader);

    EXPECT_FALSE(mParamStringEqualCondition->isSatisfied());
}

TEST_F(
    ParamStringEqualConditionTest,
    ParamStringEqualCondition_must_return_false_if_config_reader_return_not_same_value_as_paramValue)
{
    const std::string outValue{"SomeValue"};

    EXPECT_CALL(mMockVehicleConfigReader, getString(mParamName, _))
        .WillOnce(DoAll(SetArgReferee<1>(outValue), Return(0)));

    mParamStringEqualCondition = std::make_unique<ParamStringEqualCondition>(
        mParamName, mParamValue, mMockVehicleConfigReader);

    EXPECT_FALSE(mParamStringEqualCondition->isSatisfied());
}

TEST_F(ParamStringEqualConditionTest,
       ParamStringEqualCondition_must_return_true_if_config_reader_return_same_value_as_paramValue)
{
    EXPECT_CALL(mMockVehicleConfigReader, getString(mParamName, _))
        .WillOnce(DoAll(SetArgReferee<1>(mParamValue.c_str()), Return(0)));

    mParamStringEqualCondition = std::make_unique<ParamStringEqualCondition>(
        mParamName, mParamValue, mMockVehicleConfigReader);

    EXPECT_TRUE(mParamStringEqualCondition->isSatisfied());
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
