
#ifndef LODESTONE_CLI_SAVE_FORMAT_H
#define LODESTONE_CLI_SAVE_FORMAT_H

#include <algorithm>
#include <optional>
#include <string_view>
#include <vector>

#include "Lodestone.Common/registry/Identifier.h"

namespace LodestoneCLI::World {

    enum class SaveType {
        Level,
        World
    };

    struct SaveFormatRange {
        std::string_view first;
        std::string_view last;
        SaveType type;
        lodestone::common::registry::Identifier format;
    };

    namespace SaveFormats {
        inline const std::vector<SaveFormatRange> RANGES = {
            // Classic
            {
                .first = "c0_0_2a", .last = "c0_0_9a",
                .type = SaveType::Level, .format = {"lodestone", "minecraft/java/io/level/minev1"}
            },
            {
                .first = "c0_0_10a", .last = "c0_30_CPE",
                .type = SaveType::World, .format = {"lodestone", "minecraft/java/io/world/minev2"}
            },

            // Indev
            {
                .first = "in20091223_1457", .last = "in20100219",
                .type = SaveType::Level, .format = {"lodestone", "minecraft/java/io/nbt/level/mclevel"}
            },

            // Infdev
            {
                .first = "inf20100624", .last = "inf20100624",
                .type = SaveType::World, .format = {"lodestone", "minecraft/java/io/world/inf_624"}
            },

            // Alpha
            {
                .first = "inf20100625_1917", .last = "b1_2",
                .type = SaveType::World, .format = {"lodestone", "minecraft/java/io/world/alpha"}
            },

            // McRegion
            {
                .first = "b1_3", .last = "r1_2_1",
                .type = SaveType::World, .format = {"lodestone", "minecraft/java/io/world/mcregion"}
            }
        };

    } // namespace SaveFormats

    inline const SaveFormatRange* getSaveFormat(const std::string_view version) {
        for (const auto& format : SaveFormats::RANGES) {
            if (version >= format.first && version <= format.last) {
                return &format;
            }
        }

        return nullptr;
    }

} // namespace LodestoneCLI::World

#endif // LODESTONE_CLI_SAVE_FORMAT_H
