#include "FLAlertLayer.h"
#include "DialogLayer.h"

bool blockKeys = false;

float randomNumberInAGivenRangeThatGetsAddedOrRemovedFromADifferentNumber(float range) { // thanks cvolton for this amazing name
	return static_cast<float>(rand() % static_cast<int>(range * 100 + 1)) / 100.f - (range / 2.f);
}

void DeltaruneAlertLayer::initMaps() {
	auto& nameToFile = m_fields->nameToFile;
	auto& nameToSound = m_fields->nameToSound;

	nameToFile["Default"] = "SND_TXT1";
	nameToFile["Typewriter"] = "SND_TXT2";
	nameToFile["Toriel"] = "snd_txttor";
	nameToFile["Sans"] = "snd_txtsans";
	nameToFile["Papyrus"] = "snd_txtpap";
	nameToFile["Undyne"] = "snd_txtund";
	nameToFile["Alphys"] = "snd_txtal";
	nameToFile["Asgore"] = "snd_txtasg";
	nameToFile["Asriel"] = "snd_txtasr";
	nameToFile["Susie"] = "snd_txtsus";
	nameToFile["Ralsei"] = "snd_txtral";
	nameToFile["Lancer"] = "snd_txtlan";
	nameToFile["Noelle"] = "snd_txtnoe";
	nameToFile["Berdly"] = "snd_txtber";
	nameToFile["Spamton"] = "snd_txtspam";
	nameToFile["Spamton NEO"] = "snd_txtspam2";
	nameToFile["Jevil"] = "snd_txtjok";
	nameToFile["Queen"] = "snd_txtq";

	nameToSound["The Mechanic"] = "Alphys";
	nameToSound["The Shopkeeper"] = "Spamton";
	nameToSound["Scratch"] = "Lancer";
	nameToSound["Potbor"] = "Spamton NEO";
	nameToSound["Diamond Shopkeeper"] = "Papyrus";
	nameToSound["The Keymaster"] = "Susie";

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
	if (Loader::get()->isModLoaded("firee.prism")) { // probably the easiest way to detect a Prism Menu alert :3
		if (desc == "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA") {
			m_fields->incompatible = true;
			return true;
		}
	}


	auto& textArea = m_fields->textArea;
	auto& bg = m_fields->bg;
	auto& titleNode = m_fields->title;

	initMaps(); // for sounds

	this->m_noElasticity = true;

	m_fields->btn1 = m_button1 ? static_cast<CCMenuItemSpriteExtra*>(m_button1->getParent()) : nullptr;
	m_fields->btn2 = m_button2 ? static_cast<CCMenuItemSpriteExtra*>(m_button2->getParent()) : nullptr;

	textArea = m_mainLayer->getChildByID("content-text-area");
	bg = m_mainLayer->getChildByID("background");
	titleNode = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("title"));
#if !(defined(GEODE_IS_MACOS) || defined(DEBUG_MAC_INPUTS) || defined(GEODE_IS_ANDROID32))
	initCustomKeybinds();
#endif
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
	bool dialog = m_fields->dialog;
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
	if (btnSelected == 2) FLAlertLayer::onBtn2(sender);
	else if (btnSelected == 1) FLAlertLayer::onBtn1(sender);
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

void DeltaruneAlertLayer::clickedOnButton(CCMenuItemSpriteExtra* btn, ButtonSprite* buttonSprite, int btnSelected) {
	auto label = getChildOfType<CCLabelBMFont>(buttonSprite, 0);
	if (!label) {
		// log::info("Where the fuck is the label?");
		return;
	}
	if (btn->isSelected()) {
		label->setColor(ccColor3B{ 255, 255, 0 });
		m_fields->btnSelected = btnSelected;
		setHeartPosition(btn);
	}
	else label->setColor(ccColor3B{ 255,255,255 });
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

	if (!btn1) {
		// log::info("No btn1");
		return ret;
	}
	if (!btn2) {
		// log::info("No btn2");
		return ret;
	}

	if (!btn1->isSelected() && !btn2->isSelected()) {
		// log::info("None of the buttons are clicked");
		return ret;
	}
	// log::info("No problems in ccTouchBegan!");
	clickedOnButton(btn1, m_button1, 1);
	clickedOnButton(btn2, m_button2, 2);
	return ret;
}
#if (defined(GEODE_IS_MACOS) || defined(DEBUG_MAC_INPUTS) || defined(GEODE_IS_ANDROID32))
void DeltaruneAlertLayer::keyDown(enumKeyCodes key) {
	if (m_fields->incompatible) {
		FLAlertLayer::keyDown(key);
		return;
	}
	if (key == KEY_Z || key == KEY_Y /*screw QWERTZ*/) {
		if (m_fields->rolledPage)
			progressText();
		return;
	}
	else if (key == KEY_X || key == KEY_Space) {
		skipText();
		return;
	}
	else if (key == KEY_ArrowLeft || key == KEY_ArrowRight || key == KEY_Left || key == KEY_Right) {
		if (!m_mainLayer || !m_button2 || !m_fields->doneRolling) {
			FLAlertLayer::keyDown(key);
			return;
		}

		int& btnSelected = m_fields->btnSelected;
		auto label1 = getChildOfType<CCLabelBMFont>(m_button1, 0);
		auto label2 = getChildOfType<CCLabelBMFont>(m_button2, 0);
		if (key == KEY_ArrowLeft || key == KEY_Left) {
			btnSelected = 1;
			label1->setColor(ccColor3B{ 255,255,0 });
			setHeartPosition(m_fields->btn1);
			label2->setColor(ccColor3B{ 255,255,255 });
		}
		else if (key == KEY_ArrowRight || key == KEY_Right) {
			btnSelected = 2;
			label2->setColor(ccColor3B{ 255,255,0 });
			setHeartPosition(m_fields->btn2);
			label1->setColor(ccColor3B{ 255,255,255 });
		}
	}
	else FLAlertLayer::keyDown(key);
}
#else
void DeltaruneAlertLayer::initCustomKeybinds() {
	if (m_fields->incompatible) return;

	this->template addEventListener<keybinds::InvokeBindFilter>([=, this](keybinds::InvokeBindEvent* event) {
		if (event->isDown()) {
			if (!m_mainLayer || !m_button2 || !m_fields->doneRolling) {
				return ListenerResult::Propagate;
			}

			int& btnSelected = m_fields->btnSelected;
			btnSelected = 1;
			auto label1 = getChildOfType<CCLabelBMFont>(m_button1, 0);
			auto label2 = getChildOfType<CCLabelBMFont>(m_button2, 0);
			label1->setColor(ccColor3B{ 255,255,0 });
			setHeartPosition(m_fields->btn1);
			label2->setColor(ccColor3B{ 255,255,255 });
			return ListenerResult::Stop;
		}
		return ListenerResult::Propagate;
		}, "left"_spr);
	this->template addEventListener<keybinds::InvokeBindFilter>([=, this](keybinds::InvokeBindEvent* event) {
		if (event->isDown()) {
			if (!m_mainLayer || !m_button2 || !m_fields->doneRolling) {
				return ListenerResult::Propagate;
			}

			int& btnSelected = m_fields->btnSelected;
			btnSelected = 2;
			auto label1 = getChildOfType<CCLabelBMFont>(m_button1, 0);
			auto label2 = getChildOfType<CCLabelBMFont>(m_button2, 0);
			label2->setColor(ccColor3B{ 255,255,0 });
			setHeartPosition(m_fields->btn2);
			label1->setColor(ccColor3B{ 255,255,255 });
			return ListenerResult::Stop;
		}
		return ListenerResult::Propagate;
		}, "right"_spr);
	this->template addEventListener<keybinds::InvokeBindFilter>([=, this](keybinds::InvokeBindEvent* event) {
		if (event->isDown()) {
			if (m_fields->rolledPage) {
				progressText();
				return ListenerResult::Stop;
			}
		}
		return ListenerResult::Propagate;
		}, "progress"_spr);
	this->template addEventListener<keybinds::InvokeBindFilter>([=, this](keybinds::InvokeBindEvent* event) {
		if (event->isDown()) {
			skipText();
			return ListenerResult::Stop;
		}
		return ListenerResult::Propagate;
		}, "skip"_spr);
}

#endif
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

int DeltaruneAlertLayer::emptyLinesAmount(int offset) {
	auto& textArea = m_fields->textArea;
	auto& linesProgressed = m_fields->linesProgressed;
	auto fontNode = (CCNode*) textArea->getChildren()->objectAtIndex(0);
	int lines = 0;
	while (true) {
		if (linesProgressed + lines + offset >= fontNode->getChildrenCount()) break;

		auto topLineNode = fontNode->getChildren()->objectAtIndex(linesProgressed + lines + offset);
		auto topLine = static_cast<CCLabelBMFont*>(topLineNode);
		if (!topLineNode) break;
		if (!topLine) break;

		std::string topLineString = topLine->getString();
		bool empty = std::ranges::all_of(topLineString, [](unsigned char c) {
			return std::isspace(c);
			});
		if (!empty) break;
		
		lines++;

		auto star = m_mainLayer->getChildByID("star"_spr);
		auto starShadow = m_mainLayer->getChildByID("starShadow"_spr);

		if (star) star->setVisible(true);
		if (starShadow) starShadow->setVisible(true);
	}
	return lines;
}

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

	if (getLinesLeft() - emptyLinesAmount(3) <= 3) {
		if (!m_button2) {
			auto dialogLayer = m_fields->dialogLayer;
			done = true;
			if (m_fields->dialog && dialogLayer) {
				dialogLayer->onClose();
			}
			FLAlertLayer::onBtn1(btn1);
			return;
		}
		else if (btnSelected != 0) {
			done = true;
			if (btnSelected == 1) FLAlertLayer::onBtn1(btn1);
			else if (btnSelected == 2) FLAlertLayer::onBtn2(btn2);
			return;
		}
	}
	// Don't progress if there's only a choice left!
	if (getLinesLeft() < 3 && m_button2)
		return;

	// Move EVERYTHING up

	int offset;
	m_mainLayer->getChildByID("star"_spr)->setVisible(false);
	if (!noShadow) m_mainLayer->getChildByID("starShadow"_spr)->setVisible(false);

	unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
	m_fields->characterCount = 0;
	m_fields->rollingLine = 0;

	if (getLinesLeft() > 3)
		offset = 3;
	else if (getLinesLeft() == 3)
		offset = 1;

	int emptyLines = emptyLinesAmount(offset);
	offset += emptyLines;

	auto& characters = m_fields->characterSpriteNames;
	int& dialogCount = m_fields->dialogCount;
	bool progressDialog = emptyLines > 0 && characters.size() > 1;
	if (progressDialog) {
		dialogCount++;
		auto& spriteName = characters[dialogCount];
		auto prevChar = (CCSpriteGrayscale*) m_mainLayer->getChildByID("character-sprite"_spr);
		auto newChar = CCSpriteGrayscale::create(spriteName);
		newChar->setPosition(prevChar->getPosition());
		newChar->setZOrder(prevChar->getZOrder());
		m_fields->characterSprite = newChar;
		newChar->setID("character-sprite"_spr);
		m_mainLayer->addChild(newChar);

		auto title = m_fields->titles[dialogCount];
		auto& nameToSound = m_fields->nameToSound;
		m_fields->title->setString(title.c_str());
		m_mainLayer->removeChildByID("character-sprite"_spr);
		if (nameToSound.find(title.c_str()) != nameToSound.end())
			m_fields->textSound = nameToSound[title.c_str()];
		else
			m_fields->textSound = "Default";
	}

	linesProgressed += offset;
	textArea->setPositionY(textArea->getPositionY() + m_fields->textSize * offset);
	if (gradientOverlay) gradientOverlay->setPositionY(textArea->getPositionY());
	if (shadow) shadow->setPositionY(textArea->getPositionY() - 1);

	showButtons();
	float pause = Mod::get()->getSettingValue<double>("textRollingPause");
	schedule(schedule_selector(DeltaruneAlertLayer::rollText), pause / 30);
}

void DeltaruneAlertLayer::rollText(float dt) {
	int& waitQueue = m_fields->waitQueue;
	int& linesProgressed = m_fields->linesProgressed;
	int& characterCount = m_fields->characterCount;
	int& rollingLine = m_fields->rollingLine;
	bool& playedSound = m_fields->playedSound;
	bool& doneRolling = m_fields->doneRolling;
	bool& rolledPage = m_fields->rolledPage;
	float& lostTime = m_fields->lostTime;
	float const pause = Mod::get()->getSettingValue<double>("textRollingPause") / 30;

	if (dt - pause > pause)
		lostTime += dt - pause;

	bool playSound = true;

	for (bool firstRun = true; lostTime >= pause || firstRun;) {
		bool newLine = false;
		firstRun = false;
		if (waitQueue != 0) {
			waitQueue--;
			playedSound = false;
			if (lostTime >= pause && !firstRun) {
				lostTime -= pause;
			}
			continue;
		}
		CCArrayExt<TextArea*> textAreas = m_fields->textAreaClippingNode->getChildren();
		if (rollingLine == 3) {
			unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
			rolledPage = true;
			return;
		}
		else rolledPage = false;

		for (auto textArea : textAreas) {
			CCArrayExt<CCLabelBMFont*> lines = static_cast<CCNode*>(textArea->getChildren()->objectAtIndex(0))->getChildren();
			int currentLine = linesProgressed + rollingLine;
			if (currentLine < lines.size() && currentLine < linesProgressed + 3) {
				auto line = lines[currentLine];
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
		if (lostTime >= pause && !firstRun) {
			lostTime -= pause;
		}
	}
	auto nameToFile = m_fields->nameToFile;
	std::string const textSound = m_fields->textSound;
	std::string const resFolder = Mod::get()->getResourcesDir().string();
	std::string path = fmt::format("{}/{}.wav", resFolder, nameToFile[textSound]);

	if (nameToFile.find(textSound) == nameToFile.end()) return;

	if (!playSound || playedSound) {
		playedSound = false;
		return;
	}
	float pitch = 1;
	playedSound = true;

	auto& system = m_fields->system;
	auto& channel = m_fields->channel;
	auto& sound = m_fields->sound;

	if (textSound == "Queen")
		pitch = 1 + randomNumberInAGivenRangeThatGetsAddedOrRemovedFromADifferentNumber(0.2f);

	system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &sound);
	system->playSound(sound, nullptr, false, &channel);
	channel->setPitch(pitch);
	channel->setVolume(FMODAudioEngine::sharedEngine()->m_sfxVolume);
}