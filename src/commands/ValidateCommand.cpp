//
// Created by zero on 11/19/26.
//

#include "Lodestone.CLI/commands/ValidateCommand.h"

std::shared_ptr<LodestoneCLI::Commands::CommandBuilder> LodestoneCLI::Commands::ValidateCommand::registerCommand() {
    auto command = CommandBuilder::create();

    command
            ->setCommandName("validate")
            .setCommandDescription("Validates a world against the expected format/version")
            .setCallback([this] {
                execute();
            });

    return command;
}

void LodestoneCLI::Commands::ValidateCommand::execute() {
}
