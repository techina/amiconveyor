#import "InappPurchase.h"

@implementation InappPurchase

- (void)requestPurchasing:(NSString *)productID withCallback:(func)callback {
    onPurchaseFinish = callback;
    if ([SKPaymentQueue canMakePayments]) {
        NSSet *productIds = [NSSet setWithObject:productID];
        SKProductsRequest *req = [[SKProductsRequest alloc] initWithProductIdentifiers:productIds];
        req.delegate = self;
        [req start];
    } else {
        onPurchaseFinish(@"cant payment");
    }
}

- (void)productsRequest:(SKProductsRequest *)request didReceiveResponse:(SKProductsResponse *)response {
    if ([response.invalidProductIdentifiers count] > 0) {
        onPurchaseFinish(@"fail");
        return;
    }
    [[SKPaymentQueue defaultQueue] addTransactionObserver:self];
    for (SKProduct *product in response.products) {
        SKPayment *payment = [SKPayment paymentWithProduct:product];
        [[SKPaymentQueue defaultQueue] addPayment:payment];
    }
}

- (void)paymentQueue:(SKPaymentQueue *)queue updatedTransactions:(NSArray *)transactions {
    for (SKPaymentTransaction *transaction in transactions) {
        switch (transaction.transactionState) {
            case SKPaymentTransactionStatePurchasing:
                break;

            case SKPaymentTransactionStatePurchased:
                [queue finishTransaction:transaction];
                onPurchaseFinish(@"success");
                break;

            case SKPaymentTransactionStateFailed:
                [queue finishTransaction:transaction];
                onPurchaseFinish(@"fail");
                break;

            case SKPaymentTransactionStateRestored:
                [queue finishTransaction:transaction];
                break;

            default:
                break;
        }
    }
}

-(void)paymentQueue:(SKPaymentQueue *)queue removedTransactions:(NSArray *)transactions {
    [[SKPaymentQueue defaultQueue] removeTransactionObserver:self];
}

@end
