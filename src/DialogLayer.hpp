#include "include.hpp"

class $modify(DeltaruneDialogLayer, DialogLayer) {
	struct Fields {
		DialogObject* dialogObject;
		FLAlertLayer* alert;
		CCLayer* dialogMainLayer;
	};
	bool init(DialogObject*, CCArray*, int);
	void displayNextObject();
	void displayDialogObject(DialogObject*);
	bool ccTouchBegan(CCTouch*, CCEvent*) override;
	void keyDown(enumKeyCodes) override;
};