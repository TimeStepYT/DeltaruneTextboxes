#define GEODE_DEFINE_EVENT_EXPORTS
#include "../api/API.hpp"

#include "Events.hpp"
#include "hooks/FLAlertLayer.hpp"
#include "DeltaruneDialogObject.hpp"

using namespace geode::prelude;

namespace deltarune_textboxes {
    geode::Result<std::shared_ptr<DeltaruneDialogObject>> createDialogObject(
        cocos2d::CCNode* characterPortrait, 
        std::string const& voice, 
        std::string const& title, 
        std::string const& text
    ) {
        auto res = std::make_shared<DeltaruneDialogObject>(characterPortrait, voice, title, text);
        return geode::Ok(std::move(res));
    }

    geode::Result<FLAlertLayer*> createFullDialog(std::span<std::shared_ptr<DeltaruneDialogObject>> const& objects) {
        if (objects.size() == 0)
            return geode::Err("No objects in the container");

        auto const& firstObject = objects[0];
        
        FLAlertLayer* unmodifiedAlert = nullptr;
        DeltaruneEvents::createDialogWithVoice(
            &unmodifiedAlert,
            firstObject->m_characterSprite,
            firstObject->m_voice,
            firstObject->m_title,
            firstObject->m_text
        );

        auto alert = static_cast<DeltaruneAlertLayer*>(unmodifiedAlert);

        alert->setNextAlerts({objects.begin() + 1, objects.end()});
        return geode::Ok(unmodifiedAlert);
    }
}