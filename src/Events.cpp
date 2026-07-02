#include "Events.hpp"
#include "FLAlertLayer.hpp"

$on_mod(Loaded) {
    DeltaruneEvents::createDialogEvent.listen(DeltaruneEvents::createDialogBox).leak();
    DeltaruneEvents::createDialogReturnEvent.listen(DeltaruneEvents::createDialogBoxReturn).leak();
    DeltaruneEvents::createDialogWithVoiceEvent.listen(DeltaruneEvents::createDialogWithVoice).leak();
}

DeltaruneAlertLayer* DeltaruneEvents::createDialogBox(cocos2d::CCNode* characterPortrait, std::string const& title, std::string const& text) {
    auto unmodifiedAlert = DeltaruneAlertLayer::create(title.c_str(), text, "OK");
    auto alert = static_cast<DeltaruneAlertLayer*>(unmodifiedAlert);
    
    if (characterPortrait) {
        auto imageNode = alert->createImageNode();
        imageNode->setCharacterImage(characterPortrait);
    }

    alert->show();
    
    return alert;
}

DeltaruneAlertLayer* DeltaruneEvents::createDialogBoxReturn(
    FLAlertLayer*& output, 
    cocos2d::CCNode* characterPortrait, 
    std::string const& title, 
    std::string const& text
) {
    auto alert = DeltaruneEvents::createDialogBox(characterPortrait, title, text);
    output = alert;
    return alert;
}

void DeltaruneEvents::createDialogWithVoice(
    FLAlertLayer*& output,
    cocos2d::CCNode* characterPortrait,
    std::string const& voice,
    std::string const& title,
    std::string const& text
) {
    auto alert = DeltaruneEvents::createDialogBox(characterPortrait, title, text);
    alert->setTextSound(voice);
    output = alert;
}