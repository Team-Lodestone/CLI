//
// Created by zero on 11/19/26.
//

#ifndef LODESTONE_CLI_IDENTIFIER_UTILS_H
#define LODESTONE_CLI_IDENTIFIER_UTILS_H

#include <optional>
#include <string>

#include "Lodestone.Common/registry/Identifier.h"

namespace LodestoneCLI::Utils {
    class IdentifierUtils {
    public:
        static std::optional<lodestone::common::registry::Identifier> tryParse(const std::string& input) {
            const auto separator = input.find(':');

            if (separator == std::string::npos)
                return std::nullopt;

            const std::string nameSpace = input.substr(0, separator);
            const std::string path = input.substr(separator + 1);

            if (nameSpace.empty() || path.empty())
                return std::nullopt;

            return lodestone::common::registry::Identifier(nameSpace.c_str(), path.c_str());
        }
    };
}

#endif //LODESTONE_CLI_IDENTIFIER_UTILS_H
