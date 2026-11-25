//
// Created by zero on 11/25/26.
//

#include "Lodestone.CLI/commands/OptionBuilder.h"

#include "Lodestone.Common/util/Logging.h"

std::shared_ptr<LodestoneCLI::Commands::OptionBuilder> LodestoneCLI::Commands::OptionBuilder::create() {
    return std::make_shared<OptionBuilder>();
}

//region Setters
LodestoneCLI::Commands::OptionBuilder &LodestoneCLI::Commands::OptionBuilder::setOptionName(
    const std::string &optionName) {
    this->m_optionName = optionName;
    return *this;
}

LodestoneCLI::Commands::OptionBuilder &LodestoneCLI::Commands::OptionBuilder::setOptionDescription(
    const std::string &optionDescription) {
    this->m_optionDescription = optionDescription;
    return *this;
}

LodestoneCLI::Commands::OptionBuilder &LodestoneCLI::Commands::OptionBuilder::addAlias(const std::string &alias) {
    this->m_aliases.push_back(alias);
    return *this;
}

LodestoneCLI::Commands::OptionBuilder &LodestoneCLI::Commands::OptionBuilder::addRequirement(
    const OptionRequirement &requirement) {
    this->m_requirements.push_back(requirement);
    return *this;
}

LodestoneCLI::Commands::OptionBuilder &LodestoneCLI::Commands::OptionBuilder::isRequired(const bool isRequired) {
    this->m_isRequired = isRequired;
    return *this;
}

//endregion

//region Getters
std::string LodestoneCLI::Commands::OptionBuilder::getName() const {
    return this->m_optionName;
}

std::string LodestoneCLI::Commands::OptionBuilder::getDescription() const {
    return this->m_optionDescription;
}

std::vector<std::string> LodestoneCLI::Commands::OptionBuilder::getAliases() const {
    return this->m_aliases;
}

std::vector<LodestoneCLI::Commands::OptionRequirement> LodestoneCLI::Commands::OptionBuilder::getRequirements() const {
    return this->m_requirements;
}

std::function<bool(const std::vector<std::string> &)> LodestoneCLI::Commands::OptionBuilder::getCallback() const {
    return this->m_callback;
}

bool LodestoneCLI::Commands::OptionBuilder::isRequired() const {
    return this->m_isRequired;
}

LodestoneCLI::Commands::OptionBuilder &LodestoneCLI::Commands::OptionBuilder::build() {
    if (this->m_optionName.empty()) {
        LOG_ERROR("Option name is required");

        throw std::runtime_error("Option name is required");
    }

    if (this->m_optionDescription.empty()) {
        LOG_WARNING("Option description should not be empty");
    }
    return *this;
}

//endregion
