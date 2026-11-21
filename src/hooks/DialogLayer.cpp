#include "DialogLayer.hpp"
#include "FLAlertLayer.hpp"

using namespace geode::prelude;

void DeltaruneDialogLayer::changeDialogDesign(DialogObject* dialogObject) {
	std::vector<DialogObject*> objects;

	m_fields->dialogObject = dialogObject;
	std::string character = "";
	char const* title = "";
	std::string text = "";

	if (dialogObject) {
		character = dialogObject->m_character;
		text = dialogObject->m_text;
		title = character.c_str();
	}
	else {
		if (this->m_dialogObjects) {
			objects.reserve(this->m_dialogObjects->count());
			if (this->m_dialogObjects) {
				for (auto object : CCArrayExt<DialogObject*>{this->m_dialogObjects}) {
					objects.push_back(object);
				}
			}
		}
		if (objects.size() > 0) {
			character = objects.at(0)->m_character;
			title = character.c_str();
			for (size_t i = 0; i < objects.size(); i++) {
				if (text.length() == 0) {
					text = objects.at(i)->m_text;
				}
				else
					text = fmt::format("{}\n\n\n\n{}", text, objects.at(i)->m_text);
			}
		}
	}
	auto const unmodifiedAlert = DeltaruneAlertLayer::create(title, text, "OK");
	auto const alert = static_cast<DeltaruneAlertLayer*>(unmodifiedAlert);
	auto const imageNode = alert->createImageNode();

	if (dialogObject) {
		auto const texture = m_characterSprite->getTexture();

		imageNode->setCharacterImage(texture);
	}
	else {
		if (objects.size() > 0) {
			auto const firstObj = objects.at(0);
			std::string textureName = fmt::format("dialogIcon_{:03}.png", firstObj->m_characterFrame);

			imageNode->setCharacterImage(textureName);

			for (int i = 0; i < objects.size(); i++) {
				auto const diaObj = objects.at(i);
				textureName = fmt::format("dialogIcon_{:03}.png", diaObj->m_characterFrame);
				alert->m_fields->characterSpriteNames.push_back(textureName);
				alert->m_fields->titles.push_back(diaObj->m_character);
			}
		}
	}
	m_fields->alert = unmodifiedAlert;
	alert->m_fields->dialog = true;
	alert->m_fields->dialogLayer = this;
	
	alert->show();

	Loader::get()->queueInMainThread([this, alert] {
		this->setVisible(false);
		if (!this->getParent()) {
			alert->addChild(this);
		}
	});
}

void DeltaruneDialogLayer::displayDialogObject(DialogObject* obj) {
	DialogLayer::displayDialogObject(obj);
	this->changeDialogDesign(obj);
	this->m_mainLayer->setVisible(false);
	this->setVisible(false);
}

#ifndef GEODE_IS_IOS
bool DeltaruneDialogLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
	return false;
}
void DeltaruneDialogLayer::keyDown(enumKeyCodes key, double timestamp) {}
#endif