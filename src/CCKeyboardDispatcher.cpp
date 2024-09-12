#include "include.h"

#if !defined(GEODE_IS_MACOS) && !defined(DEBUG_MAC_INPUTS)
$execute{
	keybinds::BindManager::get()->registerBindable(
		{
			"left"_spr,
			"Left Choice",
			"",
			{ keybinds::Keybind::create(KEY_Left, keybinds::Modifier::None) },
			"Deltarune Textboxes"
		}
	);
	keybinds::BindManager::get()->registerBindable(
		{
			"right"_spr,
			"Right Choice",
			"",
			{ keybinds::Keybind::create(KEY_Right, keybinds::Modifier::None) },
			"Deltarune Textboxes"
		}
	);
	keybinds::BindManager::get()->registerBindable(
		{
			"progress"_spr,
			"Progress Text / Confirm Choice",
			"The button to press to get new lines in the textbox",
			{ keybinds::Keybind::create(KEY_Z, keybinds::Modifier::None), keybinds::Keybind::create(KEY_Enter, keybinds::Modifier::None) },
			"Deltarune Textboxes"
		}
	);
	keybinds::BindManager::get()->registerBindable(
		{
			"skip"_spr,
			"Skip Text",
			"The button to press to skip rolling out the text",
			{ keybinds::Keybind::create(KEY_X, keybinds::Modifier::None), keybinds::Keybind::create(KEY_Shift, keybinds::Modifier::None) },
			"Deltarune Textboxes"
		}
	);
}
#endif
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