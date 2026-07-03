#include "Events.hpp"
#include "../src/FLAlertLayer.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    DeltaruneEvents::createDialogEvent.listen(DeltaruneEvents::createDialogBox).leak();
    DeltaruneEvents::createDialogReturnEvent.listen(DeltaruneEvents::createDialogBoxReturn).leak();
    DeltaruneEvents::createDialogWithVoiceEvent.listen(DeltaruneEvents::createDialogWithVoice).leak();
    DeltaruneEvents::CreateDialogFull().listen(DeltaruneEvents::createDialogFull).leak();
}

FLAlertLayer* DeltaruneEvents::createDialogBox(cocos2d::CCNode* characterPortrait, std::string const& title, std::string const& text) {
    auto unmodifiedAlert = DeltaruneAlertLayer::create(title.c_str(), text, "OK");
    auto alert = static_cast<DeltaruneAlertLayer*>(unmodifiedAlert);
    
    if (characterPortrait) {
        auto imageNode = alert->createImageNode();
        imageNode->setCharacterImage(characterPortrait);
    }

    alert->show();
    
    return alert;
}

FLAlertLayer* DeltaruneEvents::createDialogBoxReturn(
    FLAlertLayer** output, 
    CCNode* characterPortrait, 
    std::string const& title, 
    std::string const& text
) {
    auto alert = DeltaruneEvents::createDialogBox(characterPortrait, title, text);
    if (output)
        *output = alert;
    return alert;
}

void DeltaruneEvents::createDialogWithVoice(
    FLAlertLayer** output,
    CCNode* characterPortrait,
    std::string const& voice,
    std::string const& title,
    std::string const& text
) {
    auto alert = DeltaruneEvents::createDialogBox(characterPortrait, title, text);
    static_cast<DeltaruneAlertLayer*>(alert)->setTextSound(voice);
    if (output)
        *output = alert;
}

DeltaruneDialogObject::DeltaruneDialogObject(CCNode* characterSprite, std::string const& voice, std::string const& title, std::string const& text) {
    this->m_characterSprite = characterSprite;
    this->m_voice = std::make_shared<std::string>(voice);
    this->m_title = std::make_shared<std::string>(title);
    this->m_text = std::make_shared<std::string>(text);
}

void DeltaruneEvents::createDialogFull(FLAlertLayer** output, std::vector<DeltaruneDialogObject> const& objects) {
    // temporary. just trying to get the event to work at all
    auto const& firstObject = objects.at(0);

    FLAlertLayer* alert = nullptr;
    DeltaruneEvents::createDialogWithVoice(&alert, firstObject.m_characterSprite, *firstObject.m_voice, *firstObject.m_title, *firstObject.m_text);
    
    if (output)
        *output = alert;
}