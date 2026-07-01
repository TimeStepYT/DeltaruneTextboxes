#pragma once

#include <Geode/Geode.hpp>
#include <Geode/loader/Dispatch.hpp>

namespace DeltaruneEvents {
    inline geode::Dispatch<cocos2d::CCNode*, std::string const&, std::string const&> createDialogEvent("create-dialog-box"_spr);

    void createDialogBox(cocos2d::CCNode* characterPortrait, std::string const& title, std::string const& text);
}