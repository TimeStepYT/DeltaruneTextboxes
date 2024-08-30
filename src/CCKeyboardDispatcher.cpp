#include "include.h"
class $modify(MyHookLol, CCKeyboardDispatcher) {
	// Needed because BetterInfo has special FLAlertLayers that duplicate for some reason
	bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool idk) {
		if (blockKeys && down && Loader::get()->isModLoaded("cvolton.betterinfo")) {
			if (key == enumKeyCodes::KEY_Left || key == enumKeyCodes::KEY_Right)
				return true;
			else if (key == enumKeyCodes::KEY_Escape)
				blockKeys = false;
		}
		return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, idk);
	}
};