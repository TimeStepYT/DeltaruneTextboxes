#include <Geode/utils/cocos.hpp>
#include "FLAlertLayer.hpp"

#include "DialogLayer.hpp"
#include "ImageNode.hpp"

bool blockKeys = false;

static std::mt19937 mt{std::random_device{}()};

// thanks cvolton for this amazing name
float randomNumberInAGivenRangeThatGetsAddedOrRemovedFromADifferentNumber(float const range) {
    auto const randomNumber = mt();

    float const halfRange = range / 2.f;
    int const iRange = range * 100;

    int const randomInRange = randomNumber % iRange;
    float const fRandom = randomInRange / 100.f;
    float const result = fRandom - halfRange;

    return result;
}

void DeltaruneAlertLayer::initMaps() {
    auto&& nameToFile = m_fields->nameToFile;
    auto&& nameToSound = m_fields->nameToSound;
    auto&& nameToSoundRate = m_fields->nameToSoundRate;

    nameToFile.reserve(22);

    nameToFile["Default"] = "SND_TXT1";
    nameToFile["Typewriter"] = "SND_TXT2";
    nameToFile["Toriel"] = "snd_txttor";
    nameToFile["Sans"] = "snd_txtsans";
    nameToFile["Papyrus"] = "snd_txtpap";
    nameToFile["Undyne"] = "snd_txtund";
    nameToFile["Alphys"] = "snd_txtal";
    nameToFile["Asgore"] = "snd_txtasg";
    nameToFile["Asriel"] = "snd_txtasr";
    nameToFile["Susie"] = "snd_txtsus";
    nameToFile["Ralsei"] = "snd_txtral";
    nameToFile["Lancer"] = "snd_txtlan";
    nameToFile["Noelle"] = "snd_txtnoe";
    nameToFile["Berdly"] = "snd_txtber";
    nameToFile["Spamton"] = "snd_txtspam";
    nameToFile["Spamton NEO"] = "snd_txtspam2";
    nameToFile["Jevil"] = "snd_txtjok";
    nameToFile["Queen"] = "snd_txtq";
    nameToFile["Tenna"] = "snd_txtten1";  // fallback if my random sound code doesn't work for some reason
    nameToFile["Carol"] = "snd_txtcar";
    nameToFile["Gerson"] = "snd_txtger";
    nameToFile["Jackenstein"] = "snd_txtjack";

    nameToSound.reserve(9);

    nameToSound["The Mechanic"] = "Alphys";
    nameToSound["The Shopkeeper"] = "Gerson";
    nameToSound["Scratch"] = "Lancer";
    nameToSound["Potbor"] = "Spamton NEO";
    nameToSound["Diamond Shopkeeper"] = "Jackenstein";
    nameToSound["The Keymaster"] = "Susie";
    nameToSound["Globed"] = "Tenna";
    nameToSound["Globed Error"] = "Tenna";
    nameToSound["Globed notice"] = "Tenna";  // I thought it was funny

    nameToSoundRate.reserve(4);

    nameToSoundRate["Tenna"] = 3;
    nameToSoundRate["Jackenstein"] = 4;
    nameToSoundRate["Gerson"] = 3;
    nameToSoundRate["Queen"] = 3;
}

void DeltaruneAlertLayer::initSoundRate() {
    int& soundRate = m_fields->soundRate;
    int& soundTimer = m_fields->soundTimer;
    auto const& textSound = m_fields->textSound;
    auto&& nameToSoundRate = m_fields->nameToSoundRate;
    
    if (nameToSoundRate.find(textSound) == nameToSoundRate.end())
        return;

    soundRate = nameToSoundRate[textSound];
    soundTimer = soundRate;
}

bool DeltaruneAlertLayer::init(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale) {
    float& screenSize = m_fields->screenSize;
    if (screenSize >= 569 && !m_fields->dontRestrictWidth)
        screenSize = 569;

    m_fields->text = desc;
    scroll = false;

    if (!FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale)) return false;

    NodeIDs::provideFor(this);
    this->setID("FLAlertLayer");

    std::string_view descString = desc;
    bool const isEmpty = std::ranges::all_of(descString, [](unsigned char c) {
        return std::isspace(c);
    });
    bool const prismLoaded = Loader::get()->isModLoaded("firee.prism");
    std::string_view prismString = "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";  // probably the easiest way to detect a Prism Menu alert :3
    if ((prismLoaded && desc == prismString) || isEmpty) {
        m_fields->incompatible = true;
        return true;
    }

    auto& textArea = m_fields->textArea;
    auto& bg = m_fields->bg;
    auto& titleNode = m_fields->title;

    initMaps();  // for sounds

    this->m_noElasticity = true;

    m_fields->btn1 = m_button1 ? static_cast<CCMenuItemSpriteExtra*>(m_button1->getParent()) : nullptr;
    m_fields->btn2 = m_button2 ? static_cast<CCMenuItemSpriteExtra*>(m_button2->getParent()) : nullptr;

    textArea = static_cast<TextArea*>(m_mainLayer->getChildByID("content-text-area"));
    bg = static_cast<CCScale9Sprite*>(m_mainLayer->getChildByID("background"));
    titleNode = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("title"));
#if !defined(DISABLE_KEYBOARD)
    initCustomKeybinds();
#endif
    Loader::get()->queueInMainThread([bg, titleNode, textArea, this] {
        if (m_fields->incompatible) return;
        if (m_fields->showing) return;

        if (!bg) return;
        if (!titleNode) return;
        if (!textArea) return;
        if (!m_mainLayer) return;

        m_fields->showing = true;
        decideToBlockKeys();
        changeLook();
    });
    return true;
}
void DeltaruneAlertLayer::showButtons() {
    if (m_button2 && getLinesLeft() < 3 && m_fields->doneRolling) {
        m_fields->done = true;
        m_buttonMenu->setVisible(true);
        m_fields->heart->setVisible(true);
    }
}

void DeltaruneAlertLayer::decideToBlockKeys() {
    int numOfSiblings = 0;
    if (auto const parent = getParent()) {
        auto const& siblings = parent->getChildrenExt();

        for (auto const sibling : siblings) {
            if (sibling->getID() == "FLAlertLayer")
                numOfSiblings++;
        }
    }
    if (numOfSiblings >= 1 && !m_button2)
        blockKeys = true;
    else
        blockKeys = false;
}

void DeltaruneAlertLayer::onBtn2(CCObject* sender) {
    if (m_fields->incompatible) {
        FLAlertLayer::onBtn2(sender);
        return;
    }
    if (!m_fields->done) {
        progressText();
        return;
    }

    int const btnSelected = m_fields->btnSelected;
    if (btnSelected == 0)
        return;
    if (btnSelected == 2)
        FLAlertLayer::onBtn2(sender);
    else if (btnSelected == 1)
        FLAlertLayer::onBtn1(sender);
}

void DeltaruneAlertLayer::onBtn1(CCObject* sender) {
    if (m_fields->incompatible) {
        FLAlertLayer::onBtn1(sender);
        return;
    }
    if (!m_fields->done) {
        progressText();
        return;
    }
    blockKeys = false;
    FLAlertLayer::onBtn1(sender);
}

int DeltaruneAlertLayer::getLinesLeft() {
    auto const textArea = m_fields->textArea;

    if (!m_fields->textAreaClippingNode) return 0;
    if (!textArea) return 0;

    auto bitmapFont = textArea->getChildByType<MultilineBitmapFont>(0);
    int totalLines = bitmapFont->getChildrenCount();

    return totalLines - m_fields->linesProgressed;
}

void DeltaruneAlertLayer::show() {
    bool& showing = m_fields->showing;
    bool const incompatible = m_fields->incompatible;
    auto const titleNode = m_fields->title;

    FLAlertLayer::show();

    if (showing) return;
    showing = true;
    if (incompatible) return;

    if (!m_fields->bg) return;
    if (!titleNode) return;
    if (!m_fields->textArea) return;
    if (!m_mainLayer) return;

    decideToBlockKeys();
    changeLook();
}

void DeltaruneAlertLayer::setHeartPosition(CCMenuItemSpriteExtra* button) {
    auto const heart = m_fields->heart;
    auto const text = button->getChildByType<ButtonSprite>(0)->getChildByType<CCLabelBMFont>(0);

    if (!text) return;

    auto const xPos = m_buttonMenu->getPositionX() + button->getPositionX() - text->getContentWidth() / 2 - heart->getContentWidth() / 2 - 5;

    heart->setPositionX(xPos);
}

void DeltaruneAlertLayer::clickedOnButton(CCMenuItemSpriteExtra* btn, ButtonSprite* buttonSprite, int btnSelected) {
    auto const label = buttonSprite->getChildByType<CCLabelBMFont>(0);
    if (!label) return;

    if (btn->isSelected()) {
        label->setColor(ccColor3B{255, 255, 0});
        m_fields->btnSelected = btnSelected;
        setHeartPosition(btn);
    }
    else label->setColor(ccColor3B{255, 255, 255});
}

bool DeltaruneAlertLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (m_fields->incompatible)
        return FLAlertLayer::ccTouchBegan(touch, event);

    if (!m_fields->done && !m_fields->disableClickToProgress) {
        if (m_fields->rolledPage)
            progressText();
        else
            skipText();
    }
    bool const ret = FLAlertLayer::ccTouchBegan(touch, event);
    auto const btn1 = m_fields->btn1;
    auto const btn2 = m_fields->btn2;

    if (!btn1 || !btn2) return ret;
    if (!btn1->isSelected() && !btn2->isSelected()) return ret;

    clickedOnButton(btn1, m_button1, 1);
    clickedOnButton(btn2, m_button2, 2);
    return ret;
}
#if defined(DISABLE_KEYBOARD)
void DeltaruneAlertLayer::keyDown(enumKeyCodes key) {
    if (m_fields->incompatible) {
        FLAlertLayer::keyDown(key);
        return;
    }
    if (key == KEY_Z || key == KEY_Y /*screw QWERTZ*/) {
        if (m_fields->rolledPage)
            progressText();
        return;
    } else if (key == KEY_X || key == KEY_Space) {
        skipText();
        return;
    } else if (key == KEY_ArrowLeft || key == KEY_ArrowRight || key == KEY_Left || key == KEY_Right) {
        if (!m_mainLayer || !m_button2 || !m_fields->doneRolling) {
            FLAlertLayer::keyDown(key);
            return;
        }

        int& btnSelected = m_fields->btnSelected;
        auto const label1 = m_button1->getChildByType<CCLabelBMFont>(0);
        auto const label2 = m_button2->getChildByType<CCLabelBMFont>(0);

        if (key == KEY_ArrowLeft || key == KEY_Left) {
            btnSelected = 1;
            label1->setColor(ccColor3B{255, 255, 0});
            setHeartPosition(m_fields->btn1);
            label2->setColor(ccColor3B{255, 255, 255});
        } else if (key == KEY_ArrowRight || key == KEY_Right) {
            btnSelected = 2;
            label2->setColor(ccColor3B{255, 255, 0});
            setHeartPosition(m_fields->btn2);
            label1->setColor(ccColor3B{255, 255, 255});
        }
    } else
        FLAlertLayer::keyDown(key);
}
#else
void DeltaruneAlertLayer::initCustomKeybinds() {
    if (m_fields->incompatible)
        return;

    this->template addEventListener<keybinds::InvokeBindFilter>(
        [=, this](keybinds::InvokeBindEvent* event) {
            if (event->isDown()) {
                if (!m_mainLayer || !m_button2 || !m_fields->doneRolling) {
                    return ListenerResult::Propagate;
                }

                m_fields->btnSelected = 1;
                auto const label1 = m_button1->getChildByType<CCLabelBMFont>(0);
                auto const label2 = m_button2->getChildByType<CCLabelBMFont>(0);
                label1->setColor(ccColor3B{255, 255, 0});
                setHeartPosition(m_fields->btn1);
                label2->setColor(ccColor3B{255, 255, 255});
                return ListenerResult::Stop;
            }
            return ListenerResult::Propagate;
        }, "left"_spr);

    this->template addEventListener<keybinds::InvokeBindFilter>([=, this](keybinds::InvokeBindEvent* event) {
        if (event->isDown()) {
            if (!m_mainLayer || !m_button2 || !m_fields->doneRolling) {
                return ListenerResult::Propagate;
            }

            m_fields->btnSelected = 2;
            auto const label1 = m_button1->getChildByType<CCLabelBMFont>(0);
            auto const label2 = m_button2->getChildByType<CCLabelBMFont>(0);
            label2->setColor(ccColor3B{255, 255, 0});
            setHeartPosition(m_fields->btn2);
            label1->setColor(ccColor3B{255, 255, 255});
            return ListenerResult::Stop;
        }
        return ListenerResult::Propagate;
    }, "right"_spr);

    this->template addEventListener<keybinds::InvokeBindFilter>([=, this](keybinds::InvokeBindEvent* event) {
        if (event->isDown()) {
            if (m_fields->rolledPage) {
                progressText();
                return ListenerResult::Stop;
            }
        }
        return ListenerResult::Propagate;
    }, "progress"_spr);

    this->template addEventListener<keybinds::InvokeBindFilter>([this](keybinds::InvokeBindEvent* event) {
        if (event->isDown()) {
            skipText();
            return ListenerResult::Stop;
        }
        return ListenerResult::Propagate;
    }, "skip"_spr);
}
#endif

void DeltaruneAlertLayer::skipText() {
    unschedule(schedule_selector(DeltaruneAlertLayer::rollText));

    auto const& clippingNode = m_fields->textAreaClippingNode;
    int const linesProgressed = m_fields->linesProgressed;
    bool& doneRolling = m_fields->doneRolling;

    if (!clippingNode) return;

    auto const& textAreas = clippingNode->getChildrenExt();
    
    for (auto textArea : textAreas) {
        auto const mlbmf = textArea->getChildByType<MultilineBitmapFont>(0);
        auto lines = std::move(mlbmf->getChildrenExt());

        for (int i = linesProgressed + m_fields->rollingLine; i < lines.size() && i < linesProgressed + 3; i++) {
            auto const line = lines[i];
            auto const& letters = line->getChildrenExt();

            for (auto const& letter : letters) {
                letter->setVisible(true);
            }
        }
    }

    m_fields->rolledPage = true;
    if (m_button2) {
        if (getLinesLeft() < 3)
            doneRolling = true;
    } else {
        if (getLinesLeft() <= 3)
            doneRolling = true;
    }
    if (doneRolling) showButtons();
}

int DeltaruneAlertLayer::emptyLinesAmount(int offset) {
    auto const textArea = m_fields->textArea;
    auto const linesProgressed = m_fields->linesProgressed;
    auto const fontNode = textArea->getChildByType<MultilineBitmapFont>(0);
    int lines = 0;

    while (true) {
        if (linesProgressed + lines + offset >= fontNode->getChildrenCount()) break;

        auto const topLine = fontNode->getChildByType<CCLabelBMFont>(linesProgressed + lines + offset);
        if (!topLine) break;

        std::string_view topLineString = topLine->getString();
        bool const empty = std::ranges::all_of(topLineString, [](unsigned char c) {
            return std::isspace(c);
        });
        if (!empty) break;

        lines++;

        auto const star = m_mainLayer->getChildByID("star"_spr);
        auto const starShadow = m_mainLayer->getChildByID("starShadow"_spr);

        if (star) star->setVisible(true);
        if (starShadow) starShadow->setVisible(true);
    }
    return lines;
}

// Adds the ImageNode to the dialog and returns it as well!
ImageNode* DeltaruneAlertLayer::createImageNode() {
    auto const newImageNode = ImageNode::create();

    newImageNode->setID("image-node"_spr);
    m_mainLayer->addChild(newImageNode);

    m_fields->imageNode = newImageNode;
    return newImageNode;
}

void DeltaruneAlertLayer::progressText() {
    if (!m_mainLayer) return;
    if (!m_buttonMenu) return;
    if (!m_fields->textAreaClippingNode) return;

    auto const textArea = m_fields->textArea;
    auto const btn1 = m_fields->btn1;
    auto const btn2 = m_fields->btn2;
    auto const shadow = m_fields->shadow;
    auto const gradientOverlay = m_fields->gradientOverlay;
    int const btnSelected = m_fields->btnSelected;
    int& linesProgressed = m_fields->linesProgressed;
    bool& done = m_fields->done;
    bool const noShadow = m_fields->noShadow;

    if (!textArea) return;

    if (getLinesLeft() - emptyLinesAmount(3) <= 3) {
        if (!m_button2) {
            auto const dialogLayer = m_fields->dialogLayer;
            done = true;
            if (m_fields->dialog && dialogLayer) {
                dialogLayer->keyBackClicked();
            }
            FLAlertLayer::onBtn1(btn1);
            return;
        } else if (btnSelected != 0) {
            done = true;
            if (btnSelected == 1)
                FLAlertLayer::onBtn1(btn1);
            else if (btnSelected == 2)
                FLAlertLayer::onBtn2(btn2);
            return;
        }
    }
    // Don't progress if there's only a choice left!
    if (getLinesLeft() - emptyLinesAmount(3) < 3 && m_button2)
        return;

    // Move EVERYTHING up

    int offset;
    m_mainLayer->getChildByID("star"_spr)->setVisible(false);
    if (!noShadow) m_mainLayer->getChildByID("starShadow"_spr)->setVisible(false);

    unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
    m_fields->characterCount = 0;
    m_fields->rollingLine = 0;

    if (getLinesLeft() > 3)
        offset = 3;
    else if (getLinesLeft() == 3)
        offset = 1;

    int const emptyLines = emptyLinesAmount(offset);
    offset += emptyLines;

    auto const& characters = m_fields->characterSpriteNames;
    int& dialogCount = m_fields->dialogCount;
    bool const progressDialog = emptyLines > 0 && characters.size() > 1;
    
    if (progressDialog) {
        dialogCount++;
        auto const& spriteName = characters[dialogCount];
        auto const imageNode = m_fields->imageNode;

        if (!imageNode)  // Just in case! I have no idea what other mods will do to my stuff...
            createImageNode();

        imageNode->removeAllChildrenWithCleanup(true);
        imageNode->setCharacterImage(spriteName);

        // Setting the title
        auto const& title = m_fields->titles[dialogCount];
        m_fields->title->setString(title.c_str());

        // Getting the right text sound based on the character name
        auto&& nameToSound = m_fields->nameToSound;
        if (nameToSound.find(title.c_str()) != nameToSound.end())
            m_fields->textSound = nameToSound[title.c_str()];
        else
            m_fields->textSound = "Default";
    }

    linesProgressed += offset;
    textArea->setPositionY(textArea->getPositionY() + m_fields->textSize * offset);
    if (gradientOverlay) gradientOverlay->setPositionY(textArea->getPositionY());
    if (shadow) shadow->setPositionY(textArea->getPositionY() - 1);

    showButtons();
    float const pause = Mod::get()->getSettingValue<double>("textRollingPause");
    schedule(schedule_selector(DeltaruneAlertLayer::rollText), pause / 30);
}

time_t const& t = time(nullptr);
auto const& now = fmt::localtime(t);

void DeltaruneAlertLayer::handleAprilFools() {
    if (now.tm_mon != 3 || now.tm_mday != 1)
        return;

    auto const& nameToFile = m_fields->nameToFile;
    auto randomSound = nameToFile.begin();

    std::advance(randomSound, mt() % nameToFile.size());
    m_fields->textSound = randomSound->first;
}

void DeltaruneAlertLayer::rollText(float dt) {
    int& waitQueue = m_fields->waitQueue;
    int const linesProgressed = m_fields->linesProgressed;
    int& characterCount = m_fields->characterCount;
    int& rollingLine = m_fields->rollingLine;
    bool& doneRolling = m_fields->doneRolling;
    bool& rolledPage = m_fields->rolledPage;
    float& lostTime = m_fields->lostTime;
    double const pause = Mod::get()->getSettingValue<double>("textRollingPause") / 30;
    auto& soundTimer = m_fields->soundTimer;
    auto const soundRate = m_fields->soundRate;

    if (dt - pause > pause)
        lostTime += dt - pause;

    bool playSound = true;
    char character;

    soundTimer++;

    for (bool firstRun = true; lostTime >= pause || firstRun; firstRun = false) {
        bool newLine = false;

        if (waitQueue != 0) {
            waitQueue--;
            playSound = false;

            if (lostTime >= pause && !firstRun) {
                lostTime -= pause;
            }
            continue;
        }
        
        if (rollingLine == 3) {
            unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
            rolledPage = true;
            return;
        }
        else rolledPage = false;

        auto const& textAreas = m_fields->textAreaClippingNode->getChildrenExt();
        
        for (auto const textArea : textAreas) {
            auto const mlbmf = textArea->getChildByType<MultilineBitmapFont>(0);
            auto lines = std::move(mlbmf->getChildrenExt<CCLabelBMFont>());
            int currentLine = linesProgressed + rollingLine;

            if (currentLine < lines.size() && currentLine < linesProgressed + 3) {
                auto const line = lines[currentLine];
                auto letters = std::move(line->getChildrenExt<CCSprite>());
                auto const letter = letters[characterCount];
                if (letter->isVisible()) {
                    unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
                    doneRolling = true;
                    rolledPage = true;
                    showButtons();
                    return;
                }
                letter->setVisible(true);
                character = line->getString()[characterCount];
                switch (character) {
                    case ' ':
                        waitQueue = 0;
                        playSound = false;
                        break;
                    case '.':
                        [[fallthrough]];
                    case ',':
                        [[fallthrough]];
                    case ':':
                        [[fallthrough]];
                    case ';':
                        [[fallthrough]];
                    case '?':
                        [[fallthrough]];
                    case '!':
                        waitQueue = 2;
                        break;
                    default:
                        waitQueue = 0;
                        break;
                }
                if (characterCount == line->getChildrenCount() - 1) {
                    newLine = true;
                }
            } else {
                unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
                doneRolling = true;
                rolledPage = true;
                showButtons();
                return;
            }
        }
        characterCount++;
        if (newLine) {
            characterCount = 0;
            rollingLine++;
        }
        if (lostTime >= pause && !firstRun) {
            lostTime -= pause;
        }
    }

    if (!playSound || soundTimer < soundRate) {
        return;
    }

    DeltaruneAlertLayer::playSound(character);
}

void DeltaruneAlertLayer::playSound(char character) {
    auto&& nameToFile = m_fields->nameToFile;
    auto const& textSound = m_fields->textSound;
    auto& soundTimer = m_fields->soundTimer;
    auto& prevSoundNum = m_fields->prevSoundNum;
    auto const& resFolder = Mod::get()->getResourcesDir();

    if (nameToFile.find(textSound) == nameToFile.end()) return;

    handleAprilFools();

    float pitch = 1.f;

    soundTimer = 0;

    std::string file;
    if (textSound == "Queen" || textSound == "Gerson" || textSound == "Jackenstein")
        pitch = 1 + randomNumberInAGivenRangeThatGetsAddedOrRemovedFromADifferentNumber(0.2f);

    if (textSound == "Tenna") {
        int const numSounds = 10;
        int soundNumber;

        if (character && character == '?')
            soundNumber = 5;
        else {
            soundNumber = mt() % numSounds + 1;
            while (prevSoundNum == soundNumber) {
                soundNumber = mt() % numSounds + 1;
            }
        }

        prevSoundNum = soundNumber;

        file = fmt::format("snd_txtten{}", soundNumber);
    }
    else file = nameToFile[textSound];

    auto system = m_fields->system;
    auto channel = m_fields->channel;
    auto sound = m_fields->sound;

    auto const& path = resFolder / fmt::format("{}.wav", std::string_view(file));
    system->createSound(string::pathToString(path).c_str(), FMOD_DEFAULT, nullptr, &sound);
    system->playSound(sound, nullptr, false, &channel);
    channel->setPitch(pitch);
    channel->setVolumeRamp(false);
    channel->setVolume(FMODAudioEngine::sharedEngine()->m_sfxVolume);
}