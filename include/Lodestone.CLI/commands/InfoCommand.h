//
// Created by zero on 11/19/26.
//

#ifndef LODESTONE_CLI_INFO_COMMAND_H
#define LODESTONE_CLI_INFO_COMMAND_H

#include "Lodestone.CLI/commands/ICommand.h"

namespace LodestoneCLI::Commands {
    class InfoCommand : public ICommand {
    public:
        void registerCommand(CLI::App &app) override;

        void execute() override;
    };
}

#endif //LODESTONE_CLI_INFO_COMMAND_H
