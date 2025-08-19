/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#ifndef COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CONFIGSCRIPTCMDCREATOR_H_
#define COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CONFIGSCRIPTCMDCREATOR_H_

#include "IAndroidInit.h"
#include "ICondition.h"
#include "IConfigReader.h"
#include "IConfigScriptCmdCreator.h"
#include "IConfigScriptCommand.h"

#include <memory>
#include <string>
#include <vector>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

class ConfigScriptCmdCreator : public IConfigScriptCmdCreator {
public:
    ConfigScriptCmdCreator(IAndroidInit&, const IConfigReader&);
    auto createCommand(const std::string&) -> std::unique_ptr<IConfigScriptCommand> override;

private:
    static const class ParsingError {
    public:
        ParsingError() = default;
        ParsingError(const std::string& message)
            : mMessage(message)
        {
        }
        const std::string& getMessage() const { return mMessage; }

        bool hasError() const { return !mMessage.empty(); }

    private:
        const std::string mMessage;
    } NoError;

    static auto getPropertyNameSectionStart(const std::string&) -> std::string::size_type;
    static auto getPropertyNameSectionEnd(const std::string&, const std::string::size_type)
        -> std::pair<std::string::size_type, ParsingError>;
    static auto extractPropertyName(const std::string&) -> std::pair<std::string, ParsingError>;

    static auto getDaemonNamesSectionStart(const std::string&)
        -> std::pair<std::string::size_type, ParsingError>;
    static auto getDaemonNamesSectionEnd(const std::string&, const std::string::size_type)
        -> std::string::size_type;
    static auto extractDaemonNames(const std::string&)
        -> std::pair<std::vector<std::string>, ParsingError>;

    static auto getExecuteConditionSectionStart(const std::string&) -> std::string::size_type;
    static auto getExecuteConditionSectionEnd(const std::string&, const std::string::size_type)
        -> std::pair<std::string::size_type, ParsingError>;
    static auto validateConditionParamName(const std::string&) -> ParsingError;
    static auto validateConditionParamType(const std::string&) -> ParsingError;
    static auto extractExecuteCondition(const std::string&, const IConfigReader&)
        -> std::pair<std::unique_ptr<ICondition>, ParsingError>;

    static auto trimSideWhitespaces(const std::string&) -> std::string;
    static auto validatePropertyName(const std::string&) -> ParsingError;
    static auto validateServiceName(const std::string&) -> ParsingError;

    static auto getCondition(const std::string& paramType,
                             const std::string& paramName,
                             const std::string& paramValue,
                             const IConfigReader& configReader) -> std::unique_ptr<ICondition>;

private:
    const IAndroidInit& mAndroidInit;
    const IConfigReader& mConfigReader;

    static const char daemonNamesSectionDelim;
    static const char conditionSectionDelim;
    static const char daemonNameDelim;
    static const char spaceSign;

    static const std::vector<const char*> conditionParamType;
};

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com

#endif  // COM_MITSUBISHIELECTRIC_AHU_VEHICLEPWRMGR_CONFIGSCRIPTCMDCREATOR_H_
