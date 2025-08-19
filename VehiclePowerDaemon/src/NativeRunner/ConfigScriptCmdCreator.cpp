/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "ConfigScriptCmdCreator.h"
#include "AndroidInit.h"
#include "ParamByteEqualCondition.h"
#include "ParamStringEqualCondition.h"
#include "StartNativeCmdConditional.h"
#include "StartNativeCommand.h"
#include "VehiclePowerDaemonLog.h"

#include <algorithm>
#include <string>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

const char ConfigScriptCmdCreator::daemonNamesSectionDelim{':'};
const char ConfigScriptCmdCreator::conditionSectionDelim{':'};
const char ConfigScriptCmdCreator::daemonNameDelim{' '};  // not necessary a 'space'
const char ConfigScriptCmdCreator::spaceSign{' '};

const std::vector<const char*> ConfigScriptCmdCreator::conditionParamType{"byte", "str"};

const ConfigScriptCmdCreator::ParsingError ConfigScriptCmdCreator::NoError;

ConfigScriptCmdCreator::ConfigScriptCmdCreator(IAndroidInit& androidInit,
                                               const IConfigReader& configReader)
    : mAndroidInit(androidInit)
    , mConfigReader{configReader}
{
}

auto ConfigScriptCmdCreator::createCommand(const std::string& configLine)
    -> std::unique_ptr<IConfigScriptCommand>
{
    std::string line = configLine;

    const auto [propertyName, error] = extractPropertyName(line);
    if (error.hasError()) {
        MLOGW(LogFuncId::VPD, LogId::ConfigScriptCmdCreator_createCommand, error.getMessage());
        return nullptr;
    }

    const auto [services, extractDaemonNamesError] = extractDaemonNames(line);
    if (extractDaemonNamesError.hasError()) {
        MLOGW(LogFuncId::VPD, LogId::ConfigScriptCmdCreator_createCommand,
              extractDaemonNamesError.getMessage());
        return nullptr;
    }

    const auto ptr = new (std::nothrow) StartNativeCommand(propertyName, services, mAndroidInit);
    std::unique_ptr<IConfigScriptCommand> cmd(ptr);

    if (cmd != nullptr) {
        auto [condition, error] = extractExecuteCondition(line, mConfigReader);
        if (error.hasError()) {
            MLOGW(LogFuncId::VPD, LogId::ConfigScriptCmdCreator_createCommand, error.getMessage());
            return nullptr;
        }
        if (condition != nullptr) {
            cmd.reset(new (std::nothrow)
                          StartNativeCmdConditional(std::move(cmd), std::move(condition)));
        }
    }
    return cmd;
}

auto ConfigScriptCmdCreator::getPropertyNameSectionStart(const std::string&)
    -> std::string::size_type
{
    return std::string::size_type{0};
}

auto ConfigScriptCmdCreator::getPropertyNameSectionEnd(const std::string& cmdString,
                                                       const std::string::size_type sectionStartPos)
    -> std::pair<std::string::size_type, ParsingError>
{
    auto sectionEndPos = cmdString.find(daemonNamesSectionDelim, sectionStartPos);
    if (sectionEndPos == std::string::npos) {
        // This section delimiter is required to be present in the cmd.
        return {std::string::npos,
                ParsingError(std::string{__func__} + ": no end of PropertyName section")};
    }
    return {sectionEndPos, NoError};
}

auto ConfigScriptCmdCreator::extractPropertyName(const std::string& cmdString)
    -> std::pair<std::string, ParsingError>
{
    // Possible form of this cmdString's section: " <param_name> :"

    const auto sectionStartPos = getPropertyNameSectionStart(cmdString);
    const auto [sectionEndPos, error] = getPropertyNameSectionEnd(cmdString, sectionStartPos);
    if (error.hasError()) {
        return {"", error};
    }

    std::string propertyName = cmdString.substr(sectionStartPos, sectionEndPos);
    propertyName = trimSideWhitespaces(propertyName);
    auto validateError = validatePropertyName(propertyName);

    return {propertyName, validateError};
}

auto ConfigScriptCmdCreator::getDaemonNamesSectionStart(const std::string& cmdString)
    -> std::pair<std::string::size_type, ParsingError>
{
    auto sectionStartPos = cmdString.find(daemonNamesSectionDelim);
    if (sectionStartPos == std::string::npos) {
        // This section delimiter is required to be present in the cmd.
        return {sectionStartPos,
                ParsingError(std::string{__func__} + ": no start of DaemonNames section")};
    }
    return {sectionStartPos, NoError};
}

auto ConfigScriptCmdCreator::getDaemonNamesSectionEnd(const std::string& cmdString,
                                                      const std::string::size_type sectionStartPos)
    -> std::string::size_type
{
    auto sectionEndPos = cmdString.find(conditionSectionDelim, sectionStartPos + 1);
    if (sectionEndPos == std::string::npos) {
        // There is no "conditional" section in the command.
        if (cmdString.length() - 1 == sectionStartPos) {
            // There is no "daemon names" section in the command.
            // This delimiter is the last character in the cmdString.
            sectionEndPos = sectionStartPos;
        }
        else {
            // "daemon names" section is not empty.
            sectionEndPos = cmdString.length() - 1;
        }
    }
    return sectionEndPos;
}

auto ConfigScriptCmdCreator::extractDaemonNames(const std::string& cmdString)
    -> std::pair<std::vector<std::string>, ParsingError>
{
    // Possible form of this cmdString's section: " : <daemon_name> [<daemon_name> ...] [:]"

    const std::string propertyPrefix{"init.svc.vendor."};
    std::vector<std::string> services;

    const auto [sectionStartPos, startError] = getDaemonNamesSectionStart(cmdString);
    if (startError.hasError()) {
        return {services, startError};
    }

    const auto sectionEndPos = getDaemonNamesSectionEnd(cmdString, sectionStartPos);

    const auto delims = std::string{daemonNameDelim} + spaceSign + conditionSectionDelim;

    if (std::numeric_limits<std::string::size_type>::max() - sectionStartPos < 1UL) {
        return {services,
                ParsingError(std::string{__func__} + ": unsigned integer operation wrapped")};
    }

    auto nameStartPos = cmdString.find_first_not_of(delims, sectionStartPos + 1UL);
    while (nameStartPos < sectionEndPos) {
        std::string daemonName;

        auto nameEndPos = cmdString.find_first_of(delims, nameStartPos);
        if (nameEndPos == std::string::npos) {
            // This is the last command, and there are no characters after it.
            daemonName = cmdString.substr(nameStartPos);
        }
        else {
            auto nameLength = nameEndPos - nameStartPos;
            daemonName = cmdString.substr(nameStartPos, nameLength);
        }

        const auto error = validateServiceName(daemonName);
        if (error.hasError()) {
            return {services, error};
        }

        services.push_back(propertyPrefix + daemonName);

        nameStartPos = cmdString.find_first_not_of(delims, nameEndPos);
    }

    if (services.size() == 0) {
        return {services, ParsingError(std::string{__func__} + ": no service name")};
    }

    return {services, NoError};
}

auto ConfigScriptCmdCreator::getExecuteConditionSectionStart(const std::string& cmdString)
    -> std::string::size_type
{
    auto sectionStartPos = cmdString.find(conditionSectionDelim);
    if (conditionSectionDelim == daemonNamesSectionDelim) {
        sectionStartPos = cmdString.find(daemonNamesSectionDelim, sectionStartPos + 1);
    }
    return sectionStartPos;
}

auto ConfigScriptCmdCreator::getExecuteConditionSectionEnd(
    const std::string& cmdString,
    const std::string::size_type sectionStartPos) -> std::pair<std::string::size_type, ParsingError>
{
    if (sectionStartPos == std::string::npos) {
        // There is no "conditional" section in the command.
        return {sectionStartPos, NoError};
    }

    auto sectionEndPos = std::string::npos;

    if (cmdString.length() - 1 == sectionStartPos) {
        // There is no "conditional" section in the command.
        // This delimiter is the last character in the cmdString.
        return {sectionEndPos, ParsingError(std::string{__func__} + ": empty condition section")};
    }
    else {
        // "conditional" section is not empty.
        sectionEndPos = cmdString.length() - 1;
    }

    return {sectionEndPos, NoError};
}

auto ConfigScriptCmdCreator::validateConditionParamName(const std::string& paramName)
    -> ParsingError
{
    const std::string letters{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    const std::string digits{"0123456789"};
    const std::string symbols{"-_"};
    const std::string allowedChars{letters + digits + symbols};
    if (paramName.find_first_not_of(allowedChars) != std::string::npos) {
        return ParsingError(std::string{__func__} + ": service name '" + paramName +
                            "' has chars other than: " + allowedChars);
    }

    return NoError;
}

auto ConfigScriptCmdCreator::validateConditionParamType(const std::string& paramType)
    -> ParsingError
{
    bool found = false;
    for (const auto& value : conditionParamType) {
        if (value == paramType) {
            found = true;
            break;
        }
    }

    if (!found) {
        return ParsingError(std::string{__func__} + ": param type '" + paramType +
                            "' is not valid");
    }

    return NoError;
}

auto ConfigScriptCmdCreator::extractExecuteCondition(const std::string& cmdString,
                                                     const IConfigReader& configReader)
    -> std::pair<std::unique_ptr<ICondition>, ParsingError>
{
    // Possible form of this cmdString's section: ": <cfg_param_name>.<param_type> ==
    // <expected_value>"

    const auto sectionStartPos = getExecuteConditionSectionStart(cmdString);
    const auto [sectionEndPos, error] = getExecuteConditionSectionEnd(cmdString, sectionStartPos);
    if (error.hasError()) {
        return {nullptr, error};
    }

    if (sectionStartPos == sectionEndPos) {
        return {nullptr, error};  // This section of the cmd is empty.
    }

    const char typeDelim{'.'};
    const std::string equalsSign{"=="};
    const auto whiteSpaceDelims = std::string{spaceSign};

    // Validate condition format
    const auto typeDelimPos = cmdString.find(typeDelim, sectionStartPos + 1);
    const auto equalsSignPos = cmdString.find(equalsSign, typeDelimPos + 1);
    if (typeDelimPos == std::string::npos || equalsSignPos == std::string::npos) {
        return {nullptr,
                ParsingError(std::string{__func__} + ": incorrect format of conditional section")};
    }

    // Get "<cfg_param_name>"
    const auto paramNameStartPos =
        cmdString.find_first_not_of(whiteSpaceDelims, sectionStartPos + 1);
    if (paramNameStartPos == std::string::npos) {
        return {nullptr, ParsingError(std::string{__func__} +
                                      ": there are only whitespaces in conditional section")};
    }
    if (paramNameStartPos >= typeDelimPos) {
        return {nullptr, ParsingError(std::string{__func__} + ": parameter name is missing")};
    }
    const auto paramNameLength = typeDelimPos - paramNameStartPos;
    const auto paramName = cmdString.substr(paramNameStartPos, paramNameLength);

    const auto validateError = validateConditionParamName(paramName);
    if (validateError.hasError()) {
        return {nullptr, validateError};
    }

    // Get "<param_type>"
    const auto paramTypeStartPos = typeDelimPos + 1;
    auto paramTypeEndPos = cmdString.find_first_of(whiteSpaceDelims, paramTypeStartPos);
    if (paramTypeEndPos > equalsSignPos) {
        paramTypeEndPos = equalsSignPos;
    }
    const auto paramTypeLength = paramTypeEndPos - paramTypeStartPos;
    const auto paramType = cmdString.substr(paramTypeStartPos, paramTypeLength);
    const auto validateParamTypeError = validateConditionParamType(paramType);
    if (validateParamTypeError.hasError()) {
        return {nullptr, validateParamTypeError};
    }

    // Get "<equality_sign>"
    // Let's support only '==' for now.

    // Get "<expected_value>"
    const auto paramValueStartPos =
        cmdString.find_first_not_of(whiteSpaceDelims, equalsSignPos + equalsSign.length());
    const auto paramValueEndPos = cmdString.find_first_of(whiteSpaceDelims, paramValueStartPos);
    const auto paramValueLength = paramValueEndPos - paramValueStartPos;
    const auto paramValue = cmdString.substr(paramValueStartPos, paramValueLength);
    // TODO: do we need to validate "<expected_value>"?

    // Create a condition
    return {getCondition(paramType, paramName, paramValue, configReader), NoError};
}

auto ConfigScriptCmdCreator::trimSideWhitespaces(const std::string& str) -> std::string
{
    if (str.length() == 0) {
        return str;
    }

    const char spaceChar{' '};
    const std::size_t newBegin = str.find_first_not_of(spaceChar);
    if (newBegin == std::string::npos) {
        return std::string();
    }
    const std::size_t newEnd = str.find_last_not_of(spaceChar);

    std::size_t newStrLength = newEnd - newBegin + 1;

    return str.substr(newBegin, newStrLength);
}

auto ConfigScriptCmdCreator::validatePropertyName(const std::string& propertyName) -> ParsingError
{
    if (propertyName.length() == 0) {
        return ParsingError(std::string{__func__} + ": no property name '" + propertyName + "'");
    }
    if (propertyName.find(' ') != std::string::npos) {
        return ParsingError(std::string{__func__} + ": property name '" + propertyName +
                            "' has whitespaces");
    }
    if (!isalpha(propertyName[0])) {
        return ParsingError(std::string{__func__} + ": property name '" + propertyName +
                            "' doesn't start with letter");
    }

    return NoError;
}

auto ConfigScriptCmdCreator::validateServiceName(const std::string& serviceName) -> ParsingError
{
    if (!isalpha(serviceName[0])) {
        return ParsingError(std::string{__func__} + ": service name '" + serviceName +
                            "' doesn't start with letter");
    }

    const std::string letters{"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    const std::string digits{"0123456789"};
    const std::string symbols{"-_"};
    const std::string allowedChars{letters + digits + symbols};
    if (serviceName.find_first_not_of(allowedChars) != std::string::npos) {
        return ParsingError(std::string{__func__} + ": service name '" + serviceName +
                            "' has chars other than: " + allowedChars);
    }

    return NoError;
}

std::unique_ptr<ICondition> ConfigScriptCmdCreator::getCondition(const std::string& paramType,
                                                                 const std::string& paramName,
                                                                 const std::string& paramValue,
                                                                 const IConfigReader& configReader)
{
    std::unique_ptr<ICondition> condition{nullptr};

    if (paramType == "byte") {
        auto value = static_cast<uint8_t>(std::stoi(paramValue));
        condition.reset(new (std::nothrow) ParamByteEqualCondition{paramName, value, configReader});
    }
    else if (paramType == "str") {
        condition.reset(new (std::nothrow)
                            ParamStringEqualCondition{paramName, paramValue, configReader});
    }

    return condition;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
