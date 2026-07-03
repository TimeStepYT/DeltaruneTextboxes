#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Dispatch.hpp>

class DeltaruneDialogObject {
public:
    cocos2d::CCNode* m_characterSprite = nullptr;
    std::shared_ptr<std::string> m_voice = nullptr;
    std::shared_ptr<std::string> m_title = nullptr;
    std::shared_ptr<std::string> m_text = nullptr;
    
    DeltaruneDialogObject(cocos2d::CCNode* characterSprite, std::string const& voice, std::string const& title, std::string const& text);
    DeltaruneDialogObject(DeltaruneDialogObject const&) = default;
};

namespace DeltaruneEvents {
    inline geode::Dispatch<cocos2d::CCNode*, std::string const&, std::string const&>
        createDialogEvent("create-dialog-box"_spr);
    inline geode::Dispatch<FLAlertLayer**, cocos2d::CCNode*, std::string const&, std::string const&>
        createDialogReturnEvent("create-dialog-box-return"_spr);
    inline geode::Dispatch<FLAlertLayer**, cocos2d::CCNode*, std::string const&, std::string const&, std::string const&>
        createDialogWithVoiceEvent("create-dialog-with-voice"_spr);

    class CreateDialogFull : public geode::Event<CreateDialogFull, bool(FLAlertLayer**, std::vector<DeltaruneDialogObject> const&)> {
    public:
        using Event::Event;
    };

    // Creates a dialog box with a custom node where the character sprite would be
    FLAlertLayer* createDialogBox(cocos2d::CCNode* characterPortrait, std::string const& title, std::string const& text);
    
    // Like DeataruneAlertLayer::createDialogBox but it stores it in the output pointer
    FLAlertLayer* createDialogBoxReturn(FLAlertLayer** output, cocos2d::CCNode* characterPortrait, std::string const& title, std::string const& text);
    
    // Creates a dialog box with a given voice (like "Sans") and a given node as the character portrait.
    // It stores the alert in the output pointer.
    void createDialogWithVoice(
        FLAlertLayer** output,
        cocos2d::CCNode* characterPortrait,
        std::string const& voice,
        std::string const& title,
        std::string const& text
    );
    
    // Creates a dialog box with the possibility of multiple characters speaking
    void createDialogFull(FLAlertLayer** output, std::vector<DeltaruneDialogObject> const&);
}