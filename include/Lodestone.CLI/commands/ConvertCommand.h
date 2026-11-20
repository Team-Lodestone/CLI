//
// Created by zero on 11/19/26.
//

#ifndef LODESTONE_CLI_CONVERT_COMMAND_H
#define LODESTONE_CLI_CONVERT_COMMAND_H

#include "Lodestone.CLI/commands/ICommand.h"

namespace LodestoneCLI::Commands {
    class ConvertCommand : public ICommand
    {
    public:
        void registerCommand(CLI::App& app) override;
        void execute() override;
    private:
        // Required parameters
        std::filesystem::path input_path;
        std::filesystem::path output_path;

        std::string from_version;
        std::string to_version;

        // Optional parameters
        bool m_verbose = false;
    };
}

#endif //LODESTONE_CLI_CONVERT_COMMAND_H
