//
// Created by zero on 11/19/26.
//

#include <CLI11.hpp>

#include <vector>
#include <print>

#include "Lodestone.CLI/commands/CommandRegistry.h"
#include "Lodestone.CLI/commands/ConvertCommand.h"
#include "Lodestone.CLI/commands/InfoCommand.h"
#include "Lodestone.CLI/commands/ValidateCommand.h"
#include "Lodestone.Core/Lodestone.h"
#include "Lodestone.Core/loader/NativeExtensionLoader.h"

int main(const int argc, char** argv)
{
    CLI::App app{"Lodestone-CLI"};
    argv = app.ensure_utf8(argv);

    auto* lodestone = lodestone::core::Lodestone::getInstance();
    // std::print("== Loading extensions ==\n");

    lodestone::core::loader::NativeExtensionLoader l("extensions/", lodestone);

    // l.extensionLoadingEvent += [](lodestone::common::event::Cancellable &/*cancellable*/, const std::filesystem::path &p) {
    //     std::println("Initializing extension '{}'", p.generic_string());
    // };

    // l.extensionLoadedEvent += [](const lodestone::core::LodestoneExtension *ext) {
    //     std::println("Initialized extension '{}' {}", ext->getIdentifier(), ext->getVersion().toString());
    // };

    l.load();

    //region Command Registry
    LodestoneCLI::Commands::CommandRegistry commandRegistry;
    commandRegistry.registerCommand<LodestoneCLI::Commands::InfoCommand>();
    commandRegistry.registerCommand<LodestoneCLI::Commands::ConvertCommand>();
    commandRegistry.registerCommand<LodestoneCLI::Commands::ValidateCommand>();
    commandRegistry.registerCommands(app);
    //endregion

    // Parse and execute commands
    try {
        app.parse(argc, argv);
    } catch (const CLI::ParseError& e) {
        return app.exit(e);
    }

    // No subcommand was entered
    // Display help information
    if (app.get_subcommands().empty())
    {
        std::cout << app.help();
        return 0;
    }

    return 0;
}
