//
// Created by zero on 11/19/26.
//

#include "Lodestone.CLI/commands/ConvertCommand.h"

#include "Lodestone.Core/Lodestone.h"

#include <iostream>

#include "Lodestone.CLI/utils/IdentifierUtils.h"
#include "Lodestone.CLI/world/SaveFormat.h"
#include "Lodestone.Conversion/io/options/fs/FilesystemPathOptions.h"
#include "Lodestone.Conversion/registry/Registries.h"
#include "Lodestone.Level/world/World.h"

void LodestoneCLI::Commands::ConvertCommand::registerCommand(CLI::App &app)
{
    auto* convert =
    app.add_subcommand("convert", "Converts a level/world to another format or version");

    convert->add_option("-i,--input", input_path, "World to be converted")->required()->check(CLI::ExistingPath);

    convert->add_option("-o,--output", output_path, "Converted world")->required();

    convert->add_option("-f,--from", from_version, "Source version")
        ->required();

    convert->add_option("-t,--to", to_version, "Destination version")
        ->required();

    convert->add_option("-v,--verbose", m_verbose, "Verbose output");

    convert->callback([this] {
        execute();
    });
}

void LodestoneCLI::Commands::ConvertCommand::execute()
{
    const auto lodestone = lodestone::core::Lodestone::getInstance();

    //region Validate to/from versions
    const auto versionRegistry = lodestone->getVersionRegistry();

    std::ranges::replace(from_version, '.', '_');
    std::ranges::replace(to_version, '.', '_');

    const std::optional<lodestone::common::registry::Identifier> from = Utils::IdentifierUtils::tryParse(from_version);
    if (!from.has_value())
    {
        throw std::runtime_error(from_version + " is not a correctly formatted version!");
    }
    if (versionRegistry.isValid(from.value()))
    {
        throw std::runtime_error(from_version + " is not a valid version!");
    }

    const std::optional<lodestone::common::registry::Identifier> to = Utils::IdentifierUtils::tryParse(to_version);
    if (!to.has_value())
    {
        throw std::runtime_error(to_version + " is not a correctly formatted version!");
    }
    if (versionRegistry.isValid(to.value()))
    {
        throw std::runtime_error(to_version + " is not a valid version!");
    }
    //endregion

    if (this->m_verbose)
    {
        std::cout << std::endl;
        std::cout << "Input:       " << input_path.string() << std::endl;
        std::cout << "Output:      " << output_path.string() << std::endl;
        std::cout << "From:        " << from_version << std::endl;
        std::cout << "To:          " << to_version << std::endl;
    }

    //region Read world
    const auto* fromSaveFormat = World::getSaveFormat(from_version);

    if (!fromSaveFormat) {
        throw std::runtime_error("Unsupported save format");
    }

    std::unique_ptr<lodestone::level::world::World> fromWorld;
    if (fromSaveFormat->type == World::SaveType::Level) {
        std::cout << "LEVEL!" << std::endl;
    } else {
        const auto* inputIo = lodestone::conversion::registry::WorldIORegistry::getInstance().get(fromSaveFormat->format);

        // fromWorld = inputIo->read(
        //     minecraft::common::conversion::io::options::OptionPresets::CommonFilesystemOptions{
        //         lodestone::conversion::io::options::fs::FilesystemPathOptions{
        //             temp_dir
        //         },
        //         conversion::io::options::versioned::VersionedOptions{
        //             minecraft::java::Version::b1_3
        //         }
        //     });
        std::cout << "WORLD!" << std::endl;
    }
    //endregion

    //region Write world
    const auto* toSaveFormat = World::getSaveFormat(to_version);

    if (!toSaveFormat) {
        throw std::runtime_error("Unsupported save format");
    }

    if (toSaveFormat->type == World::SaveType::Level) {
        std::cout << "LEVEL!" << std::endl;
    } else {
        std::cout << "WORLD!" << std::endl;
    }
    //endregion
}
