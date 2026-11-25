//
// Created by zero on 11/25/26.
//

#ifndef LODESTONE_CLI_COMMAND_BUILDER_H
#define LODESTONE_CLI_COMMAND_BUILDER_H

#include <filesystem>
#include <functional>
#include <string>
#include <utility>
#include <vector>

#include "OptionBuilder.h"

namespace LodestoneCLI::Commands {
    class CommandBuilder {
    public:
        static std::shared_ptr<CommandBuilder> create();

        //region Setters
        CommandBuilder &setCommandName(const std::string &commandName);

        CommandBuilder &setCommandDescription(const std::string &commandDescription);

        CommandBuilder &addOption(const OptionBuilder &option);

        CommandBuilder &setCallback(std::function<void()> callback) {
            this->m_callback = std::move(callback);
            return *this;
        }
        //endregion

        //region Getters
        std::string getCommandName();
        std::string getCommandDescription();

        std::function<void()> getCallback() {
            return this->m_callback;
        };
        //endregion

        void build(CLI::App *app);
    private:
        std::string m_commandName;
        std::string m_commandDescription;

        std::vector<OptionBuilder> m_options;

        std::function<void()> m_callback;
    };
}

#endif //LODESTONE_CLI_COMMAND_BUILDER_H
