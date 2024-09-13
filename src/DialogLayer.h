#include "include.h"

class $modify(DeltaruneDialogLayer, DialogLayer) {
	struct Fields {
		DialogObject* dialogObject;
		FLAlertLayer* alert;
	};
	bool init(DialogObject*, CCArray*, int);
	void displayNextObject();
	void displayDialogObject(DialogObject*);
	bool ccTouchBegan(CCTouch*, CCEvent*) override;
	void keyDown(enumKeyCodes) override;
};