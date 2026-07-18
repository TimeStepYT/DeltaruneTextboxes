#include "DeltaruneTextArea.hpp"

std::shared_ptr<DeltaruneTextArea> DeltaruneTextArea::create(std::string const& str, std::string const& font, float width, float size) {
    auto instance = std::make_shared<DeltaruneTextArea>();
    
    TextArea* newTextArea = TextArea::create(str, font.c_str(), 1, width, {0.f, 1.f}, size, false);
    newTextArea->setAnchorPoint({0.f, 1.f});
    newTextArea->setContentWidth(width);

    instance->m_textArea = newTextArea;

    return instance;
}

TextArea* DeltaruneTextArea::getTextArea() {
    return this->m_textArea;
}

std::vector<cocos2d::CCLabelBMFont*> DeltaruneTextArea::getLines() {
    auto parent = this->getTextArea()->getChildByType<MultilineBitmapFont>(0);
    auto const children = parent->getChildrenExt();

    std::vector<cocos2d::CCLabelBMFont*> res;
    res.reserve(children.size());
    for (auto child : children) {
        res.push_back(static_cast<cocos2d::CCLabelBMFont*>(child));
    }
    
    return res;
}