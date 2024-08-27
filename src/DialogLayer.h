#include "include.h"

class $modify(DeltaruneDialogLayer, DialogLayer) {
	struct Fields {
		bool doneRolling = false;
		bool rolledPage = false;
		int waitQueue = 0;
		int rollingLine = 0;
		int linesProgressed = 0;
		int characterCount = 0;
		bool done = false;
		gd::string text = "";
		float screenSize = CCDirector::sharedDirector()->getWinSize().width;
		int textSize = 36;
		int btnSelected = 0;
		bool dontRestrictWidth = Mod::get()->getSettingValue<bool>("dontRestrictWidth");
		bool disableClickToProgress = Mod::get()->getSettingValue<bool>("disableClickToProgress");
		CCNode* mainLayer;
		CCNode* textAreaClippingNode;
		CCNode* textArea;
		CCNode* gradientOverlay;
		CCNode* bg;
		DialogObject* p0;
	};
	bool init(DialogObject*, CCArray*, int);
	void displayDialogObject(DialogObject*);
	void closeWithDelay(float);
};