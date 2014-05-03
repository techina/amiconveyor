#import "GamePlatform_objc.h"

@implementation GamePlatform_objc

-(void)show {
    GKLocalPlayer *player = [GKLocalPlayer localPlayer];
    if ([player isAuthenticated]) {
        GKGameCenterViewController *gcView = [GKGameCenterViewController new];
        if (gcView != nil)
        {
            gcView.gameCenterDelegate = self;
            gcView.viewState = GKGameCenterViewControllerStateLeaderboards;
            [[UIApplication sharedApplication].keyWindow.rootViewController presentViewController:gcView animated:YES completion:nil];
        }
    }
}

- (void)gameCenterViewControllerDidFinish:(GKGameCenterViewController *)gameCenterViewController
{
    [[UIApplication sharedApplication].keyWindow.rootViewController dismissViewControllerAnimated:YES completion:nil];
}

- (void)report:(int)score
{
    if ([GKLocalPlayer localPlayer].isAuthenticated) {
        NSDictionary *ids = [[NSDictionary alloc] initWithContentsOfFile:[[NSBundle mainBundle] pathForResource:@"ids" ofType:@"plist"]];
        GKScore* gkScore = [[GKScore alloc] initWithLeaderboardIdentifier:ids[@"leaderboard"]];
        gkScore.value = score;
        [GKScore reportScores:@[gkScore] withCompletionHandler:^(NSError *error) {
            if (error) {
                // エラーの場合
            }
        }];
    }
}

@end
