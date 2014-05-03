#import "LBSocial.h"
#import <Social/Social.h>

void commonShare(const char *message, const char *ssFilePath, NSString* serviceType) {
    SLComposeViewController *cvc = [SLComposeViewController composeViewControllerForServiceType:serviceType];
    [cvc setInitialText:[NSString stringWithFormat:@"%s", message]];
    [cvc addImage:[UIImage imageWithContentsOfFile:[NSString stringWithFormat:@"%s", ssFilePath]]];
    [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:cvc animated:YES completion:nil];
}

void LBSocial::tweet(const char *message, const char *ssFilePath) {
    commonShare(message, ssFilePath, SLServiceTypeTwitter);
}

void LBSocial::facebook(const char *message, const char *ssFilePath) {
    commonShare(message, ssFilePath, SLServiceTypeFacebook);
}
