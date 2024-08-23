#include "include.h"
#include "FLAlertLayer.h"

void MyFLAlertLayer::changeBG() {
	m_fields->bg->removeFromParent();

	m_fields->bg = CCScale9Sprite::create("deltaruneSquare.png"_spr);
	m_fields->bg->setContentHeight(140);
	m_fields->bg->setContentWidth(m_fields->screenSize);
	m_fields->bg->setPosition(CCPoint{ CCDirector::sharedDirector()->getWinSize().width / 2, 70 });
	m_fields->bg->setID("background");

	m_fields->mainLayer->addChild(m_fields->bg);
}

void MyFLAlertLayer::changeButtons() {
	if (!m_buttonMenu) return;
	m_buttonMenu->setPositionY(32);
	m_buttonMenu->setVisible(false);

	if (!m_fields->btn2) return;

	m_fields->btn1->setPositionX(m_fields->bg->getPositionX() - m_buttonMenu->getPositionX() - m_fields->screenSize / 2 + m_fields->screenSize / 4);
	m_fields->btn2->setPositionX(m_fields->bg->getPositionX() - m_buttonMenu->getPositionX() - m_fields->screenSize / 2 + (m_fields->screenSize / 4) * 3);

	CCArrayExt<CCNode*> buttons = m_buttonMenu->getChildren();

	auto heart = CCSprite::create("heart.png"_spr);
	heart->setPositionX(-m_fields->btn1->getPositionX() + static_cast<CCNode*>(m_fields->btn1->getChildren()->objectAtIndex(0))->getPositionX());
	heart->setPositionY(m_fields->btn1->getPositionY() + m_fields->btn1->getContentHeight() / 2);
	heart->setID("heart");
	static_cast<CCNode*>(m_fields->btn1->getChildren()->objectAtIndex(0))->addChild(heart);

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
void MyFLAlertLayer::changeTitle() {
	m_fields->title->setAnchorPoint(CCPoint{ 0, 0.5 });
	static_cast<CCLabelBMFont*>(m_fields->title)->setFntFile("Determination.fnt"_spr);
	m_fields->title->setPosition(CCPoint{ m_fields->bg->getPositionX() - m_fields->bg->getContentWidth() / 2 + 24, 145 });
}
void MyFLAlertLayer::changeText() {
	m_fields->textAreaClippingNode->removeFromParent();
	auto newDesc = TextArea::create(
		m_fields->text,
		"Determination.fnt"_spr,
		1,
		m_fields->screenSize - 100,
		CCPoint{ 0, 1 },
		m_fields->textSize,
		false
	);
	newDesc->setContentWidth(m_fields->screenSize);
	newDesc->setAnchorPoint(CCPoint{ 0, 1 });
	newDesc->setPositionX(m_fields->bg->getPositionX() - m_fields->screenSize / 2 + 24);
	newDesc->setPositionY(110);
	newDesc->setZOrder(m_fields->textAreaClippingNode->getZOrder());
	newDesc->setID("content-text-area");

	auto newDescGrad = TextArea::create(
		m_fields->text,
		"DeterminationGradient.fnt"_spr,
		1,
		m_fields->screenSize - 100,
		CCPoint{ 0, 1 },
		m_fields->textSize,
		false
	);
	newDescGrad->setContentWidth(m_fields->screenSize);
	newDescGrad->setAnchorPoint(CCPoint{ 0, 1 });
	newDescGrad->setPositionX(m_fields->bg->getPositionX() - m_fields->screenSize / 2 + 24);
	newDescGrad->setPositionY(110);
	newDescGrad->setZOrder(m_fields->textAreaClippingNode->getZOrder() + 1);
	newDescGrad->setID("gradient-overlay"_spr);

	CCArrayExt<CCLabelBMFont*> linesGrad = static_cast<CCNode*>(newDescGrad->getChildren()->objectAtIndex(0))->getChildren();
	CCArrayExt<CCLabelBMFont*> lines = static_cast<CCNode*>(newDesc->getChildren()->objectAtIndex(0))->getChildren();

	for (auto line : linesGrad) {
		CCArrayExt<CCSprite*> letters = line->getChildren();
		for (auto letter : letters) {
			letter->setColor(ccColor3B{ 255, 255, 255 });
			letter->setVisible(false);
		}
	}
	for (auto line : lines) {
		CCArrayExt<CCSprite*> letters = line->getChildren();
		for (auto letter : letters) {
			letter->setVisible(false);
		}
	}

	auto clippingNode = CCClippingNode::create(CCLayerColor::create({ 0,0,0,0 }, m_fields->bg->getContentWidth(), m_fields->bg->getContentHeight() - 20));
	clippingNode->setID("content-text-area"_spr);
	clippingNode->setPositionY(10);
	clippingNode->addChild(newDesc);
	clippingNode->addChild(newDescGrad);
	m_fields->mainLayer->addChild(clippingNode);
	m_fields->textAreaClippingNode = clippingNode;
	m_fields->textArea = newDesc;
	m_fields->gradientOverlay = newDescGrad;
	float pause = Mod::get()->getSettingValue<double>("textRollingPause");
	schedule(schedule_selector(MyFLAlertLayer::rollText), pause / 30);
}
void MyFLAlertLayer::changeLook() {
	if (!m_fields->bg) return;
	if (!m_fields->title) return;
	if (!m_fields->textAreaClippingNode) return;
	if (!m_fields->mainLayer) return;

	changeBG();
	changeButtons();
	changeTitle();
	changeText();
}
void MyFLAlertLayer::addHeart(CCNode* parent, CCLabelBMFont* label) {
	if (parent->getChildByID("heart")) {
		parent->getChildByID("heart")->removeFromParentAndCleanup(true);
	}
	auto heart = CCSprite::create("heart.png"_spr);
	heart->setAnchorPoint(CCPoint{ 1, 0.5 });
	heart->setPosition(CCPoint{ label->getPositionX() - 5 - label->getContentWidth() / 2, label->getPositionY() - 2 });
	heart->setID("heart");
	parent->addChild(heart);
}