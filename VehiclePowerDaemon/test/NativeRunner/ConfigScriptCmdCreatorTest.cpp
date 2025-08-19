/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ConfigScriptCmdCreator.h"
#include "MockAndroidInit.h"
#include "MockVehicleConfigReader.h"
#include "StartNativeCommand.h"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;
using ::testing::DoAll;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SetArgReferee;

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ConfigScriptCmdCreatorTest : public ::testing::Test {
public:
    ConfigScriptCmdCreatorTest()
        : commandParser{androidInit, mMockConfigReader}
    {
    }

    ConfigScriptCmdCreator commandParser;
    MockAndroidInit androidInit;
    NiceMock<MockVehicleConfigReader> mMockConfigReader;

    // returns true if commandLine is correct
    bool testCommandLine(const std::string& commandLine)
    {
        auto command = commandParser.createCommand(commandLine);
        return command != nullptr;
    }
};

TEST_F(ConfigScriptCmdCreatorTest, CorrectStringTest)
{
    auto correctCommandLines = std::vector<std::string>{
        "prop_name1 : daemon1",
        " prop_name1 : daemon1 daemon2 ",
        "prop_name1 : daemon1 daemon2 daemon3",
        "prop_name1 : daemon1 daemon2  daemon3",
        "prop_name1: daemon1 daemon2",
        "prop_name1 :daemon1 daemon2",
        "prop_name1:daemon1 daemon2",
    };

    for (const auto& commandLine : correctCommandLines) {
        EXPECT_TRUE(testCommandLine(commandLine)) << '"' << commandLine << '"';
    }
}

TEST_F(ConfigScriptCmdCreatorTest, IncorrectStringTest)
{
    auto incorrectCommandLines =
        std::vector<std::string>{"prop_name1 ; daemon1 daemon2 daemon3",
                                 "prop_name1 : daemon1 daemon2 : daemon3",
                                 "prop_name1 :: daemon1 daemon2 daemon3",
                                 "prop_name1 prop_name2 : daemon1 daemon2 daemon3",
                                 ": daemon1 daemon2",
                                 "+: daemon1 daemon2",
                                 ":daemon1 daemon2",
                                 "prop_name1 : d+aemon1",
                                 "prop_name:",
                                 "prop_name :",
                                 "prop_name: ",
                                 "",
                                 " "};

    for (const auto& commandLine : incorrectCommandLines) {
        EXPECT_FALSE(testCommandLine(commandLine)) << '"' << commandLine << '"';
    }
}

TEST_F(ConfigScriptCmdCreatorTest, conditional_command_good_byte)
{
    auto correctCommandLines = std::vector<std::string>{
        // clang-format off
        "prop_name1 : daemon1:VAR_NAME.byte == 3",
        "prop_name1 : daemon1: VAR_NAME.byte == 3",
        "prop_name1 : daemon1 :VAR_NAME.byte == 3",
        "prop_name1 : daemon1 : VAR_NAME.byte == 3",
        "prop_name1 : daemon1:VAR_NAME.byte==3",
        "prop_name1 : daemon1: VAR_NAME.byte ==3",
        "prop_name1 : daemon1 :VAR_NAME.byte== 3",
        // clang-format on
    };

    for (const auto& commandLine : correctCommandLines) {
        EXPECT_TRUE(testCommandLine(commandLine)) << '"' << commandLine << '"';
    }
}

TEST_F(ConfigScriptCmdCreatorTest, conditional_command_good_string)
{
    auto correctCommandLines = std::vector<std::string>{
        // clang-format off
        "prop_name1 : daemon1:VAR_NAME.str == abc",
        "prop_name1 : daemon1: VAR_NAME.str == abc",
        "prop_name1 : daemon1 :VAR_NAME.str == abc",
        "prop_name1 : daemon1 : VAR_NAME.str == abc",
        "prop_name1 : daemon1:VAR_NAME.str==abc",
        "prop_name1 : daemon1: VAR_NAME.str ==abc",
        "prop_name1 : daemon1 :VAR_NAME.str== abc",
        // clang-format on
    };

    for (const auto& commandLine : correctCommandLines) {
        EXPECT_TRUE(testCommandLine(commandLine)) << '"' << commandLine << '"';
    }
}

TEST_F(ConfigScriptCmdCreatorTest, conditional_command_bad)
{
    auto incorrectCommandLines = std::vector<std::string>{
        "prop_name1 : daemon1 :",
        "prop_name1 : daemon1 : VAR_NAME.type",
        "prop_name1 : daemon1 : VAR_NAME.type==",
        "prop_name1 : daemon1 : VAR_NAME.type != value",
        "prop_name1 : daemon1 : VAR_NAME.type value",
        "prop_name1 : daemon1 : VAR_NAME. type == value",
        "prop_name1 : daemon1 : VAR_NAME .type == value",
    };

    for (const auto& commandLine : incorrectCommandLines) {
        EXPECT_FALSE(testCommandLine(commandLine)) << '"' << commandLine << '"';
    }
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
