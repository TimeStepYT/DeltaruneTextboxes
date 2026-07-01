#include "Events.hpp"
#include "FLAlertLayer.hpp"

$on_mod(Loaded) {
    DeltaruneEvents::createDialogEvent.listen(DeltaruneEvents::createDialogBox).leak();
}

void DeltaruneEvents::createDialogBox(cocos2d::CCNode* characterPortrait, std::string const& title, std::string const& text) {
    auto unmodifiedAlert = DeltaruneAlertLayer::create(title.c_str(), text, "OK");
    auto alert = static_cast<DeltaruneAlertLayer*>(unmodifiedAlert);
    auto imageNode = alert->createImageNode();

    imageNode->setCharacterImage(characterPortrait);

    alert->show();
}
