#include <Geode/Geode.hpp>

using namespace geode::prelude;

#include <Geode/modify/FLAlertLayer.hpp>
#include <Geode/modify/CCKeyboardDispatcher.hpp>

auto screenSize = CCDirector::sharedDirector()->getWinSize().width;

class $modify(MyFLAlertLayer, FLAlertLayer) {
	struct Fields {
		int linesProgressed = 0;
	};
	static FLAlertLayer* create(FLAlertLayerProtocol * delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale) {
		if (screenSize >= 569)
			screenSize = 569;
		width = screenSize - 225;
		height = 140;
		scroll = false;
		textScale = 1;
		return FLAlertLayer::create(delegate, title, desc, btn1, btn2, width, scroll, height, textScale);
	}
	void changeLook() {
		CCNode* mainLayer = this->getChildByID("main-layer");
		if (mainLayer == nullptr) return;
		CCNode* bg = mainLayer->getChildByID("background");
		if (bg == nullptr) return;
		CCNode* textArea = mainLayer->getChildByID("content-text-area");
		if (textArea == nullptr) return;

		bg->removeFromParent();

		CCScale9Sprite* newBg = CCScale9Sprite::create("deltaruneSquare.png"_spr);
		newBg->setContentHeight(140);
		newBg->setContentWidth(screenSize);
		newBg->setPosition(CCPoint(CCDirector::sharedDirector()->getWinSize().width / 2, 70));
		newBg->setZOrder(bg->getZOrder());
		newBg->setID("background");

		mainLayer->addChild(newBg);

		mainLayer->getChildByID("main-menu")->setPositionY(32);
		mainLayer->getChildByID("main-menu")->setVisible(false);

		textArea->setAnchorPoint(CCPoint(0.5, 1));
		textArea->setPositionY(115);
		auto mlbmfont = textArea->getChildren()->objectAtIndex(0);
		CCArrayExt<CCLabelBMFont*> content = as<CCNode*>(mlbmfont)->getChildren();

		int textSize = 36;
		int i = 0;
		for (CCLabelBMFont* line : content) {
			line->setFntFile("Determination.fnt"_spr);
			line->setAnchorPoint(CCPoint(0, 0.5));
			line->setPositionX(-screenSize / 2 + 24);
			line->setPositionY(-8 - textSize * i);
			if (i >= 3)
				line->setVisible(false);
			i++;
		}

		if (mainLayer->getChildByID("main-menu")->getChildByID("button-2") != nullptr && content.size() < 3)
			mainLayer->getChildByID("main-menu")->setVisible(true);
	}
	void progressText() {
		auto mainLayer = this->getChildByID("main-layer");
		if (mainLayer == nullptr) return;
		auto btn1 = mainLayer->getChildByID("main-menu")->getChildByID("button-1");
		auto btn2 = mainLayer->getChildByID("main-menu")->getChildByID("button-2");
		auto mlbmfont = mainLayer->getChildByID("content-text-area")->getChildren()->objectAtIndex(0);
		CCArrayExt<CCLabelBMFont*> content = as<CCNode*>(mlbmfont)->getChildren();
		int textSize = 36;

		if (content.size() - m_fields->linesProgressed <= 3 && btn2 == nullptr) {
			FLAlertLayer::onBtn1(as<CCObject*>(btn1));
			return;
		}

		// move EVERYTHING up
		int i = 0;
		if (content.size() - m_fields->linesProgressed > 3) {
			m_fields->linesProgressed += 3;
			for (CCLabelBMFont* line : content) {
				line->setPositionY(-8 - textSize * i + textSize * m_fields->linesProgressed);
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
				line->setPositionY(-8 - textSize * i + textSize * m_fields->linesProgressed);
				if (i >= m_fields->linesProgressed && i < m_fields->linesProgressed + 3)
					line->setVisible(true);
				else
					line->setVisible(false);
				i++;
			}
		}

		if (btn2 != nullptr && content.size() - m_fields->linesProgressed < 3)
			mainLayer->getChildByID("main-menu")->setVisible(true);
	}
	void show() {
		FLAlertLayer::show();
		changeLook();
	}
	bool ccTouchBegan(CCTouch * touch, CCEvent * event) {
		progressText();
		return ccTouchBegan(touch, event);
	}
	void keyDown(enumKeyCodes key) {
		if (key == enumKeyCodes::KEY_Enter || key == enumKeyCodes::KEY_Z || key == enumKeyCodes::CONTROLLER_A) {
			progressText();
			return;
		}
		keyDown(key);
	}
};