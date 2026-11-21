#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/EventExport.hpp>
#include <Geode/binding/FLAlertLayer.hpp>

#undef MY_MOD_ID
#define MY_MOD_ID "timestepyt.deltarune_textboxes"

namespace deltarune_textboxes {
    class DeltaruneDialogObject;

    // unique_ptr for PIMPL is weird...
    using DialogObjectPtr = std::unique_ptr<DeltaruneDialogObject, void(*)(DeltaruneDialogObject*)>;

    GEODE_EVENT_EXPORT(geode::Result<DialogObjectPtr>, createDialogObject, (
        cocos2d::CCNode* characterPortrait, 
        std::string const& voice, 
        std::string const& title, 
        std::string const& text
    ), (characterPortrait, voice, title, text));
    
    // Lets you register you own basic text sound. Please use _spr for the name!
    GEODE_EVENT_EXPORT(geode::Result<bool>, registerSound, (
        std::string const& name,
        asp::fs::Path const& file,
        int soundRate, 
        bool hasPitchVariation
    ), (name, file, soundRate = 3, hasPitchVariation = false));
    
    GEODE_EVENT_EXPORT(geode::Result<bool>, registerVaryingSound, (
        std::string const& name, 
        std::span<asp::fs::Path> files, 
        int soundRate, 
        bool hasPitchVariation 
    ), (name, files, soundRate = 3, hasPitchVariation = false));

    GEODE_EVENT_EXPORT(geode::Result<DialogObjectPtr>, copyDialogObject, (DeltaruneDialogObject const& object), (object));

    // If your node is too big (or small) then use this function to get the proper scale to make it fit
    GEODE_EVENT_EXPORT(geode::Result<float>, getCharacterSpriteScaleFrom, (cocos2d::CCNode* characterSpriteNode), (characterSpriteNode))
    
    // As a heads up, this will call FLAlertLayer::show() for you. Do not call show() yourself unless you want a broken double textbox
    GEODE_EVENT_EXPORT(geode::Result<FLAlertLayer*>, createFullDialog, (std::span<DialogObjectPtr> objects), (objects));

    // Locks all text boxes to one text sound. Please use unlockTextSound() once you're done... please
    GEODE_EVENT_EXPORT(geode::Result<bool>, lockTextSound, (std::string const& textSound), (textSound));
    
    GEODE_EVENT_EXPORT(void, unlockTextSound, (), ());
}
