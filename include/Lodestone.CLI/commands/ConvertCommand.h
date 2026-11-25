//
// Created by zero on 11/19/26.
//

#ifndef LODESTONE_CLI_CONVERT_COMMAND_H
#define LODESTONE_CLI_CONVERT_COMMAND_H

#include "CommandBuilder.h"
#include "Lodestone.CLI/commands/ICommand.h"

namespace LodestoneCLI::Commands {
    class ConvertCommand : public ICommand {
    public:
        std::shared_ptr<CommandBuilder> registerCommand() override;

        void execute() override;

    private:
        // Required parameters
        std::filesystem::path input_path;
        std::filesystem::path output_path;

        std::string m_fromVersion;
        std::string m_toVersion;

        // Optional parameters
        bool m_verbose = false;
    };
}

#endif //LODESTONE_CLI_CONVERT_COMMAND_H
