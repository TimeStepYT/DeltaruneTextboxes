#include "DialogLayer.h"
#include "FLAlertLayer.h"

bool DeltaruneDialogLayer::init(DialogObject* p0, CCArray* p1, int p2) {
	if (!DialogLayer::init(p0, p1, p2)) return false;
	m_fields->p0 = p0;
	char const* title = m_fields->p0->m_character.c_str();
	gd::string text = m_fields->p0->m_text;
	auto alert = FLAlertLayer::create(title, text, "OK");
	auto texture = m_characterSprite->getTexture();
	
#ifdef GEODE_IS_WINDOWS
	static_cast<DeltaruneAlertLayer*>(alert)->m_fields->characterSprite = CCSprite::createWithTexture(texture);
#else
	static_cast<DeltaruneAlertLayer*>(alert)->m_fields->characterSprite = CCSpriteGrayscale::createWithTexture(texture);
#endif
	static_cast<DeltaruneAlertLayer*>(alert)->m_fields->dialog = true;
	alert->show();

	this->setOpacity(0);
	geode::Loader::get()->queueInMainThread([this] {
		onClose();
		});
	return true;
}

void DeltaruneDialogLayer::displayDialogObject(DialogObject* p0) {
	DialogLayer::displayDialogObject(p0);
}