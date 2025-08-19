/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NativeConfigProvider.h"
#include "MockVehicleConfigProvider.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class NativeConfigProviderTest : public ::testing::Test {
public:
    NativeConfigProviderTest()
        : mNativeConfigProvider{mVehicleConfigProvider}
    {
    }

    std::string makeMultiLineString(const std::vector<std::string>& lines, const std::string& delim)
    {
        std::string multiLineString;
        for (size_t i = 0; i < lines.size(); ++i) {
            multiLineString += (i < lines.size() - 1) ? lines[i] + delim : lines[i];
        }
        return multiLineString;
    }

    std::vector<std::string> getResultConfigLines(const std::string& configString)
    {
        EXPECT_CALL(mVehicleConfigProvider, getInitString())
            .WillOnce(::testing::Return(configString))
            .RetiresOnSaturation();
        return mNativeConfigProvider.loadConfig();
    }

    MockVehicleConfigProvider mVehicleConfigProvider;
    NativeConfigProvider mNativeConfigProvider;
};

TEST_F(NativeConfigProviderTest, line_delimiters_are_recognized)
{
    const std::vector<std::string> testLines{"first line", "second line", "third line"};
    const std::vector<std::string> delims{"\n", ";"};

    for (const auto& delim : delims) {
        auto configResultLines = getResultConfigLines(makeMultiLineString(testLines, delim));
        ASSERT_EQ(configResultLines.size(), testLines.size());
        for (size_t i = 0; i < configResultLines.size(); ++i) {
            EXPECT_STREQ(configResultLines[i].c_str(), testLines[i].c_str());
        }
    }
}

TEST_F(NativeConfigProviderTest, empty_lines_are_skipped)
{
    const std::vector<std::string> testLines{"first line", " ", "third line", ""};
    const std::vector<std::string> expectedLines{"first line", "third line"};
    const std::string delim{"\n"};

    auto configResultLines = getResultConfigLines(makeMultiLineString(testLines, delim));

    ASSERT_EQ(configResultLines.size(), expectedLines.size());
    for (size_t i = 0; i < configResultLines.size(); ++i) {
        EXPECT_STREQ(configResultLines[i].c_str(), expectedLines[i].c_str());
    }
}

TEST_F(NativeConfigProviderTest, comment_lines_are_skipped)
{
    const std::vector<std::string> testLines{"first line", "#", "#bla"};
    const std::vector<std::string> expectedLines{"first line"};
    const std::string delim{"\n"};

    auto configResultLines = getResultConfigLines(makeMultiLineString(testLines, delim));

    ASSERT_EQ(configResultLines.size(), expectedLines.size());
    for (size_t i = 0; i < configResultLines.size(); ++i) {
        EXPECT_STREQ(configResultLines[i].c_str(), expectedLines[i].c_str());
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
