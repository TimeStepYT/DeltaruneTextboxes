#include "include.hpp"
#include "ImageNode.hpp"

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
		bool done = false;
		bool dontRestrictWidth = Mod::get()->getSettingValue<bool>("dontRestrictWidth");
		bool disableClickToProgress = Mod::get()->getSettingValue<bool>("disableClickToProgress");
		float screenSize = CCDirector::sharedDirector()->getWinSize().width;
		float lostTime = 0;
		int waitQueue = 0;
		int frame = 0;
		int rollingLine = 0;
		int linesProgressed = 0;
		int characterCount = 0;
		int textSize = 36;
		int btnSelected = 0;
		int dialogCount = 0;
		int prevSoundNum = 200;
		int soundRate = 2;
		int soundTimer = 0;
		CCMenuItemSpriteExtra* btn1;
		CCMenuItemSpriteExtra* btn2;
		CCNode* textAreaClippingNode;
		CCNode* textArea;
		CCNode* gradientOverlay;
		CCNode* shadow;
		CCNode* bg;
		CCSprite* heart;
		CCLabelBMFont* title;
		ImageNode* imageNode;
		DialogLayer* dialogLayer;
		FMOD::System* system = FMODAudioEngine::sharedEngine()->m_system;
		FMOD::Channel* channel = nullptr;
		FMOD::Sound* sound = nullptr;
		std::vector<std::string> characterSpriteNames;
		std::vector<std::string> titles;
		std::string text = "";
		std::string textSound = Mod::get()->getSettingValue<std::string>("textSound");
		std::unordered_map<std::string, std::string> nameToFile;
		std::unordered_map<std::string, std::string_view> nameToSound;
		std::unordered_map<std::string, int> nameToSoundRate;
	};
	void animateBG(float);
	void changeBG();
	void changeSingleButton(CCMenuItemSpriteExtra*, ButtonSprite*);
	void removeControllerGlyphs();
	void changeButtons();
	void changeTitle();
	void handleSound();
	void changeText();
	void changeLook();
	void decideToBlockKeys();
	void show() override;
	void initSoundRate();
	void initMaps();
	bool init(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float, float);
	void rollText(float);
	void showButtons();
	void setHeartPosition(CCMenuItemSpriteExtra*);
	int getLinesLeft();
	void skipText();
	int emptyLinesAmount(int = 0);
	void createImageNode();
	void handleAprilFools();
	void progressText();
	// I can't check for enter key so I guess I have to hook the buttons
	void onBtn2(CCObject*);
	void onBtn1(CCObject*);
	void clickedOnButton(CCMenuItemSpriteExtra*, ButtonSprite*, int);
	bool ccTouchBegan(CCTouch*, CCEvent*) override;
	void fixTouchPrio();
	void playSound(char);

#if defined(DISABLE_KEYBOARD)
	void keyDown(enumKeyCodes) override;
#else 
	void initCustomKeybinds();
#endif
};