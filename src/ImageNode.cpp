#include "ImageNode.hpp"

ImageNode* ImageNode::create() {
    auto ret = new ImageNode();

    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    CC_SAFE_DELETE(ret);
    return nullptr;
}

bool ImageNode::init() {
    if (!CCNode::init())
        return false;

    // This is just meant to be an empty node that can be filled with anything you want :D
    // So other mods may support Deltarune Textboxes!
    
    this->setContentSize({ 100, 100 });
    this->setAnchorPoint({ 0.5f, 0.5f });

    return true;
}

void ImageNode::handleCharacterImage(CCSprite* newChar) {
    newChar->setPositionX(this->getContentWidth() * this->getAnchorPoint().x);
    newChar->setPositionY(this->getContentHeight() * this->getAnchorPoint().y);

    this->addChild(newChar);
}

void ImageNode::setCharacterImage(std::string_view spriteName) {
    CCSprite* newChar;

    if (Mod::get()->getSettingValue<bool>("coloredPortraits"))
        newChar = CCSprite::create(spriteName.data());
    else
        newChar = CCSpriteGrayscale::create(spriteName.data());

    handleCharacterImage(newChar);
}

void ImageNode::setCharacterImage(CCTexture2D* spriteTexture) {
    CCSprite* newChar;

    if (Mod::get()->getSettingValue<bool>("coloredPortraits"))
        newChar = CCSprite::createWithTexture(spriteTexture);
    else
        newChar = CCSpriteGrayscale::createWithTexture(spriteTexture);

    handleCharacterImage(newChar);
}