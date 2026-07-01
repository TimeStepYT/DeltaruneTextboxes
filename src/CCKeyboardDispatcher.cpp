#include "Global.hpp"

#include <Geode/ui/Button.hpp>

#include "Events.hpp"

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

		auto debugButton = CircleButtonSprite::create(nullptr);

		if (key == KEY_U) {
			DeltaruneEvents::createDialogEvent.send(debugButton, "Button test", "Text in here\ntest\ntest\ntest\ntest");
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