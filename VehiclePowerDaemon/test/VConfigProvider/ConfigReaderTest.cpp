/*
 * COPYRIGHT (C) 2020 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <ConfigReader.h>
#include <mock/mock_VehicleConfigReader.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

using ::testing::Return;

/**
 * Verifies that the function getByte() of ConfigReader
 * returns a value not equal -1.
 */
TEST(ConfigReaderTest, getByte_returns_a_value)
{
    auto mVehicleConfigReaderPtr = new vehicleconfig::mock_VehicleConfigReader;
    std::unique_ptr<vehicleconfig::mock_VehicleConfigReader> mVehicleConfigReaderMock(
        mVehicleConfigReaderPtr);
    ConfigReader configReader(std::move(mVehicleConfigReaderMock));

    std::string key = "name";
    uint8_t value = 1;

    EXPECT_CALL(*mVehicleConfigReaderPtr, getByte(key, value)).WillOnce(Return(1));

    int result = configReader.getByte(key, value);

    EXPECT_NE(result, -1);
}

/**
 * Verifies that the function getByte() of ConfigReader
 * returns a value equal -1.
 */
TEST(ConfigReaderTest, getByte_when_reader_is_null)
{
    ConfigReader configReader(nullptr);

    std::string key = "name";
    uint8_t value = 1;

    int result = configReader.getByte(key, value);

    EXPECT_EQ(result, -1);
}

/**
 * Verifies that the function getInt() of ConfigReader
 * returns a value not equal -1.
 */
TEST(ConfigReaderTest, getInt_returns_a_value)
{
    auto mVehicleConfigReaderPtr = new vehicleconfig::mock_VehicleConfigReader;
    std::unique_ptr<vehicleconfig::mock_VehicleConfigReader> mVehicleConfigReaderMock(
        mVehicleConfigReaderPtr);
    ConfigReader configReader(std::move(mVehicleConfigReaderMock));

    std::string key = "name";
    int value = 1;

    EXPECT_CALL(*mVehicleConfigReaderPtr, getInt(key, value)).WillOnce(Return(1));

    int result = configReader.getInt(key, value);

    EXPECT_NE(result, -1);
}

/**
 * Verifies that the function getInt() of ConfigReader
 * returns a value equal -1.
 */
TEST(ConfigReaderTest, getInt_when_reader_is_null)
{
    ConfigReader configReader(nullptr);

    std::string key = "name";
    int value = 1;

    int result = configReader.getInt(key, value);

    EXPECT_EQ(result, -1);
}

/**
 * Verifies that the function getString() of ConfigReader
 * returns a value not equal -1.
 */
TEST(ConfigReaderTest, getString_returns_a_value)
{
    auto mVehicleConfigReaderPtr = new vehicleconfig::mock_VehicleConfigReader;
    std::unique_ptr<vehicleconfig::mock_VehicleConfigReader> mVehicleConfigReaderMock(
        mVehicleConfigReaderPtr);
    ConfigReader configReader(std::move(mVehicleConfigReaderMock));

    std::string key = "name";
    std::string value = "value";

    EXPECT_CALL(*mVehicleConfigReaderPtr, getString(key, value)).WillOnce(Return(1));

    int result = configReader.getString(key, value);

    EXPECT_NE(result, -1);
}

/**
 * Verifies that the function getString() of ConfigReader
 * returns a value equal -1.
 */
TEST(ConfigReaderTest, getString_when_reader_is_null)
{
    ConfigReader configReader(nullptr);

    std::string key = "name";
    std::string value = "value";

    int result = configReader.getString(key, value);

    EXPECT_EQ(result, -1);
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
