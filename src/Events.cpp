#include "Events.hpp"
#include "hooks/FLAlertLayer.hpp"

using namespace geode::prelude;

$on_mod(Loaded) {
    DeltaruneEvents::createDialogEvent.listen(DeltaruneEvents::createDialogBox).leak();
    DeltaruneEvents::createDialogReturnEvent.listen(DeltaruneEvents::createDialogBoxReturn).leak();
    DeltaruneEvents::createDialogWithVoiceEvent.listen(DeltaruneEvents::createDialogWithVoice).leak();
}

// Only here to help the events to work. I don't need this in a header file.
FLAlertLayer* createDialogBoxUnshown(cocos2d::CCNode* characterPortrait, std::string const& title, std::string const& text) {
    auto unmodifiedAlert = DeltaruneAlertLayer::create(title.c_str(), text, "OK");
    auto alert = static_cast<DeltaruneAlertLayer*>(unmodifiedAlert);
    
    if (characterPortrait) {
        auto imageNode = alert->createImageNode();
        imageNode->setCharacterImage(characterPortrait);
    }

    return alert;
}

FLAlertLayer* DeltaruneEvents::createDialogBox(cocos2d::CCNode* characterPortrait, std::string const& title, std::string const& text) {
    auto alert = createDialogBoxUnshown(characterPortrait, title, text);

    alert->show();
    
    return alert;
}

// Only here to help the events to work. I don't need this in a header file.
FLAlertLayer* createDialogWithVoiceUnshown(
    CCNode* characterPortrait,
    std::string const& voice,
    std::string const& title,
    std::string const& text
) {
    auto const& nameToChar = DeltaruneMaps::nameToCharacter;

    if (voice.size() != 0 && nameToChar.find(voice) == nameToChar.end()) {
        log::error("Character \"{}\" does not exist! Use \"\" for no sound.", voice);
        return nullptr;
    }

    auto unmodifiedAlert = createDialogBoxUnshown(characterPortrait, title, text);
    auto alert = static_cast<DeltaruneAlertLayer*>(unmodifiedAlert);

    if (voice.size() != 0)
        alert->setTextSound(nameToChar.at(voice));
    else
        alert->setTextSound(DeltaruneMaps::Character::NONE);
    
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
    auto alert = createDialogWithVoiceUnshown(characterPortrait, voice, title, text);
    
    if (alert)
        alert->show();

    if (output)
        *output = alert;
}