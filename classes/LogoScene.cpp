#include "LogoScene.h"
#include "LoadingScene.h"
#include "NetWork.h"

using namespace cocos2d;

bool LogoScene::init()
{
	if (Scene::init())
	{
		return true;
	}
	else
		return false;
}

void LogoScene::Step(float dt)
{
	this->unschedule(schedule_selector(LogoScene::Step));
	CCDirector::sharedDirector()->purgeCachedData();
	CCScene* scene = LoadingScene::create();
	//LogoScene 场景到 LoadingScene 的特效在这里处理
	CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.1f, scene));
}

void LogoScene::onEnter()
{
	Scene::onEnter();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	auto layer = LayerColor::create(Color4B(255, 255, 255, 255));
	this->addChild(layer);

	auto sprite = Sprite::create("Logo1.png");
	sprite->setPosition(Vec2(visibleSize.width / 2 + 114, visibleSize.height / 2));
	layer->addChild(sprite);

	this->schedule(schedule_selector(LogoScene::Step), 0.1f);
}