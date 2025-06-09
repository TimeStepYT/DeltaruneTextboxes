#include "include.hpp"

class $modify(DeltarunePurchaseItemPopup, PurchaseItemPopup) {
    struct Fields {
        FLAlertLayer* quickPopup = nullptr;
    };
    void handlePurchasePopup();
    void onCancel();
    void onBuy();
    bool init(GJStoreItem* p0);
};