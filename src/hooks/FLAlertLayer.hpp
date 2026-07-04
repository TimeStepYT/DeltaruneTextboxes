#pragma once

#include <Geode/modify/FLAlertLayer.hpp>

#include <Geode/ui/Button.hpp>
#include <Geode/ui/TextArea.hpp>
#include "../DeltaruneTextArea.hpp"
#include "../ImageNode.hpp"
#include <alphalaneous.alphas-ui-pack/include/API.hpp>
#include "../Events.hpp"

namespace DeltaruneMaps {
	enum Character {
		DEFAULT,
		TYPEWRITER,
		TORIEL,
		SANS,
		PAPYRUS,
		UNDYNE,
		ALPHYS,
		ASGORE,
		ASRIEL,
		SUSIE,
		RALSEI,
		LANCER,
		JEVIL,
		NOELLE,
		BERDLY,
		QUEEN,
		SPAMTON,
		SPAMTON_NEO,
		TENNA,
		CAROL,
		GERSON,
		JACKENSTEIN,
		FLOWERY,
		NUM_CHARACTERS,
		NONE // This is after NUM_CHARACTERS intentionally because it's a special case
	};
	
	struct CharacterData {
		std::string sound = "SND_TXT1";
		int soundRate = 2;
		bool hasPitchVariation = false;
	};

	inline std::unordered_map<Character, CharacterData> characterToData;
	inline std::unordered_map<std::string, Character> nameToCharacter;
	inline std::unordered_map<std::string, Character> titleToCharacter;

	void init();
}

class $modify(DeltaruneAlertLayer, FLAlertLayer) {
	struct Fields {
		bool clickedChoice = false;
		bool reverseAnim = false;
		bool showing = false;
		bool incompatible = false;
		bool dialog = false;
		bool noShadow = false;
		bool noGradient = false;
		bool doneRolling = false;
		bool rolledPage = false;
		bool done = false;
		bool disableClickToProgress = geode::Mod::get()->getSettingValue<bool>("disableClickToProgress");
		float screenSize = 0.f;
		float lostTime = 0;
		float contentXOffset = 0.f;
		int waitQueue = 0;
		int frame = 0;
		int rollingLine = 0;
		int linesProgressed = 0;
		int characterCount = 0;
		int textSize = 31;
		int btnSelected = 0;
		int dialogCount = 0;
		int prevSoundNum = 200;
		int soundRate = 2;
		int soundTimer = 0;
		CCMenuItemSpriteExtra* old_btn1 = nullptr;
		CCMenuItemSpriteExtra* old_btn2 = nullptr;
		geode::Button* btn1 = nullptr;
		geode::Button* btn2 = nullptr;
		CCNode* textContentNode = nullptr;
		TextArea* old_textArea = nullptr;
		std::shared_ptr<DeltaruneTextArea> m_textArea = nullptr;
		cocos2d::extension::CCScale9Sprite* bg = nullptr;
		cocos2d::CCLabelBMFont* title = nullptr;
		cocos2d::CCSprite* heart = nullptr;
		alpha::ui::RenderNode* renderedSprite = nullptr;
		ImageNode* imageNode;
		DialogLayer* dialogLayer;
		FMOD::System* system = FMODAudioEngine::sharedEngine()->m_system;
		FMOD::Channel* channel = nullptr;
		FMOD::Sound* sound = nullptr;
		std::vector<std::string> characterSpriteNames;
		std::vector<std::string> titles;
		std::string text = "";
		DeltaruneMaps::Character textSound = DeltaruneMaps::Character::NONE;
		std::vector<DeltaruneDialogObject> nextAlerts;
	};
	void registerKeybinds();
	void animateBG(float);
	void changeBG();
	void changeSingleButton(CCMenuItemSpriteExtra*, ButtonSprite*);
	void changeButtons();
	void changeTitle();
	void handleSound();
	cocos2d::CCLabelBMFont* createStar();
	void changeText();
	void changeLook();
	void decideToBlockKeys();
	void show() override;
	void initSoundRate();
	bool init(FLAlertLayerProtocol*, char const*, gd::string, char const*, char const*, float, bool, float, float);
	void rollText(float);
	void showButtons();
	void setHeartPosition(geode::Button*);
	int getLinesLeft();
	void skipText();
	int emptyLinesAmount(int = 0);
	ImageNode* createImageNode();
	void handleAprilFools();
	void progressText();
	void pickChoice();
	void onBtn2(CCObject*);
	void onBtn1(CCObject*);
	void clickedOnButton(geode::Button*, geode::Button*, int);
	bool ccTouchBegan(cocos2d::CCTouch*, cocos2d::CCEvent*) override;
	void playSound(char);
	void updateRenderTexture();
	void setTextSound(DeltaruneMaps::Character textSound);
	// A function to execute after the textbox is closed
	void setNextAlerts(std::vector<DeltaruneDialogObject> const& nextAlerts);
};