#include <Geode/utils/cocos.hpp>
#include <Geode/ui/Button.hpp>
#include <alphalaneous.alphas-ui-pack/include/API.hpp>
#include "hooks/FLAlertLayer.hpp"
#include "TextShaders.hpp"
#include "Global.hpp"

using namespace geode::prelude;

template <class I, class T>
bool iteratorContains(I const& iterator, T const& value) {
    return std::find(iterator.begin(), iterator.end(), value) != iterator.end();
}

void capitalize(std::string& str, bool toUpper = true) {
    bool disable = false;
    std::array<char, 5> constexpr nextFilter = {'c', 'd', '/', 'i', 's'};

    for (auto& c : str) {
        char* const next = (&c + sizeof(c));
        if (c == '<') {
            if (iteratorContains(nextFilter, *next)) {
                disable = true;
                continue;
            }
        } else if (c == '>') {
            disable = false;
            continue;
        }
        if (disable) continue;
        
        if (toUpper)
            c = toupper(c);
        else
            c = tolower(c);
    }
}

void DeltaruneAlertLayer::animateBG(float dt) {
    auto const bg = m_fields->bg;
    int& frame = m_fields->frame;
    bool& reverseAnim = m_fields->reverseAnim;

    std::string const& spriteName = fmt::format("deltaruneSquare_{}.png"_spr, frame);
    
    if (reverseAnim)
        frame--;
    else
        frame++;
    if (frame == 0 || frame == 4)
        reverseAnim = !reverseAnim;

    auto const sprite = CCSprite::create(spriteName.c_str());
    auto const prevContentSize = bg->getContentSize();

    bg->setSpriteFrame(sprite->displayFrame());
    bg->setContentSize(prevContentSize);
}

void DeltaruneAlertLayer::changeBG() {
    auto& bg = m_fields->bg;
    auto const screenSize = m_fields->screenSize;
    auto const imageNode = m_fields->imageNode;
    auto const undertaleBG = Mod::get()->getSettingValue<bool>("undertaleBG");
    auto* const director = CCDirector::sharedDirector();

    bg->removeFromParent();

    if (undertaleBG)
        bg = CCScale9Sprite::create("undertaleSquare.png"_spr);
    else
        bg = CCScale9Sprite::create("deltaruneSquare_0.png"_spr);


    bg->setContentSize({1334.f / 3, 376.f / 3}); // Measured from the game
    bg->setPosition(CCPoint{screenSize / 2, bg->getContentHeight() / 2});
    bg->setID("background");
    bg->setZOrder(-1);

    if (!Mod::get()->getSettingValue<bool>("noAnimation") && !undertaleBG)
        schedule(schedule_selector(DeltaruneAlertLayer::animateBG), 1 / 5.f);

    if (m_fields->imageNode) {
        imageNode->setZOrder(bg->getZOrder() + 1);
        imageNode->setPosition({bg->getPositionX() - bg->getContentWidth() / 2 + 68, bg->getPositionY()});
    }
    m_mainLayer->addChild(bg);
}

void DeltaruneAlertLayer::changeSingleButton(CCMenuItemSpriteExtra* btn, ButtonSprite* buttonSprite) {
    btn->m_animationEnabled = false;

    auto const buttonTexture = buttonSprite->getChildByType<CCScale9Sprite>(0);
    if (buttonTexture)
        buttonTexture->setVisible(false);

    auto const label = buttonSprite->getChildByType<CCLabelBMFont>(0);
    if (label) {
        label->setFntFile("Determination.fnt"_spr);
        label->setScale(1.f);
    }
}

void DeltaruneAlertLayer::changeButtons() {
    if (!m_buttonMenu) return;

    auto* const fields = this->m_fields.self();
    auto* const bg = fields->bg;
    
    m_buttonMenu->setPositionX(bg->getContentWidth() / 2);
    m_buttonMenu->setPositionY(32);
    m_buttonMenu->setVisible(false);

    if (!m_button2) return;
    float const positionStart = bg->getPositionX() - m_buttonMenu->getPositionX() - fields->screenSize / 2;
    Button* newBtn1 = Button::createWithLabel(this->m_button1->m_caption, "Determination.fnt"_spr);
    newBtn1->setPositionX(positionStart + bg->getContentWidth() / 4);
    newBtn1->setPositionY(fields->old_btn1->getPositionY() + 2);
    newBtn1->setAnimationType(Button::AnimationType::None);
    newBtn1->setTouchPriority(-1000);
    newBtn1->setActivateCallback([this](Button* button) {
        FLAlertLayer::onBtn1(button);
    });

    Button* newBtn2 = Button::createWithLabel(this->m_button2->m_caption, "Determination.fnt"_spr);
    newBtn2->setPositionX(positionStart + (bg->getContentWidth() / 4) * 3);
    newBtn2->setPositionY(fields->old_btn2->getPositionY() + 2);
    newBtn2->setAnimationType(Button::AnimationType::None);
    newBtn2->setTouchPriority(-1000);
    newBtn2->setActivateCallback([this](Button* button) {
        FLAlertLayer::onBtn2(button);
    });

    newBtn1->setSelectCallback([this, newBtn2](Button* button){
        this->clickedOnButton(button, newBtn2, 1);
    });
    newBtn2->setSelectCallback([this, newBtn1](Button* button){
        this->clickedOnButton(button, newBtn1, 2);
    });

    this->m_buttonMenu->addChild(newBtn1);
    this->m_buttonMenu->addChild(newBtn2);

    fields->btn1 = newBtn1;
    fields->btn2 = newBtn2;

    fields->old_btn1->removeFromParent();
    fields->old_btn2->removeFromParent();

    auto const heart = CCSprite::create("heart.png"_spr);
    heart->setVisible(false);
    heart->setPositionY(m_buttonMenu->getPositionY());
    heart->setPositionX(fields->screenSize / 2);
    heart->setID("heart"_spr);
    m_mainLayer->addChild(heart);
    fields->heart = heart;

    changeSingleButton(fields->old_btn1, m_button1);
    changeSingleButton(fields->old_btn2, m_button2);
}

void DeltaruneAlertLayer::changeTitle() {
    auto const title = m_fields->title;
    auto const bg = m_fields->bg;
    title->setAnchorPoint(CCPoint{0, 0});
    title->setFntFile("Determination.fnt"_spr);
    title->setPosition(CCPoint{bg->getPositionX() - bg->getContentWidth() / 2 + 24, bg->getContentHeight() - 2.f});
}

void DeltaruneAlertLayer::handleSound() {
    auto const& titleToChar = DeltaruneMaps::titleToCharacter;
    auto const titleString = m_fields->title->getString();
    auto& sound = m_fields->textSound;

    if (titleToChar.find(titleString) != titleToChar.end())
        sound = titleToChar.at(titleString);
    else if (m_fields->dialog)
        sound = DeltaruneMaps::Character::DEFAULT;

    initSoundRate();
}

CCLabelBMFont* DeltaruneAlertLayer::createStar() {
    auto const fields = m_fields.self();
    bool const noShadow = fields->noShadow;
    bool const noGradient = fields->noGradient;
    float const screenSize = fields->screenSize;
    auto const bg = fields->bg;

    CCLabelBMFont* const star = CCLabelBMFont::create("*", "Determination.fnt"_spr);

    float xOffset = star->getContentWidth();

    if (fields->imageNode)
        xOffset = fields->imageNode->getContentWidth() + star->getContentWidth();

    star->setPositionX(xOffset - star->getContentWidth() + 27);
    star->setPositionY(bg->getContentHeight() - 30.f);
    star->setZOrder(1);
    star->setID("star"_spr);

    fields->contentXOffset = xOffset;
    
    return star;
}

void DeltaruneAlertLayer::changeText() {
    auto const fields = m_fields.self();
    auto& oldTextArea = fields->old_textArea;
    if (!oldTextArea) return;

    auto const& sound = fields->textSound;
    auto const size = fields->textSize;
    bool& noShadow = fields->noShadow;
    bool& noGradient = fields->noGradient;

    noShadow = Mod::get()->getSettingValue<bool>("noShadow");
    noGradient = Mod::get()->getSettingValue<bool>("noGradientOverlay");

    
    auto& str = fields->text;
    auto const screenSize = fields->screenSize;
    auto const bg = fields->bg;
    
    
    DeltaruneAlertLayer::handleSound();
    
    std::string font = "Determination.fnt"_spr;
    
    if (sound == DeltaruneMaps::Character::SANS) {
        font = "ComicSans.fnt"_spr;
        capitalize(str, false);
    }
    else if (sound == DeltaruneMaps::Character::PAPYRUS) {
        font = "Papyrus.fnt"_spr;
        capitalize(str);
    }
    
    auto const star = this->createStar();

    float xOffset = fields->contentXOffset;
    float creatingWidth = bg->getContentWidth() - 120 - xOffset;
    auto const deltaruneTextArea = DeltaruneTextArea::create(
        str,
        font,
        creatingWidth,
        size
    );
    auto const newDesc = deltaruneTextArea->getTextArea();
    
    newDesc->setPositionX(24 + xOffset + star->getContentWidth());
    newDesc->setPositionY(bg->getContentHeight() - 20.f);
    newDesc->setZOrder(oldTextArea->getZOrder());
    newDesc->setID("content-text-area");

    auto const& lines = deltaruneTextArea->getLines();

    for (auto const line : lines) {
        auto const& letters = line->getChildrenExt();
        for (auto const letter : letters) {
            letter->setVisible(false);
        }
    }
    oldTextArea->removeFromParent();
    
    auto const rect = CCLayerColor::create(
        {0, 0, 0, 0},
        0, 0
    );
    
    auto const clippingNode = CCClippingNode::create(rect);
    clippingNode->setID("getoutijustneedyouforrendernodeandthebuttons"_spr);
    clippingNode->setZOrder(bg->getZOrder() - 1);
    clippingNode->setContentSize(rect->getContentSize());
    
    auto const textContentNode = CCNode::create();
    textContentNode->setID("text-content"_spr);
    textContentNode->setContentSize(bg->getContentSize());
    textContentNode->setPosition(bg->getPosition());
    textContentNode->setAnchorPoint({0.5f, 0.5f});
    fields->textContentNode = textContentNode;
    
    textContentNode->addChild(star);
    textContentNode->addChild(newDesc);
    
    this->m_buttonMenu->removeFromParentAndCleanup(false);
    
    textContentNode->addChild(this->m_buttonMenu);
    
    clippingNode->addChild(textContentNode);
    this->m_mainLayer->addChild(clippingNode);

    auto renderNode = alpha::ui::RenderNode::create(textContentNode, true);
    renderNode->setPosition(bg->getPosition());
    renderNode->setZOrder(bg->getZOrder() + 1);
    fields->renderedSprite = renderNode;

    auto shaderInstance = DeltaruneTextShaders::create(bg);
    auto program = shaderInstance.getShader(noShadow, noGradient);

    if (program)
        renderNode->setShaderProgram(program);

    m_mainLayer->addChild(renderNode);

    m_fields->m_textArea = std::move(deltaruneTextArea);
    /*
    if (newDescGrad) clippingNode->addChild(newDescGrad);
    fields->gradientOverlay = newDescGrad;
    */

    double const pause = Mod::get()->getSettingValue<double>("textRollingPause");

    fields->linesProgressed += emptyLinesAmount();
    newDesc->setPositionY(newDesc->getPositionY() + fields->textSize * fields->linesProgressed);
    /*
    if (newDescGrad) newDescGrad->setPositionY(newDesc->getPositionY());
    */

    schedule(schedule_selector(DeltaruneAlertLayer::rollText), pause / 30);
}

void DeltaruneAlertLayer::changeLook() {
    changeBG();
    changeButtons();
    changeTitle();
    changeText();
}