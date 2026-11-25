//
// Created by zero on 11/19/26.
//

#include "Lodestone.CLI/commands/InfoCommand.h"

std::shared_ptr<LodestoneCLI::Commands::CommandBuilder> LodestoneCLI::Commands::InfoCommand::registerCommand() {
    auto command = CommandBuilder::create();

    command
            ->setCommandName("info")
            .setCommandDescription("Displays information about a given level/world")
            .setCallback([this] {
                execute();
            });

    return command;
}

void LodestoneCLI::Commands::InfoCommand::execute() {
}
