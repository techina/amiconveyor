#import "Store.h"

void Store::openReviewPage() {
    NSDictionary *ids = [[NSDictionary alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"ids" ofType:@"plist"]];
    NSString *appStoreId = ids[@"appstore"];
    NSString *reviewUrl;
    NSString *osversion = [UIDevice currentDevice].systemVersion;
    NSArray *a = [osversion componentsSeparatedByString:@"."];
    BOOL isIOS7 = [(NSString *)[a objectAtIndex:0] intValue] >= 7;
    if (isIOS7) {
        reviewUrl = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/app/id%@", appStoreId];
    } else {
        reviewUrl = [NSString stringWithFormat:@"itms-apps://itunes.apple.com/WebObjects/MZStore.woa/wa/viewContentsUserReviews?id=%@&onlyLatestVersion=true&pageNumber=0&sortOrdering=1&type=Purple+Software", appStoreId];
    }
    [[UIApplication sharedApplication] openURL:[NSURL URLWithString:reviewUrl]];
}
