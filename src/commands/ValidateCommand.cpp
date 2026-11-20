//
// Created by zero on 11/19/26.
//

#include "Lodestone.CLI/commands/ValidateCommand.h"

void LodestoneCLI::Commands::ValidateCommand::registerCommand(CLI::App &app) {
    auto* validate =
        app.add_subcommand("validate", "Validates a world against the expected format/version");

    validate->callback([this] {
        execute();
    });
}

void LodestoneCLI::Commands::ValidateCommand::execute() {
}
