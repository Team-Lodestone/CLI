//
// Created by zero on 11/19/26.
//

#include "Lodestone.CLI/commands/InfoCommand.h"

void LodestoneCLI::Commands::InfoCommand::registerCommand(CLI::App &app) {
    auto* info =
        app.add_subcommand("info", "Displays information about a given level/world");

    info->callback([this] {
        execute();
    });
}

void LodestoneCLI::Commands::InfoCommand::execute() {
}
