#pragma once

#include <Geode/Geode.hpp>

#include <Geode/fmod/fmod.hpp>

#include "hooks/FLAlertLayer.hpp"

namespace global {
    inline bool blockKeys = false;
    inline std::optional<DeltaruneMaps::CharacterData> lockedTextSound;
    // inline int debugVal = 0;
}