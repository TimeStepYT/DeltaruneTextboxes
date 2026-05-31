#ifdef GEODE_IS_MACOS

#include "PlatformToolbox.hpp"

bool DeltarunePlatformToolbox::isControllerConnected() {
    if (PlatToolbox::disable)
        return false;

    return PlatformToolbox::isControllerConnected();
}

#endif