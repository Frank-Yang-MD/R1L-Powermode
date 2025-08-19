/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "MockVehicleConfigReader.h"
#include "ParamByteEqualCondition.h"

using ::testing::_;
using ::testing::Return;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

/**
 * Verifies that there function isSatisfied() returns true
 * if the function getByte() of the interface IConfigReader
 * provides ther same config value as expected.
 */
TEST(ParamByteEqualConditionTest, isSatisfied_must_return_true)
{
    // Arrangements
    MockVehicleConfigReaderOverride configReader;
    ParamByteEqualCondition paramByteEqualCondition("name", 1, configReader);
    // Action
    bool result = paramByteEqualCondition.isSatisfied();
    // Verification
    EXPECT_TRUE(result);
}

/**
 * Verifies that there function isSatisfied() returns false
 * if the function getByte() of the interface IConfigReader
 * returns failure.
 */
TEST(ParamByteEqualConditionTest, isSatisfied_must_return_false)
{
    // Arrangements
    MockVehicleConfigReader configReader;
    ON_CALL(configReader, getByte(_, _)).WillByDefault(Return(-1));
    ParamByteEqualCondition paramByteEqualCondition("name", 1, configReader);
    // Action
    bool result = paramByteEqualCondition.isSatisfied();
    // Verification
    EXPECT_FALSE(result);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
