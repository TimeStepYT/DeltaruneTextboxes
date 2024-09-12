#include <Geode/Geode.hpp>

#include <Geode/fmod/fmod.hpp>
#if !defined(GEODE_IS_MACOS) || !defined(DEBUG_MAC_INPUTS)
#include <geode.custom-keybinds/include/Keybinds.hpp>
#endif
#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/DialogLayer.hpp>
#include <vector>
#include <random>

using namespace geode::prelude;

extern bool blockKeys;