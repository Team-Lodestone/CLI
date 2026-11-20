//
// Created by zero on 11/19/26.
//

#ifndef LODESTONE_CLI_ICOMMAND_H
#define LODESTONE_CLI_ICOMMAND_H

#pragma once

#include <CLI11.hpp>

namespace LodestoneCLI::Commands {
    class ICommand {
    public:
        virtual ~ICommand() = default;

        virtual void registerCommand(CLI::App& app) = 0;
        virtual void execute() = 0;
    };
}

#endif //LODESTONE_CLI_ICOMMAND_H
