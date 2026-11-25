//
// Created by zero on 11/19/26.
//

#ifndef LODESTONE_CLI_ICOMMAND_H
#define LODESTONE_CLI_ICOMMAND_H

#pragma once

#include "Lodestone.CLI/commands/CommandBuilder.h"

namespace LodestoneCLI::Commands {
    class ICommand {
    public:
        virtual ~ICommand() = default;

        virtual std::shared_ptr<CommandBuilder> registerCommand() = 0;
        virtual void execute() = 0;
    };
}

#endif //LODESTONE_CLI_ICOMMAND_H
