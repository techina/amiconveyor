#import <UIKit/UIKit.h>

@class RootViewController;
extern "C"{
#import "GADBannerView.h"
};

@interface AppController : NSObject <UIApplicationDelegate> {
    UIWindow *window;
    RootViewController    *viewController;
    GADBannerView *bannerView_;
}

@end

