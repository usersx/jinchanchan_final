/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "HelloWorldScene.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}
class CharacterController {
public:
    void initWithCharacter(Sprite* character, Node* scene);
    void moveToTarget(Vec2 targetPosition);

private:
    Sprite* mCharacter;
    Node* mScene;

    void onTouchesBegan(Touch* touch, Event* event);
};

void CharacterController::initWithCharacter(Sprite* character, Node* scene) {

    mCharacter = character;
    mScene = scene;

    auto listener = EventListenerTouchOneByOne::create();
    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, character);

}

void CharacterController::onTouchesBegan(Touch* touch, Event* event) {

    auto target = mScene->convertToNodeSpaceAR(touch->getLocation());
    this->moveToTarget(target);
}

void CharacterController::moveToTarget(Vec2 target) {

    auto moveTo = MoveTo::create(0.5, target);
    mCharacter->runAction(moveTo);

}/*
class CharacterController {
public:
    // 初始化,传入角色 sprite
    void initWithCharacter(Sprite* character);

    // 点击回调 
    void onTouchesBegan(Touch* touch, Event* event);

    // 执行移动
    void moveToTarget(Vec2 targetPosition);

private:
    Sprite* mCharacter; // 被控制的角色
};

// 实现
void CharacterController::initWithCharacter(Sprite* character) {
    mCharacter = character;

    // 初始化点击监听
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(CharacterController::onTouchesBegan, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

void CharacterController::onTouchesBegan(Touch* touch, Event* event) {

    // 获取点击位置
    auto target = this.

    // 执行移动 
    this->moveToTarget(target);
}

void CharacterController::moveToTarget(Vec2 target) {

    auto moveTo = MoveTo::create(0.5, target);

    mCharacter->runAction(moveTo);
}*/
// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

void testController() {

    // 创建场景
    auto scene = Scene::create();

    // 创建角色精灵
    auto character = Sprite::create("grossini.png");
    character->setPosition(100, 100);
    scene->addChild(character);

    // 创建控制器 
    auto controller = CharacterController();
    controller.initWithCharacter(character, scene);

    // 第一目标点  
    Vec2 target1(300, 300);
    controller.moveToTarget(target1);

    // 延迟后第二目标点
    auto delay = DelayTime::create(2);
    auto func = CallFunc::create([&]() {
        Vec2 target2(100, 500);
    controller.moveToTarget(target2);
        });
    scene->runAction(Sequence::create(delay, func, nullptr));

    // 运行场景
    Director::getInstance()->replaceScene(scene);
}
// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width/2;
        float y = origin.y + closeItem->getContentSize().height/2;
        closeItem->setPosition(Vec2(x,y));
    }

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // position the label on the center of the screen
        label->setPosition(Vec2(origin.x + visibleSize.width/2,
                                origin.y + visibleSize.height - label->getContentSize().height));

        // add the label as a child to this layer
        this->addChild(label, 1);
    }

    // add "HelloWorld" splash screen"
    /*auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(sprite, 0);
    }
    */
    /*创建一个grossini 2d图像*/
    /*auto spriteGcn = Sprite::create("grossini.png");
    if (spriteGcn == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // position the sprite on the center of the screen
        spriteGcn->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);
        spriteGcn->setPosition(Vec2(0 + origin.x, 320 + origin.y));

        // add the sprite as a child to this layer
        this->addChild(spriteGcn, 0);
    }
    */
    /*创建一个boss对象，但呈现红色*/
    /*auto sprite3D_ = Sprite3D::create("boss.c3b"); //c3b file, created with the FBX-converter
    sprite3D_->setScale(5.f); //sets the object scale in float
    sprite3D_->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y)); //sets sprite position
    this->addChild(sprite3D_, 1); //adds sprite to scene, z-index: 1
    auto rotation = RotateBy::create(15, Vec3(0, 360, 0));
    sprite3D_->runAction(RepeatForever::create(rotation));
    sprite3D_->setAnchorPoint(Point(0.0f, 0.0f));
    */

    /*auto player = Sprite3D::create("chapter9/orc.c3b");
    player->setScale(0.08);
    player->setPositionY(terrain->getHeight(player->getPositionX(), player->getPositionZ()));
    */
    // 创建角色精灵
    auto character = Sprite::create("grossini.png");
    character->setPosition(100, 100);
    this->addChild(character);

    // 创建控制器 
    auto controller = CharacterController();
    controller.initWithCharacter(character, this);

    // 第一目标点  
    Vec2 target1(300, 300);
    controller.moveToTarget(target1);

    // 延迟后第二目标点
    auto delay = DelayTime::create(2);
    auto func = CallFunc::create([&]() {
        Vec2 target2(100, 500);
    controller.moveToTarget(target2);
        });
    this->runAction(Sequence::create(delay, func, nullptr));

    // 运行场景
    Director::getInstance()->replaceScene(this);
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}
