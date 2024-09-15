#include <Geode/Geode.hpp>

#define OLD_INPUTS defined(GEODE_IS_MACOS) || defined(DEBUG_MAC_INPUTS) || defined(GEODE_IS_ANDROID32)

#include <Geode/fmod/fmod.hpp>
#if !OLD_INPUTS
#include <geode.custom-keybinds/include/Keybinds.hpp>
#endif
#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/DialogLayer.hpp>
#include <vector>
#include <random>

using namespace geode::prelude;

extern bool blockKeys;