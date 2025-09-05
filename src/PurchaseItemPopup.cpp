#include "PurchaseItemPopup.hpp"

void DeltarunePurchaseItemPopup::handlePurchasePopup() {
    auto& quickPopup = m_fields->quickPopup;
    auto itemIcon = m_mainLayer->getChildByType<GJItemIcon>(0);
    auto bg = quickPopup->m_mainLayer->getChildByID("background");

    if (!bg) return;
    
    float xPos = bg->getPositionX();
    float yPos = bg->getContentHeight() + itemIcon->getContentHeight() / 2 - 3;
    
    itemIcon->setPositionX(xPos);
    itemIcon->setPositionY(yPos);
    
    quickPopup->m_mainLayer->addChild(itemIcon);
    
    this->setVisible(false);

    quickPopup->setTouchPriority(this->getTouchPriority() + 1);
    quickPopup->setVisible(true);
}

void DeltarunePurchaseItemPopup::onBuy() {
    this->onPurchase(nullptr);
}

void DeltarunePurchaseItemPopup::onCancel() {
    this->onClose(nullptr);
}

bool DeltarunePurchaseItemPopup::init(GJStoreItem* p0) {
    if (!PurchaseItemPopup::init(p0))
        return false;

    auto const title = this->m_mainLayer->getChildByType<CCLabelBMFont>(0)->getString();
    std::string content = "";

    auto const textArea = this->m_mainLayer->getChildByType<TextArea>(0);

    auto const mlbmf = textArea->getChildByType<MultilineBitmapFont>(0);
    auto const& contentParent = mlbmf->getChildrenExt<CCLabelBMFont>();

    for (auto const label : contentParent) {
        content.append(label->getString());
    }

    auto const menu = this->m_mainLayer->getChildByType<CCMenu>(0);

    if (!menu) return true;

    auto const button1 = menu->getChildByType<CCMenuItemSpriteExtra>(0)->getChildByType<ButtonSprite>(0);
    auto const button2 = menu->getChildByType<CCMenuItemSpriteExtra>(1)->getChildByType<ButtonSprite>(0);

    if (!button1 || !button2) return true;

    auto const button1String = button1->getChildByType<CCLabelBMFont>(0)->getString();
    auto const button2String = button2->getChildByType<CCLabelBMFont>(0)->getString();

    auto quickPopup = geode::createQuickPopup(
        title,
        content,
        button1String, button2String,
        [popup = Ref(this)] (auto, bool btn2) {
            if (btn2) {
                popup->onBuy();
                return;
            }
            popup->onCancel();
        }
    );

    // teehee
    this->setKeyboardEnabled(false);
    this->setKeypadEnabled(false);
    this->setTouchEnabled(false);
    this->setMouseEnabled(false);

    quickPopup->setVisible(false);
    m_fields->quickPopup = quickPopup;
    Loader::get()->queueInMainThread([this] { // for some reason, making it invisible immediately doesn't work
        this->handlePurchasePopup();
        });

    return true;
}