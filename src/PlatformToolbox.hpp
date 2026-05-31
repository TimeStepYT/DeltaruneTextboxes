#pragma once

#ifdef GEODE_IS_MACOS

#include <Geode/Geode.hpp>

#include <Geode/modify/PlatformToolbox.hpp>

namespace PlatToolbox {
    inline bool disable = false;
}

class $modify(DeltarunePlatformToolbox, PlatformToolbox) {
    static bool isControllerConnected();
};

#endif