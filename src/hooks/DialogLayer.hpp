#pragma once

#include <Geode/modify/DialogLayer.hpp>

class $modify(DeltaruneDialogLayer, DialogLayer) {
	struct Fields {
		DialogObject* dialogObject;
		FLAlertLayer* alert;
		CCLayer* dialogMainLayer;
	};
	void displayNextObject();
	void displayDialogObject(DialogObject*);
	void changeDialogDesign(DialogObject*);
#ifndef GEODE_IS_IOS
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) override;
	void keyDown(cocos2d::enumKeyCodes, double) override;
#endif
};