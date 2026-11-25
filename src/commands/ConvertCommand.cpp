//
// Created by zero on 11/19/26.
//

#include "Lodestone.CLI/commands/ConvertCommand.h"

#include "Lodestone.Core/Lodestone.h"

#include <iostream>

#include "Lodestone.CLI/commands/CommandBuilder.h"
#include "Lodestone.CLI/utils/IdentifierUtils.h"
#include "Lodestone.CLI/world/SaveFormat.h"
#include "Lodestone.Conversion/io/options/fs/FilesystemPathOptions.h"
#include "Lodestone.Conversion/io/options/versioned/VersionedOptions.h"
#include "Lodestone.Conversion/registry/Registries.h"
#include "Lodestone.Level/world/World.h"

std::shared_ptr<LodestoneCLI::Commands::CommandBuilder> LodestoneCLI::Commands::ConvertCommand::registerCommand()
{
    const auto inputOption = OptionBuilder::create()
        ->setOptionName("input")
        .setOptionDescription("Level/World to be converted")
        .addRequirement(OptionRequirements::EXISTING_FILE_OR_DIR)
        .addAlias("-i")
        .setCallback(input_path)
        .isRequired(true)
        .build();

    const auto outputOption = OptionBuilder::create()
        ->setOptionName("output")
        .setOptionDescription("Converted level/world destination")
        .addAlias("-o")
        .setCallback(output_path)
        .isRequired(true)
        .build();

    const auto fromOption = OptionBuilder::create()
        ->setOptionName("from")
        .setOptionDescription("Source version")
        .addAlias("-f")
        .setCallback(m_fromVersion)
        .isRequired(true)
        .build();

    const auto toOption = OptionBuilder::create()
        ->setOptionName("to")
        .setOptionDescription("Destination version")
        .addAlias("-t")
        .setCallback(m_toVersion)
        .isRequired(true)
        .build();

    // TODO: --io-from / --io-to options

    auto command = CommandBuilder::create();

    command
        ->setCommandName("convert")
        .setCommandDescription("Converts a level/world to another format or version")
        .addOption(inputOption)
        .addOption(outputOption)
        .addOption(fromOption)
        .addOption(toOption)
        .setCallback([this] {
            execute();
        });

    return command;
}

void LodestoneCLI::Commands::ConvertCommand::execute()
{
    const auto lodestone = lodestone::core::Lodestone::getInstance();

    if (this->m_verbose)
    {
        LOG_INFO("Input:       " + input_path.string());
        LOG_INFO("Output:      " + output_path.string());
        LOG_INFO("From:        " + m_fromVersion);
        LOG_INFO("To:          " + m_toVersion);
    }

    //region Validate to/from versions
    const auto versionRegistry = lodestone->getVersionRegistry();

    std::ranges::replace(m_fromVersion, '.', '_');
    std::ranges::replace(m_toVersion, '.', '_');

    const std::optional<lodestone::common::registry::Identifier> from = Utils::IdentifierUtils::tryParse(m_fromVersion);
    if (!from.has_value())
    {
        LOG_ERROR(m_fromVersion + " is not a correctly formatted version!");
        return;
    }
    if (versionRegistry.isValid(from.value()))
    {
        LOG_ERROR(m_fromVersion + " is not a valid version!");
        return;
    }

    const std::optional<lodestone::common::registry::Identifier> to = Utils::IdentifierUtils::tryParse(m_toVersion);
    if (!to.has_value())
    {
        LOG_ERROR(m_toVersion + " is not a correctly formatted version!");
        return;
    }
    if (versionRegistry.isValid(to.value()))
    {
        LOG_ERROR(m_toVersion + " is not a valid version!");
        return;
    }
    //endregion

    //region Read world
    const auto* fromSaveFormat = World::getSaveFormat(m_fromVersion);

    if (!fromSaveFormat) {
        throw std::runtime_error("Unsupported save format");
    }

    std::unique_ptr<lodestone::level::world::World> fromWorld;
    if (fromSaveFormat->type == World::SaveType::Level) {
       LOG_INFO("LEVEL!");
    } else {
        // const auto* inputIo = lodestone::conversion::registry::WorldIORegistry::getInstance().get(fromSaveFormat->format);
        //
        // fromWorld = inputIo->read(
        //     minecraft::common::conversion::io::options::OptionPresets::CommonFilesystemOptions{
        //         lodestone::conversion::io::options::fs::FilesystemPathOptions{
        //             temp_dir
        //         },
        //         lodestone::conversion::io::options::versioned::VersionedOptions{
        //             minecraft::java::Version::b1_3
        //         }
        //     });
        // LOG_INFO("WORLD!");
    }
    //endregion

    //region Write world
    const auto* toSaveFormat = World::getSaveFormat(m_toVersion);

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
