#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Dispatch.hpp>
#include <Geode/binding/FLAlertLayer.hpp>

#undef MY_MOD_ID
#define MY_MOD_ID "timestepyt.deltarune_textboxes"

namespace deltarune_textboxes {
    class DeltaruneDialogObject;

    using DialogObjectPtr = std::shared_ptr<DeltaruneDialogObject>;

    inline geode::Result<DialogObjectPtr> createDialogObject(
        cocos2d::CCNode* characterPortrait, 
        std::string const& voice, 
        std::string const& title, 
        std::string const& text
    )
    GEODE_EVENT_EXPORT(&createDialogObject, (characterPortrait, voice, title, text));
    
    inline geode::Result<FLAlertLayer*> createFullDialog(std::span<DialogObjectPtr> const& objects) 
    GEODE_EVENT_EXPORT(&createFullDialog, (objects));
}
