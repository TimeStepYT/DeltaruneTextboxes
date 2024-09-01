#include "include.h"
#include "FLAlertLayer.h"

void capitalize(std::string& str) {
	bool disable = false;
	for (char& c : str) {
		char next = *(&c + 1);
		if (c == '<') {
			if (next == 'c' || next == 'd' || next == '/') {
				disable = true;
				continue;
			}
		}
		else if (c == '>') {
			disable = false;
			continue;
		}
		if (disable) continue;
		c = toupper(c);
	}
}

void DeltaruneAlertLayer::changeBG() {
	m_fields->bg->removeFromParent();

	m_fields->bg = CCScale9Sprite::create("deltaruneSquare.png"_spr);
	m_fields->bg->setContentHeight(140);
	m_fields->bg->setContentWidth(m_fields->screenSize);
	m_fields->bg->setPosition(CCPoint{ CCDirector::sharedDirector()->getWinSize().width / 2, 70 });
	m_fields->bg->setID("background");
	if (m_fields->dialog) {
		m_fields->characterSprite->setZOrder(m_fields->bg->getZOrder() + 1);
		m_fields->characterSprite->setPosition({ m_fields->bg->getPositionX() - m_fields->screenSize / 2 + 65, m_fields->bg->getPositionY() });

		m_fields->characterSprite->setID("character-sprite"_spr);
		m_fields->mainLayer->addChild(m_fields->characterSprite);
	}
	m_fields->mainLayer->addChild(m_fields->bg);
}

void DeltaruneAlertLayer::changeButtons() {
	if (!this->m_buttonMenu) return;
	m_buttonMenu->setPositionY(32);
	m_buttonMenu->setVisible(false);

	if (!m_fields->btn2) return;

	m_fields->btn1->setPositionX(m_fields->bg->getPositionX() - m_buttonMenu->getPositionX() - m_fields->screenSize / 2 + m_fields->screenSize / 4);
	m_fields->btn2->setPositionX(m_fields->bg->getPositionX() - m_buttonMenu->getPositionX() - m_fields->screenSize / 2 + (m_fields->screenSize / 4) * 3);

	CCArrayExt<CCNode*> buttons = m_buttonMenu->getChildren();

	auto heart = CCSprite::create("heart.png"_spr);
	heart->setVisible(false);
	heart->setPositionY(this->m_buttonMenu->getPositionY());
	heart->setPositionX(this->m_buttonMenu->getPositionX());
	heart->setID("heart"_spr);
	m_fields->mainLayer->addChild(heart);
	m_fields->heart = heart;

	for (auto button : buttons) {
		if (button != typeinfo_cast<CCMenuItemSpriteExtra*>(button)) continue;
		static_cast<CCMenuItemSpriteExtra*>(button)->m_animationEnabled = false;
		CCArrayExt<CCNode*> parts = static_cast<CCNode*>(button->getChildren()->objectAtIndex(0))->getChildren();
		for (auto part : parts) {
			if (auto buttonTexture = typeinfo_cast<CCScale9Sprite*>(part)) {
				buttonTexture->setVisible(false);
			}
			else if (auto label = typeinfo_cast<CCLabelBMFont*>(part)) {
				label->setFntFile("Determination.fnt"_spr);
				label->setScale(1);
			}
		}
	}
}
void DeltaruneAlertLayer::changeTitle() {
	m_fields->title->setAnchorPoint(CCPoint{ 0, 0.5 });
	static_cast<CCLabelBMFont*>(m_fields->title)->setFntFile("Determination.fnt"_spr);
	m_fields->title->setPosition(CCPoint{ m_fields->bg->getPositionX() - m_fields->bg->getContentWidth() / 2 + 24, 145 });
}
void DeltaruneAlertLayer::changeText() {
	if (!m_fields->textArea) return;
	m_fields->textArea->removeFromParent();
	auto star = CCLabelBMFont::create("*", "Determination.fnt"_spr);
	auto str = m_fields->text;
	auto screenSize = m_fields->screenSize;
	auto titleString = std::string_view(m_fields->title->getString());
	auto bg = m_fields->bg;
	int xOffset = star->getContentWidth();
	if (m_fields->dialog) {
		xOffset = m_fields->characterSprite->getContentWidth() + 27 + star->getContentWidth();
		if (titleString == "The Mechanic") m_fields->textSound = "Alphys";
		else if (titleString == "Scratch") m_fields->textSound = "Lancer";
		else if (titleString == "The Shopkeeper") m_fields->textSound = "Spamton";
		else if (titleString == "Potbor") m_fields->textSound = "Spamton NEO";
		else if (titleString == "Diamond Shopkeeper") m_fields->textSound = "Papyrus";
		else if (titleString == "The Keymaster") m_fields->textSound = "Susie";
	}
	star->setPositionX(m_fields->bg->getPositionX() - screenSize / 2 + xOffset - star->getContentWidth() + 27);
	star->setPositionY(110);
	star->setID("star"_spr);
	auto sound = m_fields->textSound;
	std::string font = "Determination.fnt"_spr;
	if (sound == "Sans") font = "ComicSans.fnt"_spr;
	if (sound == "Papyrus") {
		font = "Papyrus.fnt"_spr;
		capitalize(str);
	}
	auto newDesc = TextArea::create(
		str,
		font.c_str(),
		1,
		screenSize - 100 - xOffset,
		CCPoint{ 0, 1 },
		m_fields->textSize,
		false
	);

	newDesc->setContentWidth(screenSize);
	newDesc->setAnchorPoint(CCPoint{ 0, 1 });
	newDesc->setPositionY(110);
	newDesc->setZOrder(m_fields->textArea->getZOrder());
	newDesc->setID("content-text-area");

	TextArea* newDescGrad = nullptr;
	if (!Mod::get()->getSettingValue<bool>("noGradientOverlay") && sound != "Sans" && sound != "Papyrus") {
		newDescGrad = TextArea::create(
			m_fields->text,
			"DeterminationGradient.fnt"_spr,
			1,
			screenSize - 100 - xOffset,
			CCPoint{ 0, 1 },
			m_fields->textSize,
			false
		);
		newDescGrad->setContentWidth(screenSize);
		newDescGrad->setAnchorPoint(CCPoint{ 0, 1 });
		newDescGrad->setPositionY(110);
		newDescGrad->setZOrder(m_fields->textArea->getZOrder() + 1);
		newDescGrad->setID("gradient-overlay"_spr);

		CCArrayExt<CCLabelBMFont*> linesGrad = static_cast<CCNode*>(newDescGrad->getChildren()->objectAtIndex(0))->getChildren();
		for (auto line : linesGrad) {
			CCArrayExt<CCSprite*> letters = line->getChildren();
			for (auto letter : letters) {
				letter->setColor(ccColor3B{ 255, 255, 255 });
				letter->setVisible(false);
			}
		}
	}
	CCArrayExt<CCLabelBMFont*> lines = static_cast<CCNode*>(newDesc->getChildren()->objectAtIndex(0))->getChildren();

	for (auto line : lines) {
		CCArrayExt<CCSprite*> letters = line->getChildren();
		for (auto letter : letters) {
			letter->setVisible(false);
		}
	}
	m_fields->mainLayer->addChild(star);

	auto rect = CCLayerColor::create({ 0,0,0,0 }, bg->getContentWidth(), bg->getContentHeight() - 20);
	auto clippingNode = CCClippingNode::create(rect);
	clippingNode->setID("content-text-area"_spr);
	clippingNode->setPositionY(10);
	clippingNode->setPositionX(bg->getPositionX() - screenSize / 2 + 24 + xOffset);
	clippingNode->addChild(newDesc);
	if (newDescGrad)
		clippingNode->addChild(newDescGrad);
	m_fields->mainLayer->addChild(clippingNode);
	m_fields->textAreaClippingNode = clippingNode;
	m_fields->textArea = newDesc;
	m_fields->gradientOverlay = newDescGrad;
	float pause = Mod::get()->getSettingValue<double>("textRollingPause");
	schedule(schedule_selector(DeltaruneAlertLayer::rollText), pause / 30);
}
void DeltaruneAlertLayer::changeLook() {
	changeBG();
	changeButtons();
	changeTitle();
	changeText();
}