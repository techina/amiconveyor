#import <StoreKit/StoreKit.h>

typedef void (^func)(NSString*);

@interface InappPurchase : NSObject <SKProductsRequestDelegate, SKPaymentTransactionObserver> {
    func onPurchaseFinish;
}

- (void)requestPurchasing:(NSString *)productID withCallback:(func)callback;

@end
