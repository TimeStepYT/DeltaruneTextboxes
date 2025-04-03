#include <Geode/Geode.hpp>

#include <Geode/fmod/fmod.hpp>

#if !defined(DISABLE_KEYBOARD)
#include <geode.custom-keybinds/include/Keybinds.hpp>
#endif

#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/modify/DialogLayer.hpp>
#include <Geode/modify/PurchaseItemPopup.hpp>
#include <vector>
#include <random>

using namespace geode::prelude;

extern bool blockKeys;