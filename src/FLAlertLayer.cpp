#include "include.h"
#include "FLAlertLayer.h"

bool blockKeys = false;

float randomNumberInAGivenRangeThatGetsAddedOrRemovedFromADifferentNumber(float range) { // thanks cvolton for this amazing name
	return static_cast<float>(rand() % static_cast<int>(range * 100 + 1)) / 100.f - (range / 2.f);
}

bool DeltaruneAlertLayer::init(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale) {
	if (m_fields->screenSize >= 569 && !m_fields->dontRestrictWidth)
		m_fields->screenSize = 569;
	std::srand(std::time(NULL));
	width = m_fields->screenSize;
	m_fields->text = desc;
	height = 140;
	scroll = false;
	textScale = 1;

	if (!FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale)) return false;
	this->m_noElasticity = true;

	NodeIDs::provideFor(this);
	setID("FLAlertLayer");
	m_fields->mainLayer = getChildByID("main-layer");
	if (m_fields->mainLayer) {
		if (this->m_buttonMenu) {
			m_fields->btn1 = m_buttonMenu->getChildByID("button-1");
			m_fields->btn2 = m_buttonMenu->getChildByID("button-2");
		}
		m_fields->textArea = m_fields->mainLayer->getChildByID("content-text-area");
		m_fields->bg = m_fields->mainLayer->getChildByID("background");
		m_fields->title = static_cast<CCLabelBMFont*>(m_fields->mainLayer->getChildByID("title"));
	}
	return true;
}
void DeltaruneAlertLayer::showButtons() {
	if (m_fields->btn2 && getLinesLeft() < 3 && m_fields->doneRolling) {
		m_fields->done = true;
		this->m_buttonMenu->setVisible(true);
	}
}
void DeltaruneAlertLayer::onBtn2(CCObject* sender) {
	if (!m_fields->done) {
		progressText();
		return;
	}
	if (m_fields->btnSelected == 0) return;
	if (m_fields->btnSelected == 2)
		FLAlertLayer::onBtn2(sender);
	else if (m_fields->btnSelected == 1)
		FLAlertLayer::onBtn1(sender);
}
void DeltaruneAlertLayer::onBtn1(CCObject* sender) {
	if (!m_fields->done) {
		progressText();
		return;
	}
	blockKeys = false;
	FLAlertLayer::onBtn1(sender);
}
int DeltaruneAlertLayer::getLinesLeft() {
	if (!m_fields->textAreaClippingNode) return 0;
	if (!m_fields->textArea) return 0;
	CCArrayExt<CCLabelBMFont*> content = static_cast<CCNode*>(m_fields->textArea->getChildren()->objectAtIndex(0))->getChildren();
	return content.size() - m_fields->linesProgressed;
}
void DeltaruneAlertLayer::show() {
	FLAlertLayer::show();
	int numOfSiblings = 0;
	if (auto parent = getParent()) {
		CCArrayExt<CCNode*> siblings = parent->getChildren();

		for (auto sibling : siblings) {
			if (sibling->getID() == "FLAlertLayer")
				numOfSiblings++;
		}
	}
	if (numOfSiblings >= 1 && !m_fields->btn2)
		blockKeys = true;
	else
		blockKeys = false;
	changeLook();
}
bool DeltaruneAlertLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
	if (!m_fields->done && !m_fields->disableClickToProgress) {
		if (m_fields->rolledPage)
			progressText();
		else
			skipText();
	}
	bool ret = FLAlertLayer::ccTouchBegan(touch, event);
	if (!m_fields->mainLayer) return ret;
	if (!this->m_buttonMenu) return ret;
	CCArrayExt<CCMenuItemSpriteExtra*> buttons = m_buttonMenu->getChildren();
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
					addHeart(parent, label);
					if (button->getID() == "button-1")
						m_fields->btnSelected = 1;
					else if (button->getID() == "button-2")
						m_fields->btnSelected = 2;
				}
				else if (selected) {
					label->setColor(ccColor3B{ 255,255,255 });
					auto heart = parent->getChildByID("heart");
					if (heart)
						heart->removeFromParentAndCleanup(true);
				}
			}
		}
	}
	return ret;
}
void DeltaruneAlertLayer::keyDown(enumKeyCodes key) {
	if (key == enumKeyCodes::KEY_Z || key == enumKeyCodes::KEY_Y /*screw QWERTZ*/ || key == enumKeyCodes::CONTROLLER_A) {
		if (m_fields->rolledPage)
			progressText();
		return;
	}
	else if (key == enumKeyCodes::KEY_X || key == enumKeyCodes::KEY_Shift || key == enumKeyCodes::CONTROLLER_B) {
		skipText();
		return;
	}
	// else if (key == enumKeyCodes::KEY_G) {
	// 	CCSprite* e = nullptr;
	// 	static_cast<CCSprite*>(e->getChildByID("crashmepls"))->getTexture();
	// 	return;
	// }
	else if (key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_Left || key == enumKeyCodes::KEY_Right) {
		if (!m_fields->mainLayer || !m_fields->btn2 || !m_fields->doneRolling) {
			FLAlertLayer::keyDown(key);
			return;
		}

		auto parent = static_cast<CCNode*>(m_fields->btn1->getChildren()->objectAtIndex(0));
		CCArrayExt<CCNode*> partsBtn1 = parent->getChildren();
		for (auto part : partsBtn1) {
			if (auto label = typeinfo_cast<CCLabelBMFont*>(part)) {
				if (key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_Left) {
					m_fields->btnSelected = 1;
					addHeart(parent, label);
					label->setColor(ccColor3B{ 255,255,0 });
				}
				else if (key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_Right) {
					label->setColor(ccColor3B{ 255,255,255 });
					auto heart = parent->getChildByID("heart");
					if (heart)
						heart->removeFromParentAndCleanup(true);
				}
			}
		}
		parent = static_cast<CCNode*>(m_fields->btn2->getChildren()->objectAtIndex(0));
		CCArrayExt<CCNode*> partsBtn2 = parent->getChildren();
		for (auto part : partsBtn2) {
			if (auto label = typeinfo_cast<CCLabelBMFont*>(part)) {
				if (key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_Left) {
					label->setColor(ccColor3B{ 255,255,255 });
					auto heart = parent->getChildByID("heart");
					if (heart)
						heart->removeFromParentAndCleanup(true);
				}
				else if (key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_Right) {
					m_fields->btnSelected = 2;
					addHeart(parent, label);
					label->setColor(ccColor3B{ 255,255,0 });
				}
			}
		}
	}
	else FLAlertLayer::keyDown(key);
}
void DeltaruneAlertLayer::skipText() {
	unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
	if (!m_fields->textAreaClippingNode) return;
	CCArrayExt<TextArea*> textAreas = m_fields->textAreaClippingNode->getChildren();
	for (auto textArea : textAreas) {
		CCArrayExt<CCLabelBMFont*> lines = static_cast<CCNode*>(textArea->getChildren()->objectAtIndex(0))->getChildren();

		for (int i = m_fields->linesProgressed + m_fields->rollingLine; i < lines.size() && i < m_fields->linesProgressed + 3; i++) {
			auto line = lines[i];
			CCArrayExt<CCNode*> letters = line->getChildren();
			for (auto letter : letters) {
				letter->setVisible(true);
			}
		}
	}

	m_fields->rolledPage = true;
	if (m_fields->btn2) {
		if (getLinesLeft() < 3)
			m_fields->doneRolling = true;
	}
	else {
		if (getLinesLeft() <= 3)
			m_fields->doneRolling = true;
	}
	if (m_fields->doneRolling) showButtons();
}
void DeltaruneAlertLayer::progressText() {
	if (!m_fields->mainLayer) return;
	if (!m_buttonMenu) return;
	if (!m_fields->textAreaClippingNode) return;
	if (!m_fields->textArea) return;

	if (getLinesLeft() <= 3) {
		if (!m_fields->btn2) {
			m_fields->done = true;
			FLAlertLayer::onBtn1(m_fields->btn1);
			return;
		}
		else if (m_fields->btnSelected != 0) {
			m_fields->done = true;
			if (m_fields->btnSelected == 1)
				FLAlertLayer::onBtn1(m_fields->btn1);
			else if (m_fields->btnSelected == 2)
				FLAlertLayer::onBtn2(m_fields->btn2);
			return;
		}
	}
	if (getLinesLeft() < 3 && m_fields->btn2)
		return;

	// move EVERYTHING up
	int offset;
	if (m_fields->dialog)
		m_fields->mainLayer->getChildByID("star"_spr)->setVisible(false);

	unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
	m_fields->characterCount = 0;
	m_fields->rollingLine = 0;

	if (getLinesLeft() > 3)
		offset = 3;
	else if (getLinesLeft() == 3)
		offset = 2;
	m_fields->linesProgressed += offset;
	m_fields->textArea->setPositionY(m_fields->textArea->getPositionY() + m_fields->textSize * offset);
	if (m_fields->gradientOverlay)
		m_fields->gradientOverlay->setPositionY(m_fields->textArea->getPositionY());

	if (m_fields->btn2 && getLinesLeft() < 3) {
		m_fields->done = true;
		this->m_buttonMenu->setVisible(true);
	}
	float pause = Mod::get()->getSettingValue<double>("textRollingPause");
	schedule(schedule_selector(DeltaruneAlertLayer::rollText), pause / 30);
}
void DeltaruneAlertLayer::rollText(float dt) {
	if (m_fields->waitQueue != 0) {
		m_fields->waitQueue--;
		m_fields->playedSound = false;
		return;
	}
	CCArrayExt<TextArea*> textAreas = m_fields->textAreaClippingNode->getChildren();
	if (m_fields->rollingLine == 3) {
		unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
		m_fields->rolledPage = true;
		return;
	}
	else
		m_fields->rolledPage = false;
	bool newLine = false;
	bool playSound = true;
	for (auto textArea : textAreas) {
		CCArrayExt<CCLabelBMFont*> lines = static_cast<CCNode*>(textArea->getChildren()->objectAtIndex(0))->getChildren();
		int i = m_fields->linesProgressed + m_fields->rollingLine;
		if (i < lines.size() && i < m_fields->linesProgressed + 3) {
			auto line = lines[i];
			CCArrayExt<CCNode*> letters = line->getChildren();
			auto letter = letters[m_fields->characterCount];
			if (letter->isVisible()) {
				unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
				m_fields->doneRolling = true;
				m_fields->rolledPage = true;
				showButtons();
				return;
			}
			letter->setVisible(true);
			switch (line->getString()[m_fields->characterCount]) {
				case ' ':
					playSound = false;
					break;
				case '.': [[fallthrough]];
				case ',': [[fallthrough]];
				case ':': [[fallthrough]];
				case ';': [[fallthrough]];
				case '?': [[fallthrough]];
				case '!':
					m_fields->waitQueue = 2;
					break;
				default:
					break;
			}
			if (m_fields->characterCount == line->getChildrenCount() - 1) {
				newLine = true;
			}
		}
		else {
			unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
			m_fields->doneRolling = true;
			m_fields->rolledPage = true;
			showButtons();
			return;
		}
	}
	m_fields->characterCount++;
	if (newLine) {
		m_fields->characterCount = 0;
		m_fields->rollingLine++;
	}
	if (playSound) {
		if (m_fields->playedSound) {
			m_fields->playedSound = false;
			return;
		}
		float pitch = 1;
		m_fields->playedSound = true;
		std::string sound = "";
		auto character = m_fields->textSound;
		if (character == "Default") sound = "SND_TXT1";
		else if (character == "Typewriter") sound = "SND_TXT2";
		else if (character == "Toriel") sound = "snd_txttor";
		else if (character == "Sans") sound = "snd_txtsans";
		else if (character == "Papyrus") sound = "snd_txtpap";
		else if (character == "Undyne") sound = "snd_txtund";
		else if (character == "Alphys") sound = "snd_txtal";
		else if (character == "Asgore") sound = "snd_txtasg";
		else if (character == "Asriel") sound = "snd_txtasr";
		else if (character == "Susie") sound = "snd_txtsus";
		else if (character == "Ralsei") sound = "snd_txtral";
		else if (character == "Lancer") sound = "snd_txtlan";
		else if (character == "Noelle") sound = "snd_txtnoe";
		else if (character == "Berdly") sound = "snd_txtber";
		else if (character == "Spamton") sound = "snd_txtspam";
		else if (character == "Spamton NEO") sound = "snd_txtspam2";
		else if (character == "Jevil") sound = "snd_txtjok";
		else if (character == "Queen") {
			sound = "snd_txtq";
			pitch = 1 + randomNumberInAGivenRangeThatGetsAddedOrRemovedFromADifferentNumber(0.2f);
		}
		std::string path = fmt::format("{}/{}.wav", Mod::get()->getResourcesDir().string(), sound);
		m_fields->system->createSound(path.c_str(), FMOD_DEFAULT, nullptr, &(m_fields->sound));
		m_fields->system->playSound(m_fields->sound, nullptr, false, &(m_fields->channel));
		m_fields->channel->setPitch(pitch);
		m_fields->channel->setVolume(FMODAudioEngine::sharedEngine()->m_sfxVolume);
	}
	else m_fields->playedSound = false;
}