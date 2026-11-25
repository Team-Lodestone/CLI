//
// Created by zero on 11/19/26.
//

#ifndef LODESTONE_CLI_COMMAND_REGISTRY_H
#define LODESTONE_CLI_COMMAND_REGISTRY_H

#include <CLI11.hpp>
#include "Lodestone.CLI/commands/ICommand.h"

namespace LodestoneCLI::Commands {
    class CommandRegistry
    {
        public:
        template<typename T> void registerCommand()
        {
            static_assert(
                std::is_base_of_v<ICommand, T>,
                "Command must inherit from ICommand"
            );

            commands.push_back(std::make_unique<T>());
        }

        void registerCommands(CLI::App& app) {
            for (const auto& command : commands) {
                auto builder = command->registerCommand();

                builder->build(&app);

                m_commandBuilders.push_back(std::vector<std::shared_ptr<CommandBuilder> >::value_type(builder));
            }
        }

        private:
        std::vector<std::unique_ptr<ICommand>> commands;

        std::vector<std::shared_ptr<CommandBuilder>> m_commandBuilders;
    };
}

#endif //LODESTONE_CLI_COMMAND_REGISTRY_H
