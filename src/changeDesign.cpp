#include <Geode/utils/cocos.hpp>
#include "FLAlertLayer.hpp"
#include "include.hpp"

template <class I, class T>
bool iteratorContains(I const& iterator, T const& value) {
    return std::find(iterator.begin(), iterator.end(), value) != iterator.end();
}

void capitalize(std::string& str) {
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
        c = toupper(c);
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

    bg->removeFromParent();

    if (undertaleBG)
        bg = CCScale9Sprite::create("undertaleSquare.png"_spr);
    else
        bg = CCScale9Sprite::create("deltaruneSquare_0.png"_spr);

    bg->setContentHeight(140);
    bg->setContentWidth(screenSize);
    bg->setPosition(CCPoint{CCDirector::sharedDirector()->getWinSize().width / 2, 70});
    bg->setID("background");
    bg->setZOrder(-1);

    if (!Mod::get()->getSettingValue<bool>("noAnimation") && !undertaleBG)
        schedule(schedule_selector(DeltaruneAlertLayer::animateBG), 1 / 5.f);

    if (m_fields->dialog) {
        imageNode->setZOrder(bg->getZOrder() + 1);
        imageNode->setPosition({bg->getPositionX() - screenSize / 2 + 68, bg->getPositionY()});
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

// fix handleTouchPrio breaking the buttons
void DeltaruneAlertLayer::fixTouchPrio() {
    auto const buttonMenu = this->m_buttonMenu;

    int const parentTouchPrio = this->getTouchPriority();
    int const menuTouchPrio = buttonMenu->getTouchPriority();

    if (parentTouchPrio >= menuTouchPrio) {
        buttonMenu->setTouchPriority(parentTouchPrio + 1);
    }
}

void DeltaruneAlertLayer::changeButtons() {
    if (!m_buttonMenu) return;
    m_buttonMenu->setPositionY(32);
    m_buttonMenu->setVisible(false);

    if (!m_button2) return;
    auto const bg = m_fields->bg;
    float const positionStart = bg->getPositionX() - m_buttonMenu->getPositionX() - m_fields->screenSize / 2;
    m_fields->btn1->setPositionX(positionStart + m_fields->screenSize / 4);
    m_fields->btn2->setPositionX(positionStart + (m_fields->screenSize / 4) * 3);

    auto const heart = CCSprite::create("heart.png"_spr);
    heart->setVisible(false);
    heart->setPositionY(m_buttonMenu->getPositionY());
    heart->setPositionX(m_buttonMenu->getPositionX());
    heart->setID("heart"_spr);
    m_mainLayer->addChild(heart);
    m_fields->heart = heart;

    changeSingleButton(m_fields->btn1, m_button1);
    changeSingleButton(m_fields->btn2, m_button2);

    Loader::get()->queueInMainThread([popup = Ref(this)] {
        Loader::get()->queueInMainThread([popup] {
            popup->fixTouchPrio();
        });
    });
}

void DeltaruneAlertLayer::changeTitle() {
    auto const title = m_fields->title;
    auto const bg = m_fields->bg;
    title->setAnchorPoint(CCPoint{0, 0});
    title->setFntFile("Determination.fnt"_spr);
    title->setPosition(CCPoint{bg->getPositionX() - bg->getContentWidth() / 2 + 24, 138});
}

void DeltaruneAlertLayer::handleSound() {
    auto&& nameToSound = m_fields->nameToSound;
    auto const titleString = m_fields->title->getString();
    auto& sound = m_fields->textSound;

    if (nameToSound.find(titleString) != nameToSound.end())
        sound = nameToSound[titleString];
    else if (m_fields->dialog)
        sound = "Default";

    initSoundRate();
}

void DeltaruneAlertLayer::changeText() {
    auto& textArea = m_fields->textArea;
    if (!textArea) return;

    auto const& sound = m_fields->textSound;
    auto const size = m_fields->textSize;
    bool& noShadow = m_fields->noShadow;

    noShadow = Mod::get()->getSettingValue<bool>("noShadow") || sound == "Sans" || sound == "Papyrus";

    CCLabelBMFont* const star = CCLabelBMFont::create("*", "Determination.fnt"_spr);
    CCLabelBMFont* starShadow = nullptr;
    if (!noShadow) starShadow = CCLabelBMFont::create("*", "Determination.fnt"_spr);

    auto& str = m_fields->text;
    auto const screenSize = m_fields->screenSize;
    auto const bg = m_fields->bg;

    int xOffset = star->getContentWidth();

    DeltaruneAlertLayer::handleSound();

    if (m_fields->dialog)
        xOffset = m_fields->imageNode->getContentWidth() + star->getContentWidth();

    star->setPositionX(bg->getPositionX() - screenSize / 2 + xOffset - star->getContentWidth() + 27);
    star->setPositionY(110);
    star->setZOrder(1);
    star->setID("star"_spr);

    if (!noShadow) {
        starShadow->setPositionX(bg->getPositionX() - screenSize / 2 + xOffset - star->getContentWidth() + 28);
        starShadow->setPositionY(109);
        starShadow->setZOrder(0);
        starShadow->setID("starShadow"_spr);
        auto const character = starShadow->getChildByType<CCSprite>(0);
        character->setColor({15, 15, 127});
    }

    std::string font = "Determination.fnt"_spr;

    if (sound == "Sans") font = "ComicSans.fnt"_spr;
    if (sound == "Papyrus") {
        font = "Papyrus.fnt"_spr;
        capitalize(str);
    }

    float creatingWidth = screenSize - 100 - xOffset;
    auto const newDesc = TextArea::create(
        str,
        font.c_str(),
        1,
        creatingWidth,
        CCPoint{0, 1},
        size,
        false);

    newDesc->setContentWidth(creatingWidth);
    newDesc->setAnchorPoint(CCPoint{0, 1});
    newDesc->setPositionY(110);
    newDesc->setZOrder(textArea->getZOrder());
    newDesc->setID("content-text-area");

    TextArea* newDescGrad = nullptr;
    TextArea* newDescShad = nullptr;
    if (!Mod::get()->getSettingValue<bool>("noGradientOverlay") && sound != "Sans" && sound != "Papyrus") {
        newDescGrad = TextArea::create(
            str,
            "DeterminationGradient.fnt"_spr,
            1,
            creatingWidth,
            CCPoint{0, 1},
            size,
            false);
        newDescGrad->setContentWidth(creatingWidth);
        newDescGrad->setAnchorPoint(CCPoint{0, 1});
        newDescGrad->setPositionY(110);
        newDescGrad->setZOrder(textArea->getZOrder() + 1);
        newDescGrad->setID("gradient-overlay"_spr);

        auto const& linesGrad = newDescGrad->getChildByType<MultilineBitmapFont>(0)->getChildrenExt();
        for (auto const line : linesGrad) {
            auto const& letters = line->getChildrenExt<CCSprite>();
            for (auto const& letter : letters) {
                letter->setColor(ccColor3B{255, 255, 255});
                letter->setVisible(false);
            }
        }
    }
    if (!noShadow) {
        newDescShad = TextArea::create(
            str,
            font.c_str(),
            1.f,
            creatingWidth,
            CCPoint{0, 1},
            size,
            false);
        newDescShad->setContentWidth(creatingWidth);
        newDescShad->setAnchorPoint(CCPoint{0, 1});
        newDescShad->setPositionY(109);
        newDescShad->setPositionX(newDescShad->getPositionX() + 1);
        newDescShad->setZOrder(textArea->getZOrder() - 1);
        newDescShad->setID("shadow"_spr);

        auto const& linesShad = newDescShad->getChildByType<MultilineBitmapFont>(0)->getChildrenExt();
        int i = 0;

        for (auto const line : linesShad) {
            auto const& letters = line->getChildrenExt<CCSprite>();
            int j = 0;

            for (auto const letter : letters) {
                auto const origLinesParent = newDesc->getChildByType<MultilineBitmapFont>(0);
                auto const origLine = origLinesParent->getChildByType<CCLabelBMFont>(i);
                auto const origChar = origLine->getChildByType<CCSprite>(j);
                auto const& color = origChar->getColor();

                if (color == ccColor3B{255, 255, 255})
                    letter->setColor({15, 15, 127});
                else if (color == ccColor3B{255, 0, 255})
                    letter->setColor({76, 0, 76});
                else if (color == ccColor3B{255, 90, 90})
                    letter->setColor({76, 0, 0});
                else if (color == ccColor3B{255, 165, 75})
                    letter->setColor({76, 38, 18});
                else if (color == ccColor3B{255, 255, 0})
                    letter->setColor({76, 76, 0});
                else if (color == ccColor3B{64, 227, 72})
                    letter->setColor({0, 76, 0});
                else if (color == ccColor3B{74, 82, 255})
                    letter->setColor({0, 0, 76});
                else {
                    uint8_t const red = color.r / 2;
                    uint8_t const green = color.g / 2;
                    uint8_t const blue = color.b / 2;
                    letter->setColor(ccColor3B{red, green, blue});
                }

                letter->setVisible(false);
                j++;
            }
            i++;
        }
    }
    auto const& lines = newDesc->getChildByType<MultilineBitmapFont>(0)->getChildrenExt();

    for (auto const line : lines) {
        auto const& letters = line->getChildrenExt();
        for (auto const letter : letters) {
            letter->setVisible(false);
        }
    }
    textArea->removeFromParent();
    m_mainLayer->addChild(star);

    if (!noShadow) m_mainLayer->addChild(starShadow);

    auto const rect = CCLayerColor::create({0, 0, 0, 0}, bg->getContentWidth(), bg->getContentHeight() - 20);
    auto const clippingNode = CCClippingNode::create(rect);
    clippingNode->setID("content-text-area"_spr);
    clippingNode->setPositionY(10);
    clippingNode->setPositionX(bg->getPositionX() - screenSize / 2 + 24 + xOffset);
    clippingNode->addChild(newDesc);
    if (newDescGrad) clippingNode->addChild(newDescGrad);
    if (!noShadow) clippingNode->addChild(newDescShad);

    m_mainLayer->addChild(clippingNode);

    m_fields->textAreaClippingNode = clippingNode;
    textArea = newDesc;
    m_fields->gradientOverlay = newDescGrad;
    m_fields->shadow = newDescShad;

    double const pause = Mod::get()->getSettingValue<double>("textRollingPause");

    m_fields->linesProgressed += emptyLinesAmount();
    textArea->setPositionY(textArea->getPositionY() + m_fields->textSize * m_fields->linesProgressed);
    if (newDescGrad) newDescGrad->setPositionY(textArea->getPositionY());
    if (newDescShad) newDescShad->setPositionY(textArea->getPositionY() - 1);

    schedule(schedule_selector(DeltaruneAlertLayer::rollText), pause / 30);
}

void DeltaruneAlertLayer::removeControllerGlyphs() {
    auto const mainLayer = this->m_mainLayer;
    auto const backControllerGlyph = mainLayer->getChildByID("controller-back-hint");
    auto const okControllerGlyph = mainLayer->getChildByID("controller-ok-hint");

    if (backControllerGlyph)
        backControllerGlyph->removeFromParentAndCleanup(true);
    if (okControllerGlyph)
        okControllerGlyph->removeFromParentAndCleanup(true);
}

void DeltaruneAlertLayer::changeLook() {
    removeControllerGlyphs();
    changeBG();
    changeButtons();
    changeTitle();
    changeText();
}