#import <Foundation/Foundation.h>
#import <GameKit/GameKit.h>

@interface GamePlatform_objc : NSObject<GKGameCenterControllerDelegate>

-(void)show;
-(void)report:(int)score;

@end
