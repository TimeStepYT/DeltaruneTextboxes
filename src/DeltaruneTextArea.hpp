#pragma once

#include <Geode/Geode.hpp>

class DeltaruneTextArea {
    TextArea* m_textArea = nullptr;
public:
    static std::shared_ptr<DeltaruneTextArea> create(std::string const& str, std::string const& font, float width, float size);

    TextArea* getTextArea();
    std::vector<cocos2d::CCLabelBMFont*> getLines();
};