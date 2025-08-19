/*
 * COPYRIGHT (C) 2018 MITSUBISHI ELECTRIC CORPORATION
 * ALL RIGHTS RESERVED
 */

#include "NativeConfigProvider.h"

#include <algorithm>
#include <cctype>

namespace com {
namespace mitsubishielectric {
namespace ahu {
namespace vehiclepwrmgr {
namespace impl {

NativeConfigProvider::NativeConfigProvider(IVehicleConfigProvider& configProvider)
    : mConfigProvider(configProvider)
{
}

std::vector<std::string> NativeConfigProvider::loadConfig()
{
    std::string configStr = mConfigProvider.getInitString();
    std::vector<std::string> configLines;

    const std::string delims{";\n"};

    std::size_t newLineBegin = 0;
    std::size_t length = 0;
    std::string line;

    std::size_t delimPos = configStr.find_first_of(delims);
    while (delimPos != std::string::npos) {
        length = delimPos - newLineBegin;
        line = configStr.substr(newLineBegin, length);
        if (isValidLine(line)) {
            configLines.push_back(line);
        }
        newLineBegin = delimPos + 1;  // skip delim
        delimPos = configStr.find_first_of(delims, newLineBegin);
    }
    line = configStr.substr(newLineBegin);
    if (isValidLine(line)) {
        configLines.push_back(line);
    }

    return configLines;
}  // LCOV_EXCL_LINE

bool NativeConfigProvider::isValidLine(const std::string& line)
{
    // skip empty lines
    auto it = std::find_if(line.begin(), line.end(), [](char ch) { return isalnum(ch); });
    if (it == line.end()) {
        return false;
    }

    // skip comments - a line which starts with '#'
    if (line.length() && line[0] == '#') {
        return false;
    }

    return true;
}

}  // namespace impl
}  // namespace vehiclepwrmgr
}  // namespace ahu
}  // namespace mitsubishielectric
}  // namespace com
