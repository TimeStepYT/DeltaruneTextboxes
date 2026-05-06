#if GEODE_MACOS

#include "PlatformToolbox.hpp"

bool DeltarunePlatformToolbox::isControllerConnected() {
    if (PlatToolbox::disable)
        return false;

    return PlatformToolbox::isControllerConnected();
}

#endif