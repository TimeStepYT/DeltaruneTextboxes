#include "DialogLayer.h"
#include "FLAlertLayer.h"

bool DeltaruneDialogLayer::init(DialogObject* dialogObject, CCArray* objectsOriginal, int p2) {
	CCArray* objects = CCArray::create();
	if (objectsOriginal) objects->addObjectsFromArray(objectsOriginal);
	if (!DialogLayer::init(dialogObject, objectsOriginal, p2)) return false;

	m_fields->dialogObject = dialogObject;
	std::string character;
	char const* title;
	std::string text = "";
	if (dialogObject) {
		character = dialogObject->m_character;
		text = dialogObject->m_text;
		title = character.c_str();
	}
	else {
		character = static_cast<DialogObject*>(objects->firstObject())->m_character;
		title = character.c_str();
		for (size_t i = 0; i < objects->count(); i++) {
			if (text.length() == 0) {
				text = static_cast<DialogObject*>(objects->objectAtIndex(i))->m_text;
			}
			else
				text = fmt::format("{}\n\n\n\n{}", text, static_cast<DialogObject*>(objects->objectAtIndex(i))->m_text);
		}
	}
	auto unmodifiedAlert = FLAlertLayer::create(title, text, "OK");
	auto alert = static_cast<DeltaruneAlertLayer*>(unmodifiedAlert);

	auto& imageNode = alert->m_fields->imageNode;

	alert->createImageNode();

	if (dialogObject) {
		CCSprite* sprite;
		auto texture = m_characterSprite->getTexture();

		imageNode->setCharacterImage(texture);
	}
	else {
		auto firstObj = static_cast<DialogObject*>(objects->firstObject());
		std::string textureName = fmt::format("dialogIcon_{:03}.png", firstObj->m_characterFrame);

		imageNode->setCharacterImage(textureName);

		for (int i = 0; i < objects->count(); i++) {
			auto diaObj = static_cast<DialogObject*>(objects->objectAtIndex(i));
			textureName = fmt::format("dialogIcon_{:03}.png", diaObj->m_characterFrame);
			alert->m_fields->characterSpriteNames.push_back(textureName);
			alert->m_fields->titles.push_back(diaObj->m_character);
		}
	}
	m_fields->alert = unmodifiedAlert;
	alert->m_fields->dialog = true;
	alert->m_fields->dialogLayer = this;

	Loader::get()->queueInMainThread([=, this] {
		this->setVisible(false);
		if (!this->getParent()) {
			alert->addChild(this);
		}
		});

	alert->show();
	return true;
}
void DeltaruneDialogLayer::displayNextObject() {
	this->setVisible(false);
}
void DeltaruneDialogLayer::displayDialogObject(DialogObject* obj) {
	DialogLayer::displayDialogObject(obj);
	this->setVisible(false);
}
bool DeltaruneDialogLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
	return false;
}
void DeltaruneDialogLayer::keyDown(enumKeyCodes key) {}
