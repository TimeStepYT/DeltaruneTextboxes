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

void DeltaruneAlertLayer::animateBG(float dt) {
	auto bg = m_fields->bg;
	int& frame = m_fields->frame;
	bool& reverseAnim = m_fields->reverseAnim;

	m_mainLayer->getChildByIDRecursive("background")->removeFromParentAndCleanup(true);
	bg = CCScale9Sprite::create(fmt::format("deltaruneSquare_{}.png"_spr, frame).c_str());
	bg->setContentHeight(140);
	bg->setContentWidth(m_fields->screenSize);
	bg->setPosition(CCPoint{ CCDirector::sharedDirector()->getWinSize().width / 2, 70 });
	bg->setID("background");
	bg->setZOrder(-1);

	m_mainLayer->addChild(bg);

	if (reverseAnim) frame--;
	else frame++;
	if (frame == 0 || frame == 4)
		reverseAnim = !reverseAnim;
}

void DeltaruneAlertLayer::changeBG() {
	auto& bg = m_fields->bg;
	auto const screenSize = m_fields->screenSize;
	auto& character = m_fields->characterSprite;
	bg->removeFromParentAndCleanup(true);
	bg = CCScale9Sprite::create("deltaruneSquare_0.png"_spr);
	bg->setContentHeight(140);
	bg->setContentWidth(screenSize);
	bg->setPosition(CCPoint{ CCDirector::sharedDirector()->getWinSize().width / 2, 70 });
	bg->setID("background");
	bg->setZOrder(-1);
	schedule(schedule_selector(DeltaruneAlertLayer::animateBG), 1 / 5.f);

	if (m_fields->dialog) {
		character->setZOrder(bg->getZOrder() + 1);
		character->setPosition({ bg->getPositionX() - screenSize / 2 + 65, bg->getPositionY() });

		character->setID("character-sprite"_spr);
		m_mainLayer->addChild(character);
	}
	m_mainLayer->addChild(bg);
}

void DeltaruneAlertLayer::changeButtons() {
	if (!m_buttonMenu) return;
	m_buttonMenu->setPositionY(32);
	m_buttonMenu->setVisible(false);

	if (!m_button2) return;
	auto& bg = m_fields->bg;
	float const positionStart = bg->getPositionX() - m_buttonMenu->getPositionX() - m_fields->screenSize / 2;
	m_fields->btn1->setPositionX(positionStart + m_fields->screenSize / 4);
	m_fields->btn2->setPositionX(positionStart + (m_fields->screenSize / 4) * 3);

	CCArrayExt<CCNode*> buttons = m_buttonMenu->getChildren();

	auto heart = CCSprite::create("heart.png"_spr);
	heart->setVisible(false);
	heart->setPositionY(m_buttonMenu->getPositionY());
	heart->setPositionX(m_buttonMenu->getPositionX());
	heart->setID("heart"_spr);
	m_mainLayer->addChild(heart);
	m_fields->heart = heart;

	for (auto button : buttons) {
		auto confirmedButton = typeinfo_cast<CCMenuItemSpriteExtra*>(button);
		if (button != confirmedButton) continue;
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
	CCLabelBMFont*& title = m_fields->title;
	auto& bg = m_fields->bg;
	title->setAnchorPoint(CCPoint{ 0, 0.5 });
	title->setFntFile("Determination.fnt"_spr);
	title->setPosition(CCPoint{ bg->getPositionX() - bg->getContentWidth() / 2 + 24, 145 });
}
void DeltaruneAlertLayer::changeText() {
	auto& textArea = m_fields->textArea;
	if (!textArea) return;
	textArea->removeFromParent();

	auto& sound = m_fields->textSound;
	auto& size = m_fields->textSize;
	bool& noShadow = m_fields->noShadow;

	noShadow = Mod::get()->getSettingValue<bool>("noShadow") || sound == "Sans" || sound == "Papyrus";

	CCLabelBMFont* star = CCLabelBMFont::create("*", "Determination.fnt"_spr);
	CCLabelBMFont* starShadow = nullptr;
	if (!noShadow) starShadow = CCLabelBMFont::create("*", "Determination.fnt"_spr);

	auto str = m_fields->text;
	auto screenSize = m_fields->screenSize;
	auto titleString = std::string_view(m_fields->title->getString());
	auto& bg = m_fields->bg;

	int xOffset = star->getContentWidth();

	if (m_fields->dialog) {
		xOffset = m_fields->characterSprite->getContentWidth() + 27 + star->getContentWidth();
		if (titleString == "The Mechanic") sound = "Alphys";
		else if (titleString == "Scratch") sound = "Lancer";
		else if (titleString == "The Shopkeeper") sound = "Spamton";
		else if (titleString == "Potbor") sound = "Spamton NEO";
		else if (titleString == "Diamond Shopkeeper") sound = "Papyrus";
		else if (titleString == "The Keymaster") sound = "Susie";
	}
	star->setPositionX(bg->getPositionX() - screenSize / 2 + xOffset - star->getContentWidth() + 27);
	star->setPositionY(110);
	star->setZOrder(1);
	star->setID("star"_spr);
	if (!noShadow) {
		starShadow->setPositionX(bg->getPositionX() - screenSize / 2 + xOffset - star->getContentWidth() + 28);
		starShadow->setPositionY(109);
		starShadow->setZOrder(0);
		starShadow->setID("starShadow"_spr);
		auto character = (CCSprite*) starShadow->getChildren()->objectAtIndex(0);
		character->setColor({ 15, 15, 127 });
	}
	std::string font = "Determination.fnt"_spr;
	if (sound == "Sans") font = "ComicSans.fnt"_spr;
	if (sound == "Papyrus") {
		font = "Papyrus.fnt"_spr;
		capitalize(str);
	}
	float creatingWidth = screenSize - 100 - xOffset * 2;
	auto newDesc = TextArea::create(
		str,
		font.c_str(),
		1,
		creatingWidth,
		CCPoint{ 0, 1 },
		size,
		false
	);

	newDesc->setContentWidth(screenSize);
	newDesc->setAnchorPoint(CCPoint{ 0, 1 });
	newDesc->setPositionY(110);
	newDesc->setZOrder(textArea->getZOrder());
	newDesc->setID("content-text-area");

	TextArea* newDescGrad = nullptr;
	TextArea* newDescShad = nullptr;
	if (!Mod::get()->getSettingValue<bool>("noGradientOverlay") && sound != "Sans" && sound != "Papyrus") {
		newDescGrad = TextArea::create(
			str,
			"DeterminationGradient.fnt"_spr,
			1,
			creatingWidth,
			CCPoint{ 0, 1 },
			size,
			false
		);
		newDescGrad->setContentWidth(screenSize);
		newDescGrad->setAnchorPoint(CCPoint{ 0, 1 });
		newDescGrad->setPositionY(110);
		newDescGrad->setZOrder(textArea->getZOrder() + 1);
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
	if (!noShadow) {
		newDescShad = TextArea::create(
			str,
			font.c_str(),
			1,
			creatingWidth,
			CCPoint{ 0, 1 },
			size,
			false
		);
		newDescShad->setContentWidth(screenSize);
		newDescShad->setAnchorPoint(CCPoint{ 0, 1 });
		newDescShad->setPositionY(109);
		newDescShad->setPositionX(newDescShad->getPositionX() + 1);
		newDescShad->setZOrder(textArea->getZOrder() - 1);
		newDescShad->setID("shadow"_spr);

		CCArrayExt<CCLabelBMFont*> linesShad = static_cast<CCNode*>(newDescShad->getChildren()->objectAtIndex(0))->getChildren();
		int i = 0;
		for (auto line : linesShad) {
			CCArrayExt<CCSprite*> letters = line->getChildren();
			int j = 0;
			for (auto letter : letters) {
				auto origLinesParent = (CCNode*) newDesc->getChildren()->objectAtIndex(0);
				auto origLine = (CCLabelBMFont*) origLinesParent->getChildren()->objectAtIndex(i);
				auto origChar = (CCSprite*) origLine->getChildren()->objectAtIndex(j);
				auto color = origChar->getColor();
				if (color == ccColor3B{ 255,255,255 })	letter->setColor({ 15, 15, 127 });
				else if (color == ccColor3B{ 255, 0, 255 }) letter->setColor({ 76, 0, 76 });
				else if (color == ccColor3B{ 255, 90, 90 }) letter->setColor({ 76, 0, 0 });
				else if (color == ccColor3B{ 255, 165, 75 }) letter->setColor({ 76, 38, 18 });
				else if (color == ccColor3B{ 255, 255, 0 }) letter->setColor({ 76, 76, 0 });
				else if (color == ccColor3B{ 64, 227, 72 }) letter->setColor({ 0, 76, 0 });
				else if (color == ccColor3B{ 74, 82, 255 }) letter->setColor({ 0, 0, 76 });
				else {
					uint8_t red = color.r / 2;
					uint8_t green = color.g / 2;
					uint8_t blue = color.b / 2;
					letter->setColor(ccColor3B{ red, green, blue });
				}

				letter->setVisible(false);
				j++;
			}
			i++;
		}
	}
	CCArrayExt<CCLabelBMFont*> lines = static_cast<CCNode*>(newDesc->getChildren()->objectAtIndex(0))->getChildren();

	for (auto line : lines) {
		CCArrayExt<CCSprite*> letters = line->getChildren();
		for (auto letter : letters) {
			letter->setVisible(false);
		}
	}
	m_mainLayer->addChild(star);
	if (!noShadow) m_mainLayer->addChild(starShadow);

	auto rect = CCLayerColor::create({ 0,0,0,0 }, bg->getContentWidth(), bg->getContentHeight() - 20);
	auto clippingNode = CCClippingNode::create(rect);
	clippingNode->setID("content-text-area"_spr);
	clippingNode->setPositionY(10);
	clippingNode->setPositionX(bg->getPositionX() - screenSize / 2 + 24 + xOffset);
	clippingNode->addChild(newDesc);
	if (newDescGrad) clippingNode->addChild(newDescGrad);
	if (!noShadow) clippingNode->addChild(newDescShad);
	m_mainLayer->addChild(clippingNode);
	m_fields->textAreaClippingNode = clippingNode;
	textArea = newDesc;
	m_fields->gradientOverlay = newDescGrad;
	m_fields->shadow = newDescShad;
	float pause = Mod::get()->getSettingValue<double>("textRollingPause");
	schedule(schedule_selector(DeltaruneAlertLayer::rollText), pause / 30);
}
void DeltaruneAlertLayer::changeLook() {
	changeBG();
	changeButtons();
	changeTitle();
	changeText();
}