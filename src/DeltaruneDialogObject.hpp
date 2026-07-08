#pragma once

#include <Geode/Geode.hpp>

namespace deltarune_textboxes {
    class DeltaruneDialogObject {
    public:
        geode::Ref<cocos2d::CCNode> m_characterSprite = nullptr;
        std::string m_voice;
        std::string m_title;
        std::string m_text;

        DeltaruneDialogObject(cocos2d::CCNode* characterSprite, std::string voice, std::string title, std::string text) :
            m_characterSprite(characterSprite), 
            m_voice(std::move(voice)), 
            m_title(std::move(title)),
            m_text(std::move(text)) {}
    };
}