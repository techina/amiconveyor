#import "GamePlatform.h"
#import "GamePlatform_objc.h"

void GamePlatform::show()
{
    [[[GamePlatform_objc alloc] init] show];
}

void GamePlatform::reportScore(int score)
{
    [[[GamePlatform_objc alloc] init] report:score];
}
