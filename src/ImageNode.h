#pragma once

#include "include.h"

class ImageNode : public cocos2d::CCNode {
public:
    static ImageNode* create();
    
    bool init();
    void handleCharacterImage(CCSprite*);
	void setCharacterImage(std::string_view);
	void setCharacterImage(CCTexture2D*);
};