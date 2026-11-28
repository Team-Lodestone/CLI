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
#include "Lodestone.Conversion/registry/Registries.h"
#include "Lodestone.Level/world/World.h"

std::shared_ptr<LodestoneCLI::Commands::CommandBuilder> LodestoneCLI::Commands::ConvertCommand::registerCommand() {
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

void LodestoneCLI::Commands::ConvertCommand::execute() {
    const auto lodestone = lodestone::core::Lodestone::getInstance();

    if (this->m_verbose) {
        LOG_INFO("Input:       " + input_path.string());
        LOG_INFO("Output:      " + output_path.string());
        LOG_INFO("From:        " + m_fromVersion);
        LOG_INFO("To:          " + m_toVersion);
    }

    //region Validate to/from versions
    const auto versionRegistry = lodestone->getVersionRegistry();

    std::ranges::replace(m_fromVersion, '.', '_');
    std::ranges::replace(m_toVersion, '.', '_');

    for (auto &version: versionRegistry.getVersions("java")) {
        LOG_INFO("Registered: java:" + version);
    }
    const auto separator = m_fromVersion.find(':');

    if (separator == std::string::npos) {
        LOG_ERROR("Invalid version format");
        return;
    }

    const auto from_edition = m_fromVersion.substr(0, separator);
    const auto from_versionName = m_fromVersion.substr(separator + 1);

    if (!versionRegistry.isValid(from_edition, from_versionName)) {
        LOG_ERROR(m_fromVersion + " is not a valid version!");
        return;
    }

    const auto to_edition = m_toVersion.substr(0, separator);
    const auto to_versionName = m_toVersion.substr(separator + 1);

    if (!versionRegistry.isValid(to_edition, to_versionName)) {
        LOG_ERROR(m_toVersion + " is not a valid version!");
        return;
    }
    //endregion

    //region Read world
    const auto *fromSaveFormat = World::getSaveFormat(m_fromVersion);

    if (!fromSaveFormat) {
        throw std::runtime_error("Unsupported save format");
    }

    std::unique_ptr<lodestone::level::world::World> fromWorld;
    if (fromSaveFormat->type == World::SaveType::Level) {
        LOG_ERROR("Levels are unsupported at this time!");
    } else {
        const auto edition = m_fromVersion.substr(0, m_fromVersion.find(':'));
        const auto &registry = lodestone::conversion::registry::IODelegateRegistry::getInstance();
        const std::string delegatePath = "minecraft/" + edition + "/io";

        const lodestone::common::registry::Identifier worldIo{
            "lodestone",
            delegatePath.c_str()
        };
        const auto delegate = registry.get(worldIo);
        const auto version = versionRegistry.getVersionId(from_edition, from_versionName);
        if (!version) {
            LOG_ERROR("Could not resolve input version ID");
            return;
        }

        if (!delegate) {
            LOG_ERROR("No IO delegate registered for edition: " + edition);
            return;
        }

        fromWorld = delegate->readWorld(
            lodestone::conversion::io::options::fs::FilesystemPathOptions{
                input_path
            },
            lodestone::conversion::io::options::versioned::VersionedOptions{
                version.value()
            }
        );
    }
    //endregion
    if (fromWorld) {
        LOG_INFO("Successfully read world!");
    }

    //region Write world
    const auto *toSaveFormat = World::getSaveFormat(m_toVersion);

    if (!toSaveFormat) {
        throw std::runtime_error("Unsupported save format");
    }

    if (toSaveFormat->type == World::SaveType::Level) {
        LOG_ERROR("Levels are unsupported at this time!");
    } else {
        const auto edition = m_toVersion.substr(0, m_toVersion.find(':'));
        const auto &registry = lodestone::conversion::registry::IODelegateRegistry::getInstance();
        const std::string delegatePath = "minecraft/" + edition + "/io";

        const lodestone::common::registry::Identifier worldIo{
            "lodestone",
            delegatePath.c_str()
        };
        const auto delegate = registry.get(worldIo);
        const auto version = versionRegistry.getVersionId(to_edition, to_versionName);
        if (!version) {
            LOG_ERROR("Could not resolve input version ID");
            return;
        }

        if (!delegate) {
            LOG_ERROR("No IO delegate registered for edition: " + edition);
            return;
        }

        // TODO: Add options
        delegate->writeWorld(lodestone::conversion::io::options::fs::FilesystemPathOptions{
                                 output_path
                             },
                             lodestone::conversion::io::options::versioned::VersionedOptions{
                                 static_cast<int>(version.value())
                             }, fromWorld);

        LOG_INFO("Successfully wrote world!");
    }

    //endregion
}
