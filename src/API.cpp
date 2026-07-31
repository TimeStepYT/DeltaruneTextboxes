#define GEODE_DEFINE_EVENT_EXPORTS
#include "../api/API.hpp"

#include "Global.hpp"
#include "Events.hpp"
#include "hooks/FLAlertLayer.hpp"
#include "DeltaruneDialogObject.hpp"

using namespace geode::prelude;

// This function only exists because of the boilerplate stuff that I don't want to repeat
static DialogObjectPtr newDialogObject(CCNode* characterPortrait, std::string const& voice, std::string const& title, std::string const& text) {
    DialogObjectPtr res(
        new DeltaruneDialogObject(characterPortrait, voice, title, text),
        +[](DeltaruneDialogObject* self) {
            delete self;
        }
    );
    return res;
}

namespace deltarune_textboxes {
    geode::Result<DialogObjectPtr> createDialogObject(
        cocos2d::CCNode* characterPortrait, 
        std::string const& voice, 
        std::string const& title, 
        std::string const& text
    ) {
        auto res = newDialogObject(characterPortrait, voice, title, text);
        return geode::Ok(std::move(res));
    }

    geode::Result<bool> registerSound(std::string const& name, std::filesystem::path const& file, int soundRate, bool hasPitchVariation) {
        if (DeltaruneMaps::externalNameToData.find(name) != DeltaruneMaps::externalNameToData.end()) {
            return geode::Err("Sound \"{}\" is already registered!", name);
        }
        auto newData = DeltaruneMaps::CharacterData::create(geode::utils::string::pathToString(file), soundRate, hasPitchVariation);
        newData.setExternal();
        DeltaruneMaps::externalNameToData[name] = newData;
        return geode::Ok(true);
    }

    geode::Result<bool> registerVaryingSound(std::string const& name, std::span<std::filesystem::path> files, int soundRate, bool hasPitchVariation) {
        if (DeltaruneMaps::externalNameToData.find(name) != DeltaruneMaps::externalNameToData.end()) {
            return geode::Err("Sound \"{}\" is already registered!", name);
        }

        std::vector<std::string> res;
        res.reserve(files.size());

        for (auto const& file : files) {
            res.emplace_back(string::pathToString(file));
        }

        DeltaruneMaps::CharacterData newData {res, soundRate, hasPitchVariation};
        newData.setExternal();

        DeltaruneMaps::externalNameToData[name] = newData;
        return geode::Ok(true);
    }


    geode::Result<FLAlertLayer*> createFullDialog(std::span<DialogObjectPtr> objects) {
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

        if (!unmodifiedAlert)
            return geode::Err("Initial alert couldn't be created");

        auto alert = static_cast<DeltaruneAlertLayer*>(unmodifiedAlert);

        alert->setNextAlerts({objects.begin() + 1, objects.end()});
        return geode::Ok(unmodifiedAlert);
    }

    geode::Result<float> getCharacterSpriteScaleFrom(CCNode *characterSpriteNode) {
        if (!characterSpriteNode)
            return geode::Err("Character sprite node is nullptr");

        float constexpr imageNodeSize = 100.f;
        float const nodeWidth = characterSpriteNode->getContentWidth();
        float const nodeHeight = characterSpriteNode->getContentHeight();

        if (nodeHeight > nodeWidth) {
            float const res = imageNodeSize / nodeHeight;
            return geode::Ok(res);
        }
        else {
            float const res = imageNodeSize / nodeWidth;
            return geode::Ok(res);
        }
    }

    geode::Result<DialogObjectPtr> copyDialogObject(DeltaruneDialogObject const& object) {
        auto copied = newDialogObject(object.m_characterSprite, object.m_voice, object.m_title, object.m_text);
        return geode::Ok(std::move(copied));
    }

    geode::Result<bool> lockTextSound(std::string const& textSound) {
        auto const& n2c = DeltaruneMaps::nameToCharacter;

        bool soundExistsHere = n2c.find(textSound) != n2c.end();
        
        if (!soundExistsHere) {
            auto const& en2d = DeltaruneMaps::externalNameToData;
            bool soundExistsExternally = en2d.find(textSound) != en2d.end();

            if (soundExistsExternally) {
                global::lockedTextSound = en2d.at(textSound);
                return geode::Ok(true);
            }
            return geode::Err(fmt::format("The sound \"{}\" is not registered", textSound));
        }

        global::lockedTextSound = DeltaruneMaps::characterToData.at(n2c.at(textSound));

        return geode::Ok(true);
    }

    void unlockTextSound() {
        global::lockedTextSound.reset();
    }
}