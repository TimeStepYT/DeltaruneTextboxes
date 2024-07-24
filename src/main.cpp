#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/modify/CCMenuItemSpriteExtra.hpp>

class $modify(MyFLAlertLayer, FLAlertLayer) {
	struct Fields {
		int linesProgressed = 0;
		bool done = false;
		gd::string text = "";
		float screenSize = CCDirector::sharedDirector()->getWinSize().width;
		int textSize = 36;
		int btnSelected = 0;
	};
	bool init(FLAlertLayerProtocol * delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale) {
		if (m_fields->screenSize >= 569 && Mod::get()->getSettingValue<bool>("restrictWidth"))
			m_fields->screenSize = 569;
		width = m_fields->screenSize;
		m_fields->text = desc;
		height = 140;
		scroll = false;
		textScale = 1;
		this->m_noElasticity = true;
		return FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale);
	}
	CCScale9Sprite* changeBG(CCNode * bg, CCNode * mainLayer) {
		bg->removeFromParent();

		CCScale9Sprite* newBg = CCScale9Sprite::create("deltaruneSquare.png"_spr);
		newBg->setContentHeight(140);
		newBg->setContentWidth(m_fields->screenSize);
		newBg->setPosition(CCPoint(CCDirector::sharedDirector()->getWinSize().width / 2, 70));
		newBg->setZOrder(bg->getZOrder());
		newBg->setID("background");

		mainLayer->addChild(newBg);
		return newBg;
	}
	void changeButtons(CCNode * mainLayer, CCNode * newBg) {
		CCNode* mainMenu = mainLayer->getChildByID("main-menu");
		if (mainMenu == nullptr) return;
		mainMenu->setPositionY(32);
		mainMenu->setVisible(false);

		if (mainMenu->getChildByID("button-2") == nullptr) return;

		mainMenu->getChildByID("button-1")->setPositionX(newBg->getPositionX() - mainMenu->getPositionX() - m_fields->screenSize / 2 + m_fields->screenSize / 4);
		mainMenu->getChildByID("button-2")->setPositionX(newBg->getPositionX() - mainMenu->getPositionX() - m_fields->screenSize / 2 + (m_fields->screenSize / 4) * 3);

		CCArrayExt<CCNode*> buttons = mainMenu->getChildren();

		auto heart = CCSprite::create("heart.png"_spr);
		heart->setPositionX(-mainMenu->getChildByID("button-1")->getPositionX() + as<CCNode*>(mainMenu->getChildByID("button-1")->getChildren()->objectAtIndex(0))->getPositionX());
		heart->setPositionY(mainMenu->getChildByID("button-1")->getPositionY() + mainMenu->getChildByID("button-1")->getContentHeight() / 2);
		heart->setID("heart");
		as<CCNode*>(mainMenu->getChildByID("button-1")->getChildren()->objectAtIndex(0))->addChild(heart);

		for (auto button : buttons) {
			if (button != typeinfo_cast<CCMenuItemSpriteExtra*>(button)) continue;
			as<CCMenuItemSpriteExtra*>(button)->m_animationEnabled = false;
			CCArrayExt<CCNode*> parts = as<CCNode*>(button->getChildren()->objectAtIndex(0))->getChildren();
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
	void changeTitle(CCNode * mainLayer, CCNode * newBg) {
		mainLayer->getChildByID("title")->setAnchorPoint(CCPoint(0, 0.5));
		as<CCLabelBMFont*>(mainLayer->getChildByID("title"))->setFntFile("Determination.fnt"_spr);
		mainLayer->getChildByID("title")->setPosition(CCPoint(newBg->getPositionX() - newBg->getContentWidth() / 2 + 24, 145));
	}
	TextArea* changeText(CCNode * textArea, CCNode * newBg, CCNode * mainLayer) {
		textArea->removeFromParent();
		auto newDesc = TextArea::create(
			m_fields->text,
			"Determination.fnt"_spr,
			1,
			m_fields->screenSize - 100,
			CCPoint(0, 1),
			m_fields->textSize,
			false
		);
		newDesc->setContentWidth(m_fields->screenSize);
		newDesc->setAnchorPoint(CCPoint(0, 1));
		newDesc->setPositionX(newBg->getPositionX() - m_fields->screenSize / 2 + 24);
		newDesc->setPositionY(120);
		newDesc->setZOrder(textArea->getZOrder());
		newDesc->setID("content-text-area");
		mainLayer->addChild(newDesc);
		CCArrayExt<CCLabelBMFont*> content = as<CCNode*>(newDesc->getChildren()->objectAtIndex(0))->getChildren();
		int i = 0;
		for (CCLabelBMFont* line : content) {
			line->setPositionY(0 - m_fields->textSize * i);
			if (i >= 3)
				line->setVisible(false);
			i++;
		}

		return newDesc;
	}
	void showButtons(CCNode * mainLayer, CCArrayExt<CCLabelBMFont*> content) {
		if (mainLayer->getChildByID("main-menu")->getChildByID("button-2") != nullptr && content.size() < 3) {
			m_fields->done = true;
			mainLayer->getChildByID("main-menu")->setVisible(true);
		}
	}
	void changeLook() {
		CCNode* mainLayer = this->getChildByID("main-layer");
		if (mainLayer == nullptr) return;
		CCNode* bg = mainLayer->getChildByID("background");
		if (bg == nullptr) return;
		CCNode* textArea = mainLayer->getChildByID("content-text-area");
		if (textArea == nullptr) return;

		auto newBg = changeBG(bg, mainLayer);
		changeButtons(mainLayer, newBg);
		changeTitle(mainLayer, newBg);
		auto newDesc = changeText(textArea, newBg, mainLayer);
		CCArrayExt<CCLabelBMFont*> content = as<CCNode*>(newDesc->getChildren()->objectAtIndex(0))->getChildren();
		showButtons(mainLayer, content);
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
		FLAlertLayer::onBtn1(sender);
	}
	void progressText() {
		auto mainLayer = this->getChildByID("main-layer");
		if (mainLayer == nullptr) return;
		if (mainLayer->getChildByID("main-menu") == nullptr) return;
		if (mainLayer->getChildByID("content-text-area") == nullptr) return;

		auto btn1 = mainLayer->getChildByID("main-menu")->getChildByID("button-1");
		auto btn2 = mainLayer->getChildByID("main-menu")->getChildByID("button-2");
		auto mlbmfont = mainLayer->getChildByID("content-text-area")->getChildren()->objectAtIndex(0);
		CCArrayExt<CCLabelBMFont*> content = as<CCNode*>(mlbmfont)->getChildren();

		if (content.size() - m_fields->linesProgressed <= 3) {
			if (btn2 == nullptr) {
				m_fields->done = true;
				FLAlertLayer::onBtn1(as<CCObject*>(btn1));
				return;
			}
			else if (m_fields->btnSelected != 0) {
				m_fields->done = true;
				if (m_fields->btnSelected == 1)
					FLAlertLayer::onBtn1(as<CCObject*>(btn1));
				else if (m_fields->btnSelected == 2)
					FLAlertLayer::onBtn2(as<CCObject*>(btn2));
				return;
			}
		}

		// move EVERYTHING up
		int i = 0;
		if (content.size() - m_fields->linesProgressed > 3) {
			m_fields->linesProgressed += 3;
			for (CCLabelBMFont* line : content) {
				line->setPositionY(0 - m_fields->textSize * i + m_fields->textSize * m_fields->linesProgressed);
				if (i >= m_fields->linesProgressed && i < m_fields->linesProgressed + 3)
					line->setVisible(true);
				else
					line->setVisible(false);
				i++;
			}
		}
		i = 0;
		if (content.size() - m_fields->linesProgressed == 3) {
			m_fields->linesProgressed += 2;
			for (CCLabelBMFont* line : content) {
				line->setPositionY(0 - m_fields->textSize * i + m_fields->textSize * m_fields->linesProgressed);
				if (i >= m_fields->linesProgressed && i < m_fields->linesProgressed + 3)
					line->setVisible(true);
				else
					line->setVisible(false);
				i++;
			}
		}

		if (btn2 != nullptr && content.size() - m_fields->linesProgressed < 3) {
			m_fields->done = true;
			mainLayer->getChildByID("main-menu")->setVisible(true);
		}
	}
	void show() {
		FLAlertLayer::show();
		changeLook();
	}
	void addHeart(CCNode * parent, CCLabelBMFont * label) {
		if (parent->getChildByID("heart") != nullptr) {
			parent->getChildByID("heart")->removeFromParentAndCleanup(true);
		}
		auto heart = CCSprite::create("heart.png"_spr);
		heart->setAnchorPoint(CCPoint(1, 0.5));
		heart->setPosition(CCPoint(label->getPositionX() - 5 - label->getContentWidth() / 2, label->getPositionY() - 2));
		heart->setID("heart");
		parent->addChild(heart);
	}
	bool ccTouchBegan(CCTouch * touch, CCEvent * event) {
		if (Mod::get()->getSettingValue<bool>("clickToProgress"))
			progressText();
		bool ret = FLAlertLayer::ccTouchBegan(touch, event);
		CCArrayExt<CCMenuItemSpriteExtra*> buttons = this->getChildByID("main-layer")->getChildByID("main-menu")->getChildren();
		for (auto button : buttons) {
			auto parent = as<CCNode*>(button->getChildren()->objectAtIndex(0));
			CCArrayExt<CCNode*> parts = parent->getChildren();
			for (auto part : parts) {
				if (auto label = typeinfo_cast<CCLabelBMFont*>(part)) {
					if (button->isSelected()) {
						label->setColor(_ccColor3B(255, 255, 0));
						addHeart(parent, label);
						if (button->getID() == "button-1")
							m_fields->btnSelected = 1;
						else if (button->getID() == "button-2")
							m_fields->btnSelected = 2;
					}
					else {
						label->setColor(_ccColor3B(255, 255, 255));
						auto heart = parent->getChildByID("heart");
						if (heart != nullptr)
							heart->removeFromParentAndCleanup(true);
					}
				}
			}
		}
		return ret;
	}
	void keyDown(enumKeyCodes key) {
		log::info("{}", (int) key);
		if (key == enumKeyCodes::KEY_Z || key == enumKeyCodes::CONTROLLER_A) {
			progressText();
			return;
		}
		else if (key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_Left || key == enumKeyCodes::KEY_Right) {
			if (this->getChildByID("main-layer") == nullptr) {
				FLAlertLayer::keyDown(key);
				return;
			}
			CCNode* btn1 = this->getChildByID("main-layer")->getChildByID("main-menu")->getChildByID("button-1");
			CCNode* btn2 = this->getChildByID("main-layer")->getChildByID("main-menu")->getChildByID("button-2");
			if (btn2 == nullptr) {
				FLAlertLayer::keyDown(key);
				return;
			}
			auto parent = as<CCNode*>(btn1->getChildren()->objectAtIndex(0));
			CCArrayExt<CCNode*> partsBtn1 = parent->getChildren();
			for (auto part : partsBtn1) {
				if (auto label = typeinfo_cast<CCLabelBMFont*>(part)) {
					if (key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_Left) {
						m_fields->btnSelected = 1;
						addHeart(parent, label);
						label->setColor(_ccColor3B(255, 255, 0));
					}
					else if (key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_Right) {
						label->setColor(_ccColor3B(255, 255, 255));
						auto heart = parent->getChildByID("heart");
						if (heart != nullptr)
							heart->removeFromParentAndCleanup(true);
					}
				}
			}
			parent = as<CCNode*>(btn2->getChildren()->objectAtIndex(0));
			CCArrayExt<CCNode*> partsBtn2 = parent->getChildren();
			for (auto part : partsBtn2) {
				if (auto label = typeinfo_cast<CCLabelBMFont*>(part)) {
					if (key == enumKeyCodes::KEY_ArrowLeft || key == enumKeyCodes::KEY_Left) {
						label->setColor(_ccColor3B(255, 255, 255));
						auto heart = parent->getChildByID("heart");
						if (heart != nullptr)
							heart->removeFromParentAndCleanup(true);
					}
					else if (key == enumKeyCodes::KEY_ArrowRight || key == enumKeyCodes::KEY_Right) {
						m_fields->btnSelected = 2;
						addHeart(parent, label);
						label->setColor(_ccColor3B(255, 255, 0));
					}
				}
			}
		}
		FLAlertLayer::keyDown(key);
	}
};