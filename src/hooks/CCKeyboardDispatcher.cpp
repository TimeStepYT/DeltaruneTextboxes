#include "../Global.hpp"

#include <Geode/modify/CCKeyboardDispatcher.hpp>
#include <Geode/ui/Button.hpp>

#include "../../api/API.hpp"

using namespace geode::prelude;
using namespace deltarune_textboxes;

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

		
		// if (key == KEY_U) {
		// 	auto characterSprite1 = CircleButtonSprite::create(nullptr, CircleBaseColor::Blue, CircleBaseSize::Large);
		// 	auto characterSprite2 = CircleButtonSprite::create(nullptr, CircleBaseColor::Cyan, CircleBaseSize::Large);
		// 	std::vector<std::shared_ptr<DeltaruneDialogObject>> objects;
		// 	objects.push_back(createDialogObject(characterSprite1, "Queen", "Queen", "Poor Noelle She Would Love Hitting All These Cars").unwrap());
		// 	objects.push_back(createDialogObject(characterSprite2, "Noelle", "Noelle", "Umm, actually, I... she, um, might not, um...").unwrap());
		// 	objects.push_back(createDialogObject(characterSprite1, "Queen", "Queen", "Stop Everything").unwrap());
		// 	objects.push_back(createDialogObject(characterSprite1, "Queen", "Queen", "Kris Get The Banana").unwrap());
		// 	objects.push_back(createDialogObject(characterSprite1, "Queen", "Queen", "Potassium").unwrap());
			
		// 	Result<FLAlertLayer*> res = deltarune_textboxes::createFullDialog(objects);
			
		// }

		if (global::blockKeys && Loader::get()->isModLoaded("cvolton.betterinfo")) {
			if (key == KEY_Left || key == KEY_Right)
				return true;
			else if (key == KEY_Escape)
				global::blockKeys = false;
		}

		return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, isRepeat, time);
	}
};