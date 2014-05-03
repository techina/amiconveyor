#include "AppDelegate.h"
#include "Common/LBFileUtils.h"
#include "Scene/Title/TitleScene.h"

USING_NS_CC;

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();
    if (!glView) {
        glView = GLView::create("leadblow");
        director->setOpenGLView(glView);
    }
    glView->setDesignResolutionSize(640, 960, ResolutionPolicy::FIXED_WIDTH);

#ifdef COCOS2D_DEBUG
    // turn on display FPS
    director->setDisplayStats(true);
#endif

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0 / 60);

    // create a scene. it's an autorelease object
    auto scene = TitleScene::createScene();

    srand((unsigned int)time(NULL));

    // run
    director->runWithScene(scene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}

static CustomCommand saveToFileCommand;

void AppDelegate::screenShot(const char* fileName, std::function<void(std::string)> callback) {
    auto s = Director::getInstance()->getVisibleSize();
    auto texture = RenderTexture::create(s.width, s.height);
    texture->setPosition(Point(s.width * 0.5f, s.height * 0.5f));
    texture->beginWithClear(1, 1, 1, 1);
    Director::getInstance()->getRunningScene()->visit();
    texture->end();
    std::string fullpath = LBFileUtils::getCachePath() + fileName;
    saveToFileCommand.init(texture->getGlobalZOrder());
    saveToFileCommand.func = [texture, fullpath, callback]() {
        Image *image = texture->newImage(true);
        if (image)
        {
            image->saveToFile(fullpath.c_str(), true);
        }
        CC_SAFE_DELETE(image);
        callback(fullpath);
    };
    Director::getInstance()->getRenderer()->addCommand(&saveToFileCommand);
}

cocos2d::ui::Button* AppDelegate::createButton(const char* img, const char* txt)
{
    auto button = cocos2d::ui::Button::create();
    button->loadTextureNormal(img, cocos2d::ui::TextureResType::UI_TEX_TYPE_LOCAL);
    button->setTitleText(txt);
    button->setTitleFontSize(48);
    button->setTouchEnabled(true);
    button->setPressedActionEnabled(true);
    return button;
}
