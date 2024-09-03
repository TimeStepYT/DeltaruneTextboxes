#include "include.h"
#include "FLAlertLayer.h"

bool blockKeys = false;

float randomNumberInAGivenRangeThatGetsAddedOrRemovedFromADifferentNumber(float range) { // thanks cvolton for this amazing name
	return static_cast<float>(rand() % static_cast<int>(range * 100 + 1)) / 100.f - (range / 2.f);
}

bool DeltaruneAlertLayer::init(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale) {
	float& screenSize = m_fields->screenSize;
	if (screenSize >= 569 && !m_fields->dontRestrictWidth)
		screenSize = 569;
	std::srand(std::time(NULL));
	m_fields->text = desc;
	scroll = false;

	if (!FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale)) return false;

	NodeIDs::provideFor(this);
	setID("FLAlertLayer");
	if (Loader::get()->isModLoaded("firee.prism")) {
		if (desc == "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") {
			m_fields->incompatible = true;
			return true;
		}
	}

	auto& textArea = m_fields->textArea;
	auto& bg = m_fields->bg;
	auto& titleNode = m_fields->title;

	this->m_noElasticity = true;

	m_fields->btn1 = m_button1 ? this->m_button1->getParent() : nullptr;
	m_fields->btn2 = m_button2 ? this->m_button2->getParent() : nullptr;

	textArea = m_mainLayer->getChildByID("content-text-area");
	bg = m_mainLayer->getChildByID("background");
	titleNode = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("title"));
	Loader::get()->queueInMainThread([bg, titleNode, textArea, this] {
		if (m_fields->incompatible) return;
		if (m_fields->showing) return;

		if (!bg) return;
		if (!titleNode) return;
		if (!textArea) return;
		if (!m_mainLayer) return;

		m_fields->showing = true;
		decideToBlockKeys();
		changeLook();
		});
	return true;
}
void DeltaruneAlertLayer::showButtons() {
	if (m_button2 && getLinesLeft() < 3 && m_fields->doneRolling) {
		m_fields->done = true;
		m_buttonMenu->setVisible(true);
		m_fields->heart->setVisible(true);
	}
}

void DeltaruneAlertLayer::decideToBlockKeys() {
	int numOfSiblings = 0;
	if (auto parent = getParent()) {
		CCArrayExt<CCNode*> siblings = parent->getChildren();

		for (auto sibling : siblings) {
			if (sibling->getID() == "FLAlertLayer")
				numOfSiblings++;
		}
	}
	if (numOfSiblings >= 1 && !m_button2)
		blockKeys = true;
	else
		blockKeys = false;
}

void DeltaruneAlertLayer::onBtn2(CCObject* sender) {
	if (m_fields->incompatible) {
		FLAlertLayer::onBtn2(sender);
		return;
	}
	if (!m_fields->done) {
		progressText();
		return;
	}
	int& btnSelected = m_fields->btnSelected;
	if (btnSelected == 0) return;
	if (btnSelected == 2)
		FLAlertLayer::onBtn2(sender);
	else if (btnSelected == 1)
		FLAlertLayer::onBtn1(sender);
}
void DeltaruneAlertLayer::onBtn1(CCObject* sender) {
	if (m_fields->incompatible) {
		FLAlertLayer::onBtn1(sender);
		return;
	}
	if (!m_fields->done) {
		progressText();
		return;
	}
	blockKeys = false;
	FLAlertLayer::onBtn1(sender);
}
int DeltaruneAlertLayer::getLinesLeft() {
	auto& textArea = m_fields->textArea;
	if (!m_fields->textAreaClippingNode) return 0;
	if (!textArea) return 0;

	auto bitmapFont = static_cast<CCNode*>(textArea->getChildren()->objectAtIndex(0));
	CCArrayExt<CCLabelBMFont*> content = bitmapFont->getChildren();
	return content.size() - m_fields->linesProgressed;
}
void DeltaruneAlertLayer::show() {
	bool& showing = m_fields->showing;
	bool& incompatible = m_fields->incompatible;
	auto& titleNode = m_fields->title;

	FLAlertLayer::show();

	if (showing) return;
	showing = true;
	if (incompatible) return;

	if (!m_fields->bg) return;
	if (!titleNode) return;
	if (!m_fields->textArea) return;
	if (!m_mainLayer) return;


	auto title = std::string_view(titleNode->getString());

	if (Loader::get()->isModLoaded("user95401.geode-mod-comments") && (title == "Create Comment" || this->getID() == "finish")) {
		this->setVisible(false);
		Loader::get()->queueInMainThread([this] {
			this->setVisible(true);
			if (m_buttonMenu->getChildByID("input")) {
				m_fields->incompatible = true;
				return;
			}
			decideToBlockKeys();
			changeLook();
			});
		return;
	}
	decideToBlockKeys();
	changeLook();
}
void DeltaruneAlertLayer::setHeartPosition(CCNode* button) {
	auto& heart = m_fields->heart;
	CCNode* text = nullptr;
	for (auto node : (CCArrayExt<CCNode*>) static_cast<CCNode*>(button->getChildren()->objectAtIndex(0))->getChildren()) {
		if (auto label = typeinfo_cast<CCLabelBMFont*>(node)) {
			text = label;
			break;
		}
	}
	if (!text) return;
	heart->setPositionX(m_buttonMenu->getPositionX() + button->getPositionX() - text->getContentWidth() / 2 - heart->getContentWidth() / 2 - 5);
}
bool DeltaruneAlertLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
	if (m_fields->incompatible) return FLAlertLayer::ccTouchBegan(touch, event);

	if (!m_fields->done && !m_fields->disableClickToProgress) {
		if (m_fields->rolledPage)
			progressText();
		else
			skipText();
	}
	bool ret = FLAlertLayer::ccTouchBegan(touch, event);
	auto& btn1 = m_fields->btn1;
	auto& btn2 = m_fields->btn2;
	int& btnSelected = m_fields->btnSelected;

	if (!m_mainLayer) return ret;
	if (!m_buttonMenu) return ret;
	if (!btn1) return ret;
	if (!btn2) return ret;
	CCArrayExt<CCMenuItemSpriteExtra*> buttons = this->m_buttonMenu->getChildren();
	bool selected = false;
	for (auto button : buttons) {
		if (button->isSelected()) {
			selected = true;
		}
	}
	for (auto button : buttons) {
		auto parent = static_cast<CCNode*>(button->getChildren()->objectAtIndex(0));
		CCArrayExt<CCNode*> parts = parent->getChildren();
		for (auto part : parts) {
			if (auto label = typeinfo_cast<CCLabelBMFont*>(part)) {
				if (button->isSelected()) {
					label->setColor(ccColor3B{ 255, 255, 0 });
					if (button == btn1) {
						btnSelected = 1;
						setHeartPosition(btn1);
					}
					else if (button == btn2) {
						btnSelected = 2;
						setHeartPosition(btn2);
					}
				}
				else if (selected) {
					label->setColor(ccColor3B{ 255,255,255 });
				}
			}
		}
	}
	return ret;
}
void DeltaruneAlertLayer::keyDown(enumKeyCodes key) {
	if (m_fields->incompatible) {
		FLAlertLayer::keyDown(key);
		return;
	}
	if (key == enumKeyCodes::KEY_Z || key == enumKeyCodes::KEY_Y /*screw QWERTZ*/ || key == enumKeyCodes::CONTROLLER_A) {
		if (m_fields->rolledPage)
			progressText();
		return;
	}
	else if (key == enumKeyCodes::KEY_X || key == enumKeyCodes::KEY_Shift || key == enumKeyCodes::CONTROLLER_B) {
		skipText();
		return;
	}
	else if (key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_Left || key == enumKeyCodes::KEY_Right) {
		if (!m_mainLayer || !m_button2 || !m_fields->doneRolling) {
			FLAlertLayer::keyDown(key);
			return;
		}

		int& btnSelected = m_fields->btnSelected;
		CCArrayExt<CCNode*> partsBtn1 = m_button1->getChildren();
		for (auto part : partsBtn1) {
			if (auto label = typeinfo_cast<CCLabelBMFont*>(part)) {
				if (key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_Left) {
					btnSelected = 1;
					label->setColor(ccColor3B{ 255,255,0 });
					setHeartPosition(m_fields->btn1);
				}
				else if (key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_Right) {
					label->setColor(ccColor3B{ 255,255,255 });
				}
			}
		}
		CCArrayExt<CCNode*> partsBtn2 = m_button2->getChildren();
		for (auto part : partsBtn2) {
			if (auto label = typeinfo_cast<CCLabelBMFont*>(part)) {
				if (key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_Left) {
					label->setColor(ccColor3B{ 255,255,255 });
				}
				else if (key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_Right) {
					btnSelected = 2;
					label->setColor(ccColor3B{ 255,255,0 });
					setHeartPosition(m_fields->btn2);
				}
			}
		}
	}
	else FLAlertLayer::keyDown(key);
}
void DeltaruneAlertLayer::skipText() {
	unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
	auto& clippingNode = m_fields->textAreaClippingNode;
	int& linesProgressed = m_fields->linesProgressed;
	bool& doneRolling = m_fields->doneRolling;

	if (!clippingNode) return;
	CCArrayExt<TextArea*> textAreas = clippingNode->getChildren();
	for (auto textArea : textAreas) {
		CCArrayExt<CCLabelBMFont*> lines = static_cast<CCNode*>(textArea->getChildren()->objectAtIndex(0))->getChildren();

		for (int i = linesProgressed + m_fields->rollingLine; i < lines.size() && i < linesProgressed + 3; i++) {
			auto line = lines[i];
			CCArrayExt<CCNode*> letters = line->getChildren();
			for (auto letter : letters) {
				letter->setVisible(true);
			}
		}
	}

	m_fields->rolledPage = true;
	if (m_button2) {
		if (getLinesLeft() < 3)
			doneRolling = true;
	}
	else {
		if (getLinesLeft() <= 3)
			doneRolling = true;
	}
	if (doneRolling) showButtons();
}
// int step = 0; // funny way to get which line caused the issue on mobile
// void loggingStep() {
// 	log::info("{}", step);
// 	step++;
// }
void DeltaruneAlertLayer::progressText() {
	if (!m_mainLayer) return;
	if (!m_buttonMenu) return;
	if (!m_fields->textAreaClippingNode) return;

	auto& textArea = m_fields->textArea;
	auto& btn1 = m_fields->btn1;
	auto& btn2 = m_fields->btn2;
	auto& shadow = m_fields->shadow;
	auto& gradientOverlay = m_fields->gradientOverlay;
	int& btnSelected = m_fields->btnSelected;
	int& linesProgressed = m_fields->linesProgressed;
	bool& done = m_fields->done;
	bool& noShadow = m_fields->noShadow;

	if (!textArea) return;

	if (getLinesLeft() <= 3) {
		if (!m_button2) {
			done = true;
			FLAlertLayer::onBtn1(btn1);
			return;
		}
		else if (btnSelected != 0) {
			done = true;
			if (btnSelected == 1)
				FLAlertLayer::onBtn1(btn1);
			else if (btnSelected == 2)
				FLAlertLayer::onBtn2(btn2);
			return;
		}
	}
	if (getLinesLeft() < 3 && m_button2)
		return;

	// move EVERYTHING up
	
	int offset;
	
	m_mainLayer->getChildByID("star"_spr)->setVisible(false);
	
	if (!noShadow) m_mainLayer->getChildByID("starShadow"_spr)->setVisible(false);
	


	unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
	m_fields->characterCount = 0;
	m_fields->rollingLine = 0;


	if (getLinesLeft() > 3)
		offset = 3;
	else if (getLinesLeft() == 3)
		offset = 2;


	auto fontNode = (CCNode*) textArea->getChildren()->objectAtIndex(0);


	while (true) {
		auto topLine = (CCLabelBMFont*) fontNode->getChildren()->objectAtIndex(linesProgressed + offset);
		if (!topLine) break;
		std::string topLineString = topLine->getString();
		std::string noSpaceTopLineString = "";
		std::for_each(topLineString.begin(), topLineString.end(), [&](char c) {
			if (c != ' ') noSpaceTopLineString += c;
			});
		if (noSpaceTopLineString != "") break;
		offset++;
		m_mainLayer->getChildByID("star"_spr)->setVisible(true);
		if (!noShadow) m_mainLayer->getChildByID("starShadow"_spr)->setVisible(true);
	}


	linesProgressed += offset;
	textArea->setPositionY(textArea->getPositionY() + m_fields->textSize * offset);
	if (gradientOverlay)
		gradientOverlay->setPositionY(textArea->getPositionY());
	if (shadow)
		shadow->setPositionY(textArea->getPositionY() - 1);


	showButtons();
	float pause = Mod::get()->getSettingValue<double>("textRollingPause");
	schedule(schedule_selector(DeltaruneAlertLayer::rollText), pause / 30);
	log::info("Finished");
}
void DeltaruneAlertLayer::rollText(float dt) {
	int& waitQueue = m_fields->waitQueue;
	int& linesProgressed = m_fields->linesProgressed;
	int& characterCount = m_fields->characterCount;
	int& rollingLine = m_fields->rollingLine;
	bool& playedSound = m_fields->playedSound;
	bool& doneRolling = m_fields->doneRolling;
	bool& rolledPage = m_fields->rolledPage;

	if (waitQueue != 0) {
		waitQueue--;
		playedSound = false;
		return;
	}
	CCArrayExt<TextArea*> textAreas = m_fields->textAreaClippingNode->getChildren();
	if (rollingLine == 3) {
		unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
		rolledPage = true;
		return;
	}
	else rolledPage = false;

	bool newLine = false;
	bool playSound = true;
	for (auto textArea : textAreas) {
		CCArrayExt<CCLabelBMFont*> lines = static_cast<CCNode*>(textArea->getChildren()->objectAtIndex(0))->getChildren();
		int i = linesProgressed + rollingLine;
		if (i < lines.size() && i < linesProgressed + 3) {
			auto line = lines[i];
			CCArrayExt<CCNode*> letters = line->getChildren();
			auto letter = letters[characterCount];
			if (letter->isVisible()) {
				unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
				doneRolling = true;
				rolledPage = true;
				showButtons();
				return;
			}
			letter->setVisible(true);
			switch (line->getString()[characterCount]) {
				case ' ':
					playSound = false;
					break;
				case '.': [[fallthrough]];
				case ',': [[fallthrough]];
				case ':': [[fallthrough]];
				case ';': [[fallthrough]];
				case '?': [[fallthrough]];
				case '!':
					waitQueue = 2;
					break;
				default:
					break;
			}
			if (characterCount == line->getChildrenCount() - 1) {
				newLine = true;
			}
		}
		else {
			unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
			doneRolling = true;
			rolledPage = true;
			showButtons();
			return;
		}
	}
	characterCount++;
	if (newLine) {
		characterCount = 0;
		rollingLine++;
	}
	if (playSound) {
		if (playedSound) {
			playedSound = false;
			return;
		}
		float pitch = 1;
		playedSound = true;
		std::string sound = "";
		if (sound == "Default") sound = "SND_TXT1";
		else if (sound == "Typewriter") sound = "SND_TXT2";
		else if (sound == "Toriel") sound = "snd_txttor";
		else if (sound == "Sans") sound = "snd_txtsans";
		else if (sound == "Papyrus") sound = "snd_txtpap";
		else if (sound == "Undyne") sound = "snd_txtund";
		else if (sound == "Alphys") sound = "snd_txtal";
		else if (sound == "Asgore") sound = "snd_txtasg";
		else if (sound == "Asriel") sound = "snd_txtasr";
		else if (sound == "Susie") sound = "snd_txtsus";
		else if (sound == "Ralsei") sound = "snd_txtral";
		else if (sound == "Lancer") sound = "snd_txtlan";
		else if (sound == "Noelle") sound = "snd_txtnoe";
		else if (sound == "Berdly") sound = "snd_txtber";
		else if (sound == "Spamton") sound = "snd_txtspam";
		else if (sound == "Spamton NEO") sound = "snd_txtspam2";
		else if (sound == "Jevil") sound = "snd_txtjok";
		else if (sound == "Queen") {
			sound = "snd_txtq";
			pitch = 1 + randomNumberInAGivenRangeThatGetsAddedOrRemovedFromADifferentNumber(0.2f);
		}
		std::string path = fmt::format("{}/{}.wav", Mod::get()->getResourcesDir().string(), sound);
		auto& system = m_fields->system;
		auto& fmodSound = m_fields->sound;
		auto& channel = m_fields->channel;

		system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &fmodSound);
		system->playSound(fmodSound, nullptr, false, &channel);
		channel->setPitch(pitch);
		channel->setVolume(FMODAudioEngine::sharedEngine()->m_sfxVolume);
	}
	else playedSound = false;
}