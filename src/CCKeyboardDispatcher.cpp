#include "Global.hpp"

#include <Geode/ui/Button.hpp>

#include "../api/Events.hpp"

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
			auto debugButton = CircleButtonSprite::create(nullptr);
			auto debugButton2 = CircleButtonSprite::create(nullptr, CircleBaseColor::Cyan);
			std::vector<DeltaruneDialogObject> objects;
			objects.emplace_back(debugButton, "Sans", "Title 1", "Text 1");
			objects.emplace_back(debugButton2, "Queen", "Title 2", "Text 2");
			DeltaruneEvents::CreateDialogFull().send(nullptr, objects);
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