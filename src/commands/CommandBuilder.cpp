//
// Created by zero on 11/25/26.
//

#include "Lodestone.CLI/commands/CommandBuilder.h"

std::shared_ptr<LodestoneCLI::Commands::CommandBuilder> LodestoneCLI::Commands::CommandBuilder::create() {
    return std::make_shared<CommandBuilder>();
}

//region Setters
LodestoneCLI::Commands::CommandBuilder &LodestoneCLI::Commands::CommandBuilder::setCommandName(
    const std::string &commandName) {
    this->m_commandName = commandName;
    return *this;
}

LodestoneCLI::Commands::CommandBuilder &LodestoneCLI::Commands::CommandBuilder::setCommandDescription(
    const std::string &commandDescription) {
    this->m_commandDescription = commandDescription;
    return *this;
}

LodestoneCLI::Commands::CommandBuilder &LodestoneCLI::Commands::CommandBuilder::addOption(const OptionBuilder &option) {
    this->m_options.push_back(option);
    return *this;
}

//endregion

//region Getters
std::string LodestoneCLI::Commands::CommandBuilder::getCommandName() {
    return this->m_commandName;
}

std::string LodestoneCLI::Commands::CommandBuilder::getCommandDescription() {
    return this->m_commandDescription;
}

//endregion

void LodestoneCLI::Commands::CommandBuilder::build(CLI::App *app) {
    auto *command =
            app->add_subcommand(this->getCommandName(), this->getCommandDescription());

    for (const OptionBuilder &option: this->m_options) {
        std::vector<std::string> aliases = option.getAliases();

        // Join aliases for CLI11
        // Otherwise, use option name as only name
        CLI::Option *cli_option;
        if (!aliases.empty()) {
            std::ostringstream oss;
            aliases.push_back("--" + option.getName());
            for (size_t i = 0; i < aliases.size(); ++i) {
                if (i > 0) {
                    oss << ",";
                }
                oss << aliases[i];
            }

            const std::string joinedAliases = oss.str();

            cli_option = command->add_option(joinedAliases, option.getCallback(), option.getDescription());
        } else {
            cli_option = command->add_option(option.getName(), option.getCallback(), option.getDescription());
        }

        if (option.isRequired()) {
            cli_option->required();
        }

        for (const OptionRequirement &requirement: option.getRequirements()) {
            cli_option->check(requirement.toValidator());
        }
    }

    command->callback([this] {
        if (m_callback) {
            m_callback();
        }
    });
}
