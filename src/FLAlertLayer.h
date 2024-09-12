#include "include.h"

class $modify(DeltaruneAlertLayer, FLAlertLayer) {
	struct Fields {
		bool clickedChoice = false;
		bool reverseAnim = false;
		bool showing = false;
		bool incompatible = false;
		bool dialog = false;
		bool noShadow = false;
		bool doneRolling = false;
		bool rolledPage = false;
		bool playedSound = false;
		bool done = false;
		bool dontRestrictWidth = Mod::get()->getSettingValue<bool>("dontRestrictWidth");
		bool disableClickToProgress = Mod::get()->getSettingValue<bool>("disableClickToProgress");
		float screenSize = CCDirector::sharedDirector()->getWinSize().width;
		int waitQueue = 0;
		int frame = 0;
		int rollingLine = 0;
		int linesProgressed = 0;
		int characterCount = 0;
		int textSize = 36;
		int btnSelected = 0;
		CCMenuItemSpriteExtra* btn1;
		CCMenuItemSpriteExtra* btn2;
		CCNode* textAreaClippingNode;
		CCNode* textArea;
		CCNode* gradientOverlay;
		CCNode* shadow;
		CCNode* bg;
		CCSprite* heart;
		CCSpriteGrayscale* characterSprite;
		CCLabelBMFont* title;
		FMOD::System* system = FMODAudioEngine::sharedEngine()->m_system;
		FMOD::Channel* channel;
		FMOD::Sound* sound;
		std::string text = "";
		std::string textSound = Mod::get()->getSettingValue<std::string>("textSound");
		std::unordered_map<std::string, std::string> nameToFile;
		std::unordered_map<std::string, std::string> nameToSound;
	};
	void animateBG(float);
	void changeBG();
	void changeSingleButton(CCMenuItemSpriteExtra*, ButtonSprite*);
	void changeButtons();
	void changeTitle();
	void changeText();
	void changeLook();
	void decideToBlockKeys();
	void show() override;
	void initMaps();
	bool init(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float, float);
	void rollText(float);
	void showButtons();
	void setHeartPosition(CCNode*);
	int getLinesLeft();
	void skipText();
	void progressText();
	// I can't check for enter key so I guess I have to hook the buttons
	void onBtn2(CCObject*);
	void onBtn1(CCObject*);
	void clickedOnButton(CCMenuItemSpriteExtra*, ButtonSprite*, int);
	bool ccTouchBegan(CCTouch*, CCEvent*) override;

	#if defined(GEODE_IS_MACOS) || defined(DEBUG_MAC_INPUT)
	void keyDown(enumKeyCodes) override;
	#else 
	void initCustomKeybinds();
	#endif
};