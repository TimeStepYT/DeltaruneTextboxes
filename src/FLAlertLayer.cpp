#include "include.h"
#include "FLAlertLayer.h"

bool blockKeys = false;

bool MyFLAlertLayer::init(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale) {
	if (m_fields->screenSize >= 569 && !m_fields->dontRestrictWidth)
		m_fields->screenSize = 569;

	width = m_fields->screenSize;
	m_fields->text = desc;
	height = 140;
	scroll = false;
	textScale = 1;
	m_noElasticity = true;
	bool ret = FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale);

	NodeIDs::provideFor(this);

	if (m_fields->mainLayer = getChildByID("main-layer")) {
		if (m_buttonMenu) {
			m_fields->btn1 = m_buttonMenu->getChildByID("button-1");
			m_fields->btn2 = m_buttonMenu->getChildByID("button-2");
		}
		m_fields->textAreaClippingNode = m_fields->mainLayer->getChildByID("content-text-area");
		m_fields->bg = m_fields->mainLayer->getChildByID("background");
		m_fields->title = m_fields->mainLayer->getChildByID("title");
	}
	setID("FLAlertLayer");
	return ret;
}
void MyFLAlertLayer::showButtons(CCArrayExt<CCLabelBMFont*> content) {
	if (m_fields->btn2 && content.size() < 3) {
		m_fields->done = true;
		m_buttonMenu->setVisible(true);
	}
}
void MyFLAlertLayer::onBtn2(CCObject* sender) {
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
void MyFLAlertLayer::onBtn1(CCObject* sender) {
	if (!m_fields->done) {
		progressText();
		return;
	}
	blockKeys = false;
	FLAlertLayer::onBtn1(sender);
}
int MyFLAlertLayer::getLinesLeft(auto& content) {
	return content.size() - m_fields->linesProgressed;
}
void MyFLAlertLayer::progressText() {
	if (!m_fields->mainLayer) return;
	if (!m_buttonMenu) return;
	if (!m_fields->textAreaClippingNode) return;
	if (!m_fields->textArea) return;

	auto mlbmFont = m_fields->textArea->getChildren()->objectAtIndex(0);
	auto fontNode = static_cast<CCNode*>(mlbmFont);
	CCArrayExt<CCLabelBMFont*> content = fontNode->getChildren();

	if (getLinesLeft(content) <= 3) {
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

	// move EVERYTHING up
	int i = 0;
	int offset;

	if (getLinesLeft(content) > 3)
		offset = 3;
	else if (getLinesLeft(content) == 3)
		offset = 2;
	m_fields->linesProgressed += offset;
	m_fields->textArea->setPositionY(m_fields->textArea->getPositionY() + m_fields->textSize * offset);
	m_fields->gradientOverlay->setPositionY(m_fields->textArea->getPositionY());

	if (m_fields->btn2 && getLinesLeft(content) < 3) {
		m_fields->done = true;
		m_buttonMenu->setVisible(true);
	}
}
void MyFLAlertLayer::show() {
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
bool MyFLAlertLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
	if (!m_fields->done && !m_fields->disableClickToProgress)
		progressText();
	bool ret = FLAlertLayer::ccTouchBegan(touch, event);
	if (!m_fields->mainLayer) return ret;
	if (!m_buttonMenu) return ret;
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
void MyFLAlertLayer::keyDown(enumKeyCodes key) {
	if (key == enumKeyCodes::KEY_Z || key == enumKeyCodes::CONTROLLER_A) {
		progressText();
		return;
	}
	else if (key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_Left || key == enumKeyCodes::KEY_Right) {
		if (!m_fields->mainLayer) {
			FLAlertLayer::keyDown(key);
			return;
		}
		if (!m_fields->btn2) {
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