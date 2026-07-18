#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <Geode/binding/FLAlertLayer.hpp>

#undef MY_MOD_ID
#define MY_MOD_ID "timestepyt.deltarune_textboxes"

namespace deltarune_textboxes {
    class DeltaruneDialogObject;

    // unique_ptr for PIMPL is weird...
    using DialogObjectPtr = std::unique_ptr<DeltaruneDialogObject, void(*)(DeltaruneDialogObject*)>;

    inline geode::Result<DialogObjectPtr> createDialogObject(
        cocos2d::CCNode* characterPortrait, 
        std::string const& voice, 
        std::string const& title, 
        std::string const& text
    )
    GEODE_EVENT_EXPORT(&createDialogObject, (characterPortrait, voice, title, text));
    
    // Lets you register you own basic text sound. Please use _spr for the name!
    inline geode::Result<bool> registerSound(std::string const& name, std::filesystem::path const& file, int soundRate = 3, bool hasPitchVariation = false)
    GEODE_EVENT_EXPORT(&registerSound, (name, file, soundRate, hasPitchVariation));
    
    inline geode::Result<bool> registerVaryingSound(std::string const& name, std::span<std::filesystem::path> files, int soundRate = 3, bool hasPitchVariation = false)
    GEODE_EVENT_EXPORT(&registerVaryingSound, (name, files, soundRate, hasPitchVariation));

    inline geode::Result<DialogObjectPtr> copyDialogObject(DeltaruneDialogObject const& object)
    GEODE_EVENT_EXPORT(&copyDialogObject, (object));
    
    // As a heads up, this will call FLAlertLayer::show() for you. Do not call show() yourself unless you want a broken double textbox
    inline geode::Result<FLAlertLayer*> createFullDialog(std::span<DialogObjectPtr> objects) 
    GEODE_EVENT_EXPORT(&createFullDialog, (objects));
}
