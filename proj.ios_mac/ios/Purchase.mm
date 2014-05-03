#import "Purchase.h"
#import "InappPurchase.h"

void Purchase::purchase(const char* productID, function<void(string)> callback) {
    auto iap = [[InappPurchase alloc] init];
    [iap requestPurchasing:[NSString stringWithUTF8String:productID] withCallback:^(NSString* message) {
        callback(string([message UTF8String]));
    }];
}
