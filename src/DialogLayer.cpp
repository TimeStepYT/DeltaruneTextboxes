#include "DialogLayer.h"
#include "FLAlertLayer.h"

bool DeltaruneDialogLayer::init(DialogObject* p0, CCArray* p1, int p2) {
	if (!DialogLayer::init(p0, p1, p2)) return false;
	m_fields->p0 = p0;
	char const* title = m_fields->p0->m_character.c_str();
	gd::string text = m_fields->p0->m_text;
	auto alert = FLAlertLayer::create(title, text, "OK");

	auto texture = m_characterSprite->getTexture();
#ifdef GEODE_IS_ANDROID || GEODE_IS_MACOS
	static_cast<DeltaruneAlertLayer*>(alert)->m_fields->characterSprite = CCSpriteGrayscale::createWithTexture(texture);
#else
	static_cast<DeltaruneAlertLayer*>(alert)->m_fields->characterSprite = CCSprite::createWithTexture(texture);
#endif
	static_cast<DeltaruneAlertLayer*>(alert)->m_fields->dialog = true;
	alert->show();

	this->setOpacity(0);
	this->schedule(schedule_selector(DeltaruneDialogLayer::closeWithDelay));
	return true;
}

void DeltaruneDialogLayer::displayDialogObject(DialogObject* p0) {
	DialogLayer::displayDialogObject(p0);
}

void DeltaruneDialogLayer::closeWithDelay(float dt) {
	if (!m_characterSprite) return;
	onClose();
	unschedule(schedule_selector(DeltaruneDialogLayer::closeWithDelay));
}