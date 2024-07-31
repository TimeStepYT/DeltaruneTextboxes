#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

bool blockKeys = false;

class $modify(MyHookLol, CCKeyboardDispatcher) {
	bool dispatchKeyboardMSG(enumKeyCodes key, bool down, bool idk) {
		if (blockKeys && down) {
			if (key == enumKeyCodes::KEY_Left || key == enumKeyCodes::KEY_Right)
				return true;
			else if (key == enumKeyCodes::KEY_Escape)
				blockKeys = false;
		}

		return CCKeyboardDispatcher::dispatchKeyboardMSG(key, down, idk);
	}
};
class $modify(MyFLAlertLayer, FLAlertLayer) {
	struct Fields {
		int linesProgressed = 0;
		bool done = false;
		gd::string text = "";
		float screenSize = CCDirector::sharedDirector()->getWinSize().width;
		int textSize = 36;
		int btnSelected = 0;
		bool dontRestrictWidth = Mod::get()->getSettingValue<bool>("dontRestrictWidth");
		bool disableClickToProgress = Mod::get()->getSettingValue<bool>("disableClickToProgress");
		CCNode* mainLayer;
		CCNode* mainMenu;
		CCNode* btn1;
		CCNode* btn2;
		CCNode* textArea;
		CCNode* bg;
		CCNode* title;
	};
	bool init(FLAlertLayerProtocol * delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale) {
		if (m_fields->screenSize >= 569 && !m_fields->dontRestrictWidth)
			m_fields->screenSize = 569;
		width = m_fields->screenSize;
		m_fields->text = desc;
		height = 140;
		scroll = false;
		textScale = 1;
		this->m_noElasticity = true;
		bool ret = FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale);
		if (m_fields->mainLayer = this->getChildByID("main-layer")) {
			if (m_fields->mainMenu = m_fields->mainLayer->getChildByID("main-menu")) {
				m_fields->btn1 = m_fields->mainMenu->getChildByID("button-1");
				m_fields->btn2 = m_fields->mainMenu->getChildByID("button-2");
			}
			m_fields->textArea = m_fields->mainLayer->getChildByID("content-text-area");
			m_fields->bg = m_fields->mainLayer->getChildByID("background");
			m_fields->title = m_fields->mainLayer->getChildByID("title");
		}
		this->setID("FLAlertLayer");
		return ret;
	}
	void changeBG() {
		m_fields->bg->removeFromParent();

		m_fields->bg = CCScale9Sprite::create("deltaruneSquare.png"_spr);
		m_fields->bg->setContentHeight(140);
		m_fields->bg->setContentWidth(m_fields->screenSize);
		m_fields->bg->setPosition(CCPoint{ CCDirector::sharedDirector()->getWinSize().width / 2, 70 });
		m_fields->bg->setID("background");

		m_fields->mainLayer->addChild(m_fields->bg);
	}
	void changeButtons() {
		if (!m_fields->mainMenu) return;
		m_fields->mainMenu->setPositionY(32);
		m_fields->mainMenu->setVisible(false);

		if (!m_fields->btn2) return;

		m_fields->btn1->setPositionX(m_fields->bg->getPositionX() - m_fields->mainMenu->getPositionX() - m_fields->screenSize / 2 + m_fields->screenSize / 4);
		m_fields->btn2->setPositionX(m_fields->bg->getPositionX() - m_fields->mainMenu->getPositionX() - m_fields->screenSize / 2 + (m_fields->screenSize / 4) * 3);

		CCArrayExt<CCNode*> buttons = m_fields->mainMenu->getChildren();

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
				}
			}
		}
	}
	void changeTitle() {
		m_fields->title->setAnchorPoint(CCPoint{ 0, 0.5 });
		static_cast<CCLabelBMFont*>(m_fields->title)->setFntFile("Determination.fnt"_spr);
		m_fields->title->setPosition(CCPoint{ m_fields->bg->getPositionX() - m_fields->bg->getContentWidth() / 2 + 24, 145 });
	}
	void changeText() {
		m_fields->textArea->removeFromParent();
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
		newDesc->setPositionY(120);
		newDesc->setZOrder(m_fields->textArea->getZOrder());
		newDesc->setID("content-text-area");
		m_fields->mainLayer->addChild(newDesc);
		m_fields->textArea = newDesc;
		CCArrayExt<CCLabelBMFont*> content = static_cast<CCNode*>(newDesc->getChildren()->objectAtIndex(0))->getChildren();
		int i = 0;
		for (CCLabelBMFont* line : content) {
			line->setPositionY(0 - m_fields->textSize * i);
			if (i >= 3)
				line->setVisible(false);
			i++;
		}
	}
	void showButtons(CCArrayExt<CCLabelBMFont*> content) {
		if (m_fields->btn2 && content.size() < 3) {
			m_fields->done = true;
			m_fields->mainMenu->setVisible(true);
		}
	}
	void changeLook() {
		if (!m_fields->bg) return;
		if (!m_fields->title) return;
		if (!m_fields->textArea) return;
		if (!m_fields->mainLayer) return;

		changeBG();
		changeButtons();
		changeTitle();
		changeText();
		CCArrayExt<CCLabelBMFont*> content = static_cast<CCNode*>(m_fields->textArea->getChildren()->objectAtIndex(0))->getChildren();
		showButtons(content);
	}
	// I can't check for enter key so I guess I have to hook these
	void onBtn2(CCObject * sender) {
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
	void onBtn1(CCObject * sender) {
		if (!m_fields->done) {
			progressText();
			return;
		}
		blockKeys = false;
		FLAlertLayer::onBtn1(sender);
	}
	int getLinesLeft(auto & content) {
		return content.size() - m_fields->linesProgressed;
	}
	void progressText() {
		if (!m_fields->mainLayer) return;
		if (!m_fields->mainMenu) return;
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

		if (getLinesLeft(content) > 3)
			m_fields->linesProgressed += 3;

		else if (getLinesLeft(content) == 3)
			m_fields->linesProgressed += 2;

		for (CCLabelBMFont* line : content) {
			line->setPositionY(0 - m_fields->textSize * i + m_fields->textSize * m_fields->linesProgressed);
			// hide the lines outside of the textbox
			// TODO replace with CCClippingNode
			if (i >= m_fields->linesProgressed && i < m_fields->linesProgressed + 3)
				line->setVisible(true);
			else
				line->setVisible(false);
			i++;
		}

		if (m_fields->btn2 && getLinesLeft(content) < 3) {
			m_fields->done = true;
			m_fields->mainMenu->setVisible(true);
		}
	}
	void show() {
		FLAlertLayer::show();
		int numOfSiblings = 0;
		if (auto parent = this->getParent()) {
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
	void addHeart(CCNode * parent, CCLabelBMFont * label) {
		if (parent->getChildByID("heart")) {
			parent->getChildByID("heart")->removeFromParentAndCleanup(true);
		}
		auto heart = CCSprite::create("heart.png"_spr);
		heart->setAnchorPoint(CCPoint{ 1, 0.5 });
		heart->setPosition(CCPoint{ label->getPositionX() - 5 - label->getContentWidth() / 2, label->getPositionY() - 2 });
		heart->setID("heart");
		parent->addChild(heart);
	}
	bool ccTouchBegan(CCTouch * touch, CCEvent * event) {
		if (!m_fields->done && !m_fields->disableClickToProgress)
			progressText();
		bool ret = FLAlertLayer::ccTouchBegan(touch, event);
		if (!m_fields->mainLayer) return ret;
		if (!m_fields->mainMenu) return ret;
		CCArrayExt<CCMenuItemSpriteExtra*> buttons = m_fields->mainMenu->getChildren();
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
	void keyDown(enumKeyCodes key) {
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
};