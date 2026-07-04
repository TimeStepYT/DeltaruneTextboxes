#pragma once

#include <Geode/modify/DialogLayer.hpp>

class $modify(DeltaruneDialogLayer, DialogLayer) {
	struct Fields {
		DialogObject* dialogObject;
		FLAlertLayer* alert;
		CCLayer* dialogMainLayer;
	};
	bool init(DialogObject*, cocos2d::CCArray*, int);
	void displayNextObject();
	void displayDialogObject(DialogObject*);
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) override;
	void keyDown(cocos2d::enumKeyCodes, double) override;
};