//
// Created by zero on 11/19/26.
//

#ifndef LODESTONE_CLI_INFO_COMMAND_H
#define LODESTONE_CLI_INFO_COMMAND_H

#include "Lodestone.CLI/commands/ICommand.h"

namespace LodestoneCLI::Commands {
    class InfoCommand : public ICommand {
    public:
        std::shared_ptr<CommandBuilder> registerCommand() override;

        void execute() override;
    };
}

#endif //LODESTONE_CLI_INFO_COMMAND_H
