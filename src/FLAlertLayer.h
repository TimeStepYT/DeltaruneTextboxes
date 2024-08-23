#include "include.h"

class $modify(MyFLAlertLayer, FLAlertLayer) {
	struct Fields {
		int linesProgressed = 0;
		bool done = false;
		gd::string text = "";
		float screenSize = CCDirector::sharedDirector()->getWinSize().width;
		int textSize = 36;
		int btnSelected = 0;
		bool dontRestrictWidth = Mod::get()->getSettingValue<bool>("dontRestrictWidth");
		bool disableClickToProgress = Mod::get()->getSettingValue<bool>("disableClickToProgress");
		CCNode* mainLayer;
		CCNode* btn1;
		CCNode* btn2;
		CCNode* textAreaClippingNode;
		CCNode* textArea;
		CCNode* gradientOverlay;
		CCNode* bg;
		CCNode* title;
	};
	void changeBG();
	void changeButtons();
	void changeTitle();
	void changeText();
	void changeLook();
	bool init(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float, float);
	void showButtons(CCArrayExt<CCLabelBMFont*>);
	// I can't check for enter key so I guess I have to hook the buttons
	void onBtn2(CCObject*);
	void onBtn1(CCObject*);
	int getLinesLeft(auto&);
	void progressText();
	void show();
	void addHeart(CCNode*, CCLabelBMFont*);
	bool ccTouchBegan(CCTouch*, CCEvent*);
	void keyDown(enumKeyCodes);
};