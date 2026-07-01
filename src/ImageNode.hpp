#pragma once

class ImageNode : public cocos2d::CCNode {
public:
    static ImageNode* create();
    
    bool init();
    void handleCharacterImage(cocos2d::CCNode*);
	void setCharacterImage(std::string_view);
	void setCharacterImage(cocos2d::CCTexture2D*);
	void setCharacterImage(cocos2d::CCNode*);
};