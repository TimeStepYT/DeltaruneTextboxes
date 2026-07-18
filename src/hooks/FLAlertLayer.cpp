#include <Geode/utils/cocos.hpp>
#include "../TextShaders.hpp"
#include "../ImageNode.hpp"
#include "../Global.hpp"
#include "FLAlertLayer.hpp"
#include "DialogLayer.hpp"
#include "PlatformToolbox.hpp"
#include "../Events.hpp"
#include "../DeltaruneDialogObject.hpp"

using namespace geode::prelude;

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

$on_mod(Loaded) {
    DeltaruneMaps::init();
}

void DeltaruneMaps::init() {
    auto& nameToChar = DeltaruneMaps::nameToCharacter;
    auto& charToData = DeltaruneMaps::characterToData;
    auto& titleToChar = DeltaruneMaps::titleToCharacter;

    using namespace DeltaruneMaps;

    nameToChar.reserve(static_cast<int>(Character::NUM_CHARACTERS));

    nameToChar["Default"] = Character::DEFAULT; 
    nameToChar["Typewriter"] = Character::TYPEWRITER; 
    nameToChar["Toriel"] = Character::TORIEL; 
    nameToChar["Sans"] = Character::SANS; 
    nameToChar["Papyrus"] = Character::PAPYRUS; 
    nameToChar["Undyne"] = Character::UNDYNE; 
    nameToChar["Alphys"] = Character::ALPHYS; 
    nameToChar["Asgore"] = Character::ASGORE; 
    nameToChar["Asriel"] = Character::ASRIEL; 
    nameToChar["Susie"] = Character::SUSIE; 
    nameToChar["Ralsei"] = Character::RALSEI; 
    nameToChar["Lancer"] = Character::LANCER; 
    nameToChar["Noelle"] = Character::NOELLE; 
    nameToChar["Berdly"] = Character::BERDLY; 
    nameToChar["Spamton"] = Character::SPAMTON; 
    nameToChar["Spamton NEO"] = Character::SPAMTON_NEO;
    nameToChar["Jevil"] = Character::JEVIL;
    nameToChar["Queen"] = Character::QUEEN;
    nameToChar["Tenna"] = Character::TENNA;
    nameToChar["Carol"] = Character::CAROL;
    nameToChar["Gerson"] = Character::GERSON;
    nameToChar["Jackenstein"] = Character::JACKENSTEIN;
    nameToChar["Flowery"] = Character::FLOWERY;

    charToData.reserve(static_cast<int>(Character::NUM_CHARACTERS));

    charToData[Character::DEFAULT] = CharacterData::create("SND_TXT1");
    charToData[Character::TYPEWRITER] = CharacterData::create("SND_TXT2");
    charToData[Character::TORIEL] = CharacterData::create("snd_txttor");
    charToData[Character::SANS] = CharacterData::create("snd_txtsans");
    charToData[Character::PAPYRUS] = CharacterData::create("snd_txtpap");
    charToData[Character::UNDYNE] = CharacterData::create("snd_txtund");
    charToData[Character::ALPHYS] = CharacterData::create("snd_txtal");
    charToData[Character::ASGORE] = CharacterData::create("snd_txtasg");
    charToData[Character::ASRIEL] = CharacterData::create("snd_txtasr");
    charToData[Character::SUSIE] = CharacterData::create("snd_txtsus");
    charToData[Character::RALSEI] = CharacterData::create("snd_txtral");
    charToData[Character::LANCER] = CharacterData::create("snd_txtlan");
    charToData[Character::NOELLE] = CharacterData::create("snd_txtnoe");
    charToData[Character::BERDLY] = CharacterData::create("snd_txtber");
    charToData[Character::SPAMTON] = CharacterData::create("snd_txtspam");
    charToData[Character::SPAMTON_NEO] = CharacterData::create("snd_txtspam2");
    charToData[Character::JEVIL] = CharacterData::create("snd_txtjok");
    charToData[Character::QUEEN] = CharacterData::create("snd_txtq", 3, true);
    charToData[Character::TENNA] = CharacterData::create("", 3, true);
    charToData[Character::CAROL] = CharacterData::create("snd_txtcar");
    charToData[Character::GERSON] = CharacterData::create("snd_txtger", 3, true);
    charToData[Character::JACKENSTEIN] = CharacterData::create("snd_txtjack", 4, true);
    charToData[Character::FLOWERY] = CharacterData::create("", 3, true);

    titleToChar.reserve(9);

    titleToChar["The Mechanic"] = Character::ALPHYS;
    titleToChar["The Shopkeeper"] = Character::GERSON;
    titleToChar["Scratch"] = Character::LANCER;
    titleToChar["Potbor"] = Character::SPAMTON_NEO;
    titleToChar["Diamond Shopkeeper"] = Character::JACKENSTEIN;
    titleToChar["The Keymaster"] = Character::SUSIE;
    titleToChar["Globed"] = Character::TENNA;
    titleToChar["Globed Error"] = Character::TENNA;
    titleToChar["Globed notice"] = Character::TENNA; // I thought it was funny
}

void DeltaruneAlertLayer::initSoundRate() {
    int& soundRate = m_fields->soundRate;
    int& soundTimer = m_fields->soundTimer;
    auto const& textSound = m_fields->textSound;
    auto const& charToData = DeltaruneMaps::characterToData;
    
    if (charToData.find(textSound) == charToData.end())
        return;

    soundRate = charToData.at(textSound).m_soundRate;
    soundTimer = soundRate;
}

bool DeltaruneAlertLayer::init(FLAlertLayerProtocol* delegate, char const* title, gd::string desc, char const* btn1, char const* btn2, float width, bool scroll, float height, float textScale) {
    float& screenSize = m_fields->screenSize;

    screenSize = CCDirector::sharedDirector()->getWinSize().width;

    m_fields->text = desc;
    scroll = false;

#ifdef GEODE_IS_WINDOWS
    // Removing controller glyphs like this seems better than removing the sprites afterwards
    const auto app = CCApplication::get();
    const bool controllerConnected = app->m_bControllerConnected;

    app->m_bControllerConnected = false;
    
    if (!FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale)) {
        app->m_bControllerConnected = controllerConnected;
        return false;
    }
    app->m_bControllerConnected = controllerConnected;
#elifdef GEODE_IS_MACOS
    PlatToolbox::disable = true;
    
    if (!FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale)) {
        PlatToolbox::disable = false;
        return false;
    }

    PlatToolbox::disable = false;
#else
    if (!FLAlertLayer::init(delegate, title, desc, btn1, btn2, width, scroll, height, textScale))
        return false;
#endif
    

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

    auto& textArea = m_fields->old_textArea;
    auto& bg = m_fields->bg;
    auto& titleNode = m_fields->title;

    auto const& nameToChar = DeltaruneMaps::nameToCharacter;
    auto const& textSoundStr = Mod::get()->getSettingValue<std::string>("textSound");

    if (nameToChar.find(textSoundStr) != nameToChar.end())
        this->m_fields->textSound = nameToChar.at(textSoundStr);

    this->m_noElasticity = true;

    m_fields->old_btn1 = m_button1 ? static_cast<CCMenuItemSpriteExtra*>(m_button1->getParent()) : nullptr;
    m_fields->old_btn2 = m_button2 ? static_cast<CCMenuItemSpriteExtra*>(m_button2->getParent()) : nullptr;

    textArea = static_cast<TextArea*>(m_mainLayer->getChildByID("content-text-area"));
    bg = static_cast<CCScale9Sprite*>(m_mainLayer->getChildByID("background"));
    titleNode = static_cast<CCLabelBMFont*>(m_mainLayer->getChildByID("title"));

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

void DeltaruneAlertLayer::registerKeybinds() {
    if (m_fields->incompatible)
        return;

    this->addEventListener(KeybindSettingPressedEventV3(Mod::get(), "keybind-left"),
        [this](Keybind const& keybind, bool down, bool repeat, double time){
            if (!down || repeat || !m_mainLayer || !m_button2 || !m_fields->doneRolling) 
                return;

            int& btnSelected = m_fields->btnSelected;
            auto const label1 = m_fields->btn1->getChildByType<CCLabelBMFont>(0);
            auto const label2 = m_fields->btn2->getChildByType<CCLabelBMFont>(0);

            btnSelected = 1;
            label1->setColor(ccColor3B{255, 255, 0});
            this->setHeartPosition(m_fields->btn1);
            label2->setColor(ccColor3B{255, 255, 255});
        }
    );
    this->addEventListener(KeybindSettingPressedEventV3(Mod::get(), "keybind-right"),
        [this](Keybind const& keybind, bool down, bool repeat, double time){
            if (!down || repeat || !m_mainLayer || !m_button2 || !m_fields->doneRolling) 
                return;

            int& btnSelected = m_fields->btnSelected;
            auto const label1 = m_fields->btn1->getChildByType<CCLabelBMFont>(0);
            auto const label2 = m_fields->btn2->getChildByType<CCLabelBMFont>(0);

            btnSelected = 2;
            label2->setColor(ccColor3B{255, 255, 0});
            this->setHeartPosition(m_fields->btn2);
            label1->setColor(ccColor3B{255, 255, 255});
        }
    );
    this->addEventListener(KeybindSettingPressedEventV3(Mod::get(), "keybind-confirm"),
        [this](Keybind const& keybind, bool down, bool repeat, double time){
            if (!down || repeat || !m_fields->rolledPage) 
                return;

            auto gm = GameManager::get();

            // Special case with the A button on the controller:
            // Without it, it would skip every other group of three lines of text because
            // no matter what, the A button clicks at the same time as well
            if (keybind.key == cocos2d::CONTROLLER_A) {
                if (getLinesLeft() - emptyLinesAmount(3) <= 3)
                    this->pickChoice();
                return;
            }

            this->progressText();
        }
    );

    // I wanted to add Controller_B as a default to this but that closes the entire popup.
    // I can't be bothered to disable that right now
    this->addEventListener(KeybindSettingPressedEventV3(Mod::get(), "keybind-skip"),
        [this](Keybind const& keybind, bool down, bool repeat, double time){
            if (!down || repeat) return;

            this->skipText();
        }
    );
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
        global::blockKeys = true;
    else
        global::blockKeys = false;
}

void DeltaruneAlertLayer::onBtn2(CCObject* sender) {
    if (m_fields->incompatible) {
        FLAlertLayer::onBtn2(sender);
        return;
    }
    if (!m_fields->done) {
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

    global::blockKeys = false;

    if (this->m_button2) {
        FLAlertLayer::onBtn1(sender);
        return;
    }

    auto& nextAlerts = this->m_fields->nextAlerts;
    
    size_t skippedCount = 0;
    for (auto const& alert : nextAlerts) {
        if (!alert) {
            skippedCount++;
            continue;
        }
        
        FLAlertLayer* unmodifiedAlert = nullptr;
        DeltaruneEvents::createDialogWithVoice(
            &unmodifiedAlert,
            alert->m_characterSprite,
            alert->m_voice, 
            alert->m_title, 
            alert->m_text
        );
        
        if (!unmodifiedAlert) {
            skippedCount++;
            continue;
        }

        std::vector<DialogObjectPtr> objects;
        objects.reserve(nextAlerts.size() - 1 - skippedCount);

        for (size_t i = 1 + skippedCount; i < nextAlerts.size(); ++i) {
            objects.push_back(std::move(nextAlerts[i]));
        }
        
        if (objects.size() != 0) {
            auto alert = static_cast<DeltaruneAlertLayer*>(unmodifiedAlert);

            alert->setNextAlerts(objects);
        }
        break;
    }

    FLAlertLayer::onBtn1(sender);
}

int DeltaruneAlertLayer::getLinesLeft() {
    auto const& textArea = m_fields->m_textArea;

    if (!textArea) return 0;

    int totalLines = textArea->getLines().size();

    return totalLines - m_fields->linesProgressed;
}

void DeltaruneAlertLayer::show() {
    bool& showing = m_fields->showing;
    bool const incompatible = m_fields->incompatible;
    auto const titleNode = m_fields->title;

    FLAlertLayer::show();

    if (showing) return;
    if (incompatible) return;
    
    showing = true;

    if (!m_fields->bg) return;
    if (!titleNode) return;
    if (!m_fields->old_textArea) return;
    if (!m_mainLayer) return;
    
    Loader::get()->queueInMainThread([this](){
        this->registerKeybinds();
    });

    decideToBlockKeys();
    changeLook();
}

void DeltaruneAlertLayer::setHeartPosition(Button* button) {
    auto const heart = m_fields->heart;
    auto const text = button->getChildByType<CCLabelBMFont>(0);

    if (!text) return;

    float const distanceFromText = 5.f;
    float const buttonLeftSide = button->getPositionX() - text->getContentWidth() / 2;
    float const xPos = m_fields->screenSize / 2 + buttonLeftSide - heart->getContentWidth() / 2 - distanceFromText;

    heart->setPositionX(xPos);
}

void DeltaruneAlertLayer::clickedOnButton(Button* activatedButton, Button* otherButton, int btnSelected) {
    auto const label = activatedButton->getChildByType<CCLabelBMFont*>(0);
    auto const otherLabel = otherButton->getChildByType<CCLabelBMFont*>(0);
    if (!label || !otherLabel) return;

    label->setColor(ccColor3B{255, 255, 0});
    m_fields->btnSelected = btnSelected;
    this->setHeartPosition(activatedButton);
    otherLabel->setColor(ccColor3B{255, 255, 255});
}

bool DeltaruneAlertLayer::ccTouchBegan(CCTouch* touch, CCEvent* event) {
    if (m_fields->incompatible)
        return FLAlertLayer::ccTouchBegan(touch, event);

    if (!m_fields->done && !m_fields->disableClickToProgress) {
        if (m_fields->rolledPage)
            this->progressText();
        else
            this->skipText();
    }
    return FLAlertLayer::ccTouchBegan(touch, event);
}

void DeltaruneAlertLayer::skipText() {
    auto const fields = this->m_fields.self();
    
    this->setState(TextboxState::WAITING);

    int const linesProgressed = fields->linesProgressed;
    bool& doneRolling = fields->doneRolling;

    auto const textArea = fields->m_textArea;
    
    if (!textArea)
        return;

    auto const lines = textArea->getLines();

    for (int i = linesProgressed + fields->rollingLine; i < lines.size() && i < linesProgressed + 3; i++) {
        auto const line = lines[i];
        auto const& letters = line->getChildrenExt();

        for (auto const& letter : letters) {
            letter->setVisible(true);
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
    auto const textArea = m_fields->m_textArea;
    int const linesProgressed = m_fields->linesProgressed;
    int lineCount = 0;

    while (true) {
        auto const lines = textArea->getLines();
        if (linesProgressed + lineCount + offset >= lines.size()) break;

        auto const topLine = lines.at(linesProgressed + lineCount + offset);
        if (!topLine) break;

        std::string_view topLineString = topLine->getString();
        bool const empty = std::ranges::all_of(topLineString, [](unsigned char c) {
            return std::isspace(c);
        });
        if (!empty) break;

        lineCount++;

        auto const star = m_fields->textContentNode->getChildByID("star"_spr);

        if (star) star->setVisible(true);
    }
    return lineCount;
}

// Adds the ImageNode to the dialog and returns it as well!
ImageNode* DeltaruneAlertLayer::createImageNode() {
    if (this->m_fields->imageNode) {
        log::debug("Skipped creating an ImageNode again");
        return this->m_fields->imageNode;
    }
    
    auto const newImageNode = ImageNode::create();

    newImageNode->setID("image-node"_spr);
    this->m_mainLayer->addChild(newImageNode);

    this->m_fields->imageNode = newImageNode;
    return newImageNode;
}

void DeltaruneAlertLayer::pickChoice() {
    auto const fields = this->m_fields.self();
    bool& done = fields->done;
    auto const btn1 = fields->old_btn1;
    auto const btn2 = fields->old_btn2;
    int const btnSelected = fields->btnSelected;

    if (!m_button2) {
        auto const dialogLayer = fields->dialogLayer;

        done = true;
        
        if (fields->dialog && dialogLayer)
            dialogLayer->keyBackClicked();

        FLAlertLayer::onBtn1(btn1);
    } else if (btnSelected != 0) {
        done = true;
        
        if (btnSelected == 1)
            FLAlertLayer::onBtn1(btn1);
        else if (btnSelected == 2)
            FLAlertLayer::onBtn2(btn2);
    }
}

void DeltaruneAlertLayer::progressText() {
    auto const fields = m_fields.self();
    
    if (!m_mainLayer) return;
    if (!m_buttonMenu) return;

    auto const deltaruneTextArea = fields->m_textArea;
    int& linesProgressed = fields->linesProgressed;
    bool const noShadow = fields->noShadow;

    if (!deltaruneTextArea) return;
    
    auto const textArea = deltaruneTextArea->getTextArea();
    if (!textArea) return;
    
    if (this->getLinesLeft() - this->emptyLinesAmount(3) <= 3) {
        this->pickChoice();
        if (fields->done)
            return;
    }
    // Don't progress if there's only a choice left!
    if (this->getLinesLeft() - this->emptyLinesAmount(3) < 3 && m_button2)
        return;

    // Move EVERYTHING up

    int offset;
    fields->textContentNode->getChildByID("star"_spr)->setVisible(false);

    //this->unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
    fields->characterCount = 0;
    fields->rollingLine = 0;

    if (this->getLinesLeft() > 3)
        offset = 3;
    else if (this->getLinesLeft() == 3)
        offset = 1;

    int const emptyLines = this->emptyLinesAmount(offset);
    offset += emptyLines;

    auto const& characters = fields->characterSpriteNames;
    int& dialogCount = fields->dialogCount;
    bool const progressDialog = emptyLines > 0 && characters.size() > 1;
    
    // Only applies if it comes from a DialogLayer
    if (progressDialog) {
        dialogCount++;
        auto const& spriteName = characters[dialogCount];
        auto const imageNode = fields->imageNode;

        if (!imageNode)  // Just in case! I have no idea what other mods will do to my stuff...
            this->createImageNode();

        imageNode->removeAllChildrenWithCleanup(true);
        imageNode->setCharacterImage(spriteName);

        // Setting the title
        auto const& title = fields->titles[dialogCount];
        fields->title->setString(title.c_str());

        // Getting the right text sound based on the character name
        auto const& nameToChar = DeltaruneMaps::nameToCharacter;
        if (nameToChar.find(title.c_str()) != nameToChar.end())
            fields->textSound = nameToChar.at(title);
        else
            fields->textSound = DeltaruneMaps::Character::DEFAULT;
    }

    linesProgressed += offset;
    textArea->setPositionY(textArea->getPositionY() + fields->textSize * offset);

    this->showButtons();
    this->setState(TextboxState::ROLLING_TEXT);
}

time_t const& t = time(nullptr);
auto const& now = geode::localtime(t);

void DeltaruneAlertLayer::handleAprilFools() {
    if (now.tm_mon != 3 || now.tm_mday != 1)
        return;

    auto const& charToData = DeltaruneMaps::characterToData;
    auto randomSound = charToData.begin();

    std::advance(randomSound, mt() % static_cast<int>(DeltaruneMaps::Character::NUM_CHARACTERS));
    m_fields->textSound = randomSound->first;
}

void DeltaruneAlertLayer::rollText(float dt) {
    auto const fields = m_fields.self();

    int& waitQueue = fields->waitQueue;
    int const linesProgressed = fields->linesProgressed;
    int& characterCount = fields->characterCount;
    int& rollingLine = fields->rollingLine;
    bool& doneRolling = fields->doneRolling;
    bool& rolledPage = fields->rolledPage;
    float& lostTime = fields->lostTime;
    double const pause = Mod::get()->getSettingValue<double>("textRollingPause") / 30;
    auto& soundTimer = fields->soundTimer;
    auto const soundRate = fields->soundRate;

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
            this->setState(TextboxState::WAITING);
            rolledPage = true;
            return;
        }
        else rolledPage = false;

        auto const textArea = fields->m_textArea;
        
        auto const lines = textArea->getLines();
        int currentLine = linesProgressed + rollingLine;

        if (currentLine < lines.size() && currentLine < linesProgressed + 3) {
            auto const line = lines.at(currentLine);
            auto letters = line->getChildrenExt<CCSprite>();
            auto const letter = letters[characterCount];
            if (letter->isVisible()) {
                this->setState(TextboxState::WAITING);
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
            this->setState(TextboxState::WAITING);
            doneRolling = true;
            rolledPage = true;
            showButtons();
            this->updateRenderTexture();
            return;
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
    this->updateRenderTexture();

    if (!playSound || soundTimer < soundRate) {
        return;
    }

    if (fields->externalSound.has_value()) {
        DeltaruneAlertLayer::playExternalSound(character);
    }
    else
        DeltaruneAlertLayer::playSound(character);
}

void DeltaruneAlertLayer::updateRenderTexture() {
    // auto fields = this->m_fields.self();

    // fields->renderedSprite->render();
}

void DeltaruneAlertLayer::playExternalSound(char character) {
    auto const& textSound = m_fields->externalSound.value();
    auto& soundTimer = m_fields->soundTimer;
    
    if (DeltaruneMaps::externalNameToData.find(textSound) == DeltaruneMaps::externalNameToData.end()) {
        return;
    }
    
    auto const& data = DeltaruneMaps::externalNameToData.at(textSound);
    float pitch = 1.f;
    soundTimer = 0;

    m_fields->soundRate = data.m_soundRate;

    if (data.m_hasPitchVariation)
        pitch = 1 + randomNumberInAGivenRangeThatGetsAddedOrRemovedFromADifferentNumber(0.2f);

    this->doTheSoundPlaying(data.m_sounds, pitch);
}

void DeltaruneAlertLayer::playSound(char character) {
    auto const& charToData = DeltaruneMaps::characterToData;
    auto const& textSound = m_fields->textSound;
    auto& soundTimer = m_fields->soundTimer;
    auto& prevSoundNum = m_fields->prevSoundNum;

    auto const& resFolder = Mod::get()->getResourcesDir();

    if (charToData.find(textSound) == charToData.end()) {
        return;
    }

    auto const& charData = charToData.at(textSound);

    this->handleAprilFools();

    float pitch = 1.f;

    soundTimer = 0;

    std::string file = charToData.at(textSound).m_sounds.at(0);
    if (charData.m_hasPitchVariation)
        pitch = 1 + randomNumberInAGivenRangeThatGetsAddedOrRemovedFromADifferentNumber(0.2f);

    if (textSound == DeltaruneMaps::Character::TENNA) {
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
    else if (textSound == DeltaruneMaps::Character::FLOWERY) {
        int const numSounds = 3;
        int soundNumber = mt() % numSounds + 1;

        while (prevSoundNum == soundNumber) {
            soundNumber = mt() % numSounds + 1;
        }

        prevSoundNum = soundNumber;

        file = fmt::format("snd_flowery_voicenoise_{}", soundNumber);
    }

    auto filePath = Mod::get()->getResourcesDir() / fmt::format("{}.wav", std::string_view(file));

    this->doTheSoundPlaying(string::pathToString(filePath), pitch);
}

void DeltaruneAlertLayer::doTheSoundPlaying(std::span<std::string const> files, float pitch) {
    auto const& resFolder = Mod::get()->getResourcesDir();
    auto& prevSoundNum = m_fields->prevSoundNum;

    auto system = m_fields->system;
    auto channel = m_fields->channel;
    auto sound = m_fields->sound;

    int const numSounds = files.size();
    int soundNumber = 0;

    if (numSounds > 1) {
        soundNumber = mt() % numSounds;

        while (prevSoundNum == soundNumber) {
            soundNumber = mt() % numSounds;
        }

        prevSoundNum = soundNumber;
    }

    auto const path = files[soundNumber];
    system->createSound(string::pathToString(path).c_str(), FMOD_DEFAULT, nullptr, &sound);
    system->playSound(sound, nullptr, false, &channel);
    channel->setPitch(pitch);
    channel->setVolumeRamp(false);
    channel->setVolume(FMODAudioEngine::sharedEngine()->m_sfxVolume);
}

void DeltaruneAlertLayer::doTheSoundPlaying(std::string const& file, float pitch) {
    auto system = m_fields->system;
    auto channel = m_fields->channel;
    auto sound = m_fields->sound;

    system->createSound(file.c_str(), FMOD_DEFAULT, nullptr, &sound);
    system->playSound(sound, nullptr, false, &channel);
    channel->setPitch(pitch);
    channel->setVolumeRamp(false);
    channel->setVolume(FMODAudioEngine::sharedEngine()->m_sfxVolume);
}

void DeltaruneAlertLayer::setTextSound(DeltaruneMaps::Character textSound) {
    this->m_fields->textSound = textSound;
}

void DeltaruneAlertLayer::setNextAlerts(std::span<DialogObjectPtr> newNextAlerts) {
    auto const fields = this->m_fields.self();
    auto& nextAlerts = fields->nextAlerts;
    auto size = newNextAlerts.size();

    nextAlerts.reserve(size);
    
    for (size_t i = 0; i < size; ++i) {
        nextAlerts.push_back(std::move(newNextAlerts[i]));
    }
}

void DeltaruneAlertLayer::setState(TextboxState newState) {
    auto& state = this->m_fields->state;

    if (state == newState)
        return;

    switch (newState) {
        case TextboxState::ROLLING_TEXT:
        {
            double const pause = Mod::get()->getSettingValue<double>("textRollingPause");
            this->schedule(schedule_selector(DeltaruneAlertLayer::rollText), pause / 30);
            break;
        }
        case TextboxState::WAITING:
            this->unschedule(schedule_selector(DeltaruneAlertLayer::rollText));
            break;
        default:
            break;
    }
    state = newState;
}