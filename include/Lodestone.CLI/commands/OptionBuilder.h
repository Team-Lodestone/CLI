//
// Created by zero on 11/25/26.
//

#ifndef LODESTONE_CLI_OPTION_BUILDER_H
#define LODESTONE_CLI_OPTION_BUILDER_H

#pragma once
#include <functional>
#include <string>

#include "CLI11.hpp"

namespace LodestoneCLI::Commands {
    class OptionRequirement {
public:
    using RequirementFunc = std::function<std::string(const std::string&)>;

    explicit OptionRequirement(std::string name, RequirementFunc requirement) {
        this->m_name = std::move(name);
        this->m_requirement = std::move(requirement);
    };

    [[nodiscard]] std::string validate(const std::string &value) const {
        return m_requirement ? m_requirement(value) : std::string{};
    }

    [[nodiscard]] CLI::Validator toValidator() const {
        return {
            [*this](const std::string& value) {
                return this->validate(value);
            },
            m_name
        };
    }
private:
    std::string m_name{};

    // Function to be called when a requirement is to be checked.
    // Returns a string error message if the requirement is not met.
    RequirementFunc m_requirement = [](const std::string&) { return std::string{}; };
};

class OptionRequirements {
public:
    static inline const OptionRequirement EXISTING_FILE_OR_DIR{
        "EXISTING_FILE_OR_DIR",
        [](const std::string& path) -> std::string {
            std::error_code ec;

            if (const auto status = std::filesystem::status(path, ec); ec || (!std::filesystem::is_regular_file(status) && !std::filesystem::is_directory(status))) {
                return "Path must be an existing file or directory: " + path;
            }
            return {};
        }
    };

    static inline const OptionRequirement EXISTING_PATH{
        "EXISTING_PATH",
        [](const std::string& path) -> std::string {
            if (!std::filesystem::exists(path)) {
                return "Path does not exist: " + path;
            }
            return {};
        }
    };

    static inline const OptionRequirement EXISTING_FILE{
        "EXISTING_FILE",
        [](const std::string& path) -> std::string {
            if (!std::filesystem::is_regular_file(path)) {
                return "File does not exist: " + path;
            }
            return {};
        }
    };

    static inline const OptionRequirement EXISTING_DIR{
        "EXISTING_DIR",
        [](const std::string& path) -> std::string {
            if (!std::filesystem::is_directory(path)) {
                return "Directory does not exist: " + path;
            }
            return {};
        }
    };
};

class OptionBuilder {
public:
    static std::shared_ptr<OptionBuilder> create();

    //region Setters
    OptionBuilder &setOptionName(const std::string &optionName);

    OptionBuilder &setOptionDescription(const std::string &optionDescription);

    OptionBuilder &addAlias(const std::string &alias);

    OptionBuilder &addRequirement(const OptionRequirement &requirement);
    template<typename T> static bool convertValue(const std::string& input, T& output);
    template<typename T>
    OptionBuilder &setCallback(T &value);

    OptionBuilder &isRequired(bool isRequired);
    //endregion

    //region Getters
    [[nodiscard]] std::string getName() const;
    [[nodiscard]] std::string getDescription() const;
    [[nodiscard]] std::vector<std::string> getAliases() const;
    [[nodiscard]] std::vector<OptionRequirement> getRequirements() const;
    [[nodiscard]] std::function<bool(const std::vector<std::string> &)> getCallback() const;
    [[nodiscard]] bool isRequired() const;
    //endregion

    OptionBuilder &build();

private:
    std::string m_optionName;
    std::string m_optionDescription;

    std::vector<std::string> m_aliases;

    std::vector<OptionRequirement> m_requirements;

    bool m_isRequired = false;

    std::function<bool(const std::vector<std::string> &)> m_callback;
};

template<typename T>
bool OptionBuilder::convertValue(const std::string &input, T &output) {
    if constexpr (std::is_same_v<T, std::string>) {
        output = input;
        return true;
    } else if constexpr (std::is_same_v<T, std::filesystem::path>) {
        output = std::filesystem::path(input);
        return true;
    } else if constexpr (std::is_same_v<T, bool>) {
        if (input == "true" || input == "1") {
            output = true;
            return true;
        }

        if (input == "false" || input == "0") {
            output = false;
            return true;
        }

        return false;
    } else {
        std::istringstream stream(input);
        stream >> output;
        return stream && stream.peek() == std::char_traits<char>::eof();
    }
}

template<typename T>
OptionBuilder &OptionBuilder::setCallback(T &value) {
    this->m_callback = [&value](const std::vector<std::string>& values) {
        if (values.empty()) {
            return false;
        }

        return convertValue(values.front(), value);
    };
    return *this;
}
}

#endif //LODESTONE_CLI_OPTION_BUILDER_H
