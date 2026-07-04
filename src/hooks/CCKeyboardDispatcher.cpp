#include "../Global.hpp"

#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/ui/Button.hpp>

#include "../Events.hpp"

using namespace geode::prelude;

class $modify(MyHookLol, CCKeyboardDispatcher) {
	// Needed because BetterInfo has special FLAlertLayers that duplicate for some reason
	bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool isRepeat, double time) {
		if (!down)
			return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, isRepeat, time);

		// if (key == KEY_I) {
		// 	global::debugVal++;
		// 	DeltaruneTextShaders::updateLastShader();
		// }
		// else if (key == KEY_U) {
		// 	global::debugVal--;
		// 	DeltaruneTextShaders::updateLastShader();
		// }

		
		if (key == KEY_U) {
			auto characterSprite1 = CircleButtonSprite::create(nullptr, CircleBaseColor::Blue, CircleBaseSize::Large);
			auto characterSprite2 = CircleButtonSprite::create(nullptr, CircleBaseColor::Cyan, CircleBaseSize::Large);
			std::vector<DeltaruneDialogObject> objects;
			objects.emplace_back(characterSprite1, "Queen", "Queen", "Poor Noelle She Would Love Hitting All These Cars");
			objects.emplace_back(characterSprite2, "Noelle", "Noelle", "Umm, actually, I... she, um, might not, um...");
			objects.emplace_back(characterSprite1, "Queen", "Queen", "Stop Everything");
			objects.emplace_back(characterSprite1, "Queen", "Queen", "Kris Get The Banana");
			objects.emplace_back(characterSprite1, "Queen", "Queen", "Potassium");
			CreateDialogFull().send(nullptr, objects);
		}

		if (global::blockKeys && Loader::get()->isModLoaded("cvolton.betterinfo")) {
			if (key == KEY_Left || key == KEY_Right)
				return true;
			else if (key == KEY_Escape)
				global::blockKeys = false;
		}

		return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, isRepeat, time);
	}
};