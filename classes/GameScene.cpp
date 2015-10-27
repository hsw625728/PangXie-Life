#include "GameScene.h"
#include "Player.h"
#include "NetWork.h"
#include "cocos2d\cocos\ui\UIButton.h"

using namespace cocos2d;
using namespace ui;

template<> GameScene* Singleton<GameScene>::msSingleton = 0;
GameScene::GameScene()
{
	new NetManager();
	new Player();
}
GameScene::~GameScene()
{
	delete NetManager::getSingletonPtr();
	delete Player::getSingletonPtr();
}
bool GameScene::init()
{
	if (!Scene::init())
		return false;

	NetManager::getSingleton().init();
	this->addChild((Node*)(NetManager::getSingletonPtr()));
	return true;
}

void GameScene::onEnter()
{
	Scene::onEnter();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();


	mLayerUI = Layer::create();
	mLayerUISchedule = Layer::create();
	mLayerFI = Layer::create();
	mLayerSP = Layer::create();
	mLayerBI = Layer::create();
	mLayerBG = LayerColor::create(Color4B(122, 122, 122, 255));

	/////////////////////////////////////////////////////////////////////////////////////
	//mLayerUI  Begin
	/////////////////////////////////////////////////////////////////////////////////////
	//左上角名字Label
	std::string spider(Player::getSingleton().getNickname());
	Label* lb_name = Label::createWithTTF(spider, "fonts/STLITI.TTF", 72);
	lb_name->setDimensions(400, 100);
	lb_name->setColor(Color3B(125, 0, 0));
	lb_name->setPosition(origin + Vec2(250, visibleSize.height - 60));
	lb_name->enableShadow(Color4B(0, 0, 0, 128), Size(4, -4));
	mLayerUI->addChild(lb_name);

	//中间状态Label
	mLabel_state = Label::createWithTTF("Sleep ... ...", "fonts/STLITI.TTF", 72);
	mLabel_state->setDimensions(1000, 100);
	mLabel_state->setColor(Color3B(125, 0, 0));
	mLabel_state->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height - 60));
	mLabel_state->enableShadow(Color4B(0, 0, 0, 128), Size(4, -4));
	mLayerUI->addChild(mLabel_state);

	//右侧金钱Label
	//char moneyStr[64];
	//memset(moneyStr, 0, 64);
	//_i64toa(Player::getSingleton().getMoney(), moneyStr, 10);
	//Label* lb_money = Label::createWithTTF(std::string(moneyStr), "fonts/STLITI.TTF", 72);
	//lb_money->setDimensions(400, 100);
	//lb_money->setColor(Color3B(125, 0, 0));
	//lb_money->setPosition(origin + Vec2(visibleSize.width - 250, visibleSize.height - 60));
	//lb_money->enableShadow(Color4B(0, 0, 0, 128), Size(4, -4));
	//mLayerUI->addChild(lb_money);

	//右侧日程显示按钮
	Button* button = Button::create("schedule_open.png", "schedule_open.png");
	button->setContentSize(Size(400, 100));
	button->setPosition(origin + Vec2(visibleSize.width - 250, visibleSize.height - 60));
	button->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEvent, this));
	button->setZoomScale(0.4f);
	button->setPressedActionEnabled(true);
	mLayerUI->addChild(button);
	button->setOpacity(100);
	/////////////////////////////////////////////////////////////////////////////////////
	//mLayerUI  End
	/////////////////////////////////////////////////////////////////////////////////////


	/////////////////////////////////////////////////////////////////////////////////////
	//mLayerUISchedule  Begin
	/////////////////////////////////////////////////////////////////////////////////////
	//this->updateScheduleListView();


	////关闭按钮
	//Button* closeBtn = Button::create("CloseNormal.png", "CloseSelected.png");
	//closeBtn->setPosition(Vec2(visibleSize.width - closeBtn->getContentSize().width / 2, visibleSize.height - closeBtn->getContentSize().height / 2));
	//closeBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventSchedule, this));
	//mLayerUISchedule->addChild(closeBtn);

	/////////////////////////////////////////////////////////////////////////////////////
	//mLayerUISchedule  End
	/////////////////////////////////////////////////////////////////////////////////////
	this->addChild(mLayerBG);
	this->addChild(mLayerBI);
	this->addChild(mLayerSP);
	this->addChild(mLayerFI);

	mLayerManager = LayerMultiplex::create(mLayerUI, mLayerUISchedule, nullptr);
	mLayerManager->switchTo(0);
	this->addChild(mLayerManager);
	NetManager::getSingleton().cs_Login();
}
void GameScene::updatePlayerState(std::string& msg)
{
	mLabel_state->setString(msg);
}

void GameScene::updateScheduleListView()
{
	mLayerUISchedule->removeAllChildren();

	Layout* mLayoutSchedule = Layout::create();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	const DailyActions& acts = Player::getSingleton().getDailySchedule();

	//界面背景容器
	mLayoutSchedule->setSize(Size(visibleSize.width * 0.836f, visibleSize.height - 2));
	mLayoutSchedule->setPosition(Vec2(visibleSize.width / 2 - mLayoutSchedule->getContentSize().width/2, 1));
	mLayoutSchedule->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	mLayoutSchedule->setBackGroundColor(Color3B(100, 100, 100));
	//mLayoutSchedule->setBackGroundImage("SCloseNormal.png");

	//界面添加和关闭按钮
	Button* addBtn = Button::create("schedule_add.png", "schedule_add.png");
	addBtn->setPosition(Vec2(addBtn->getContentSize().width / 2, mLayoutSchedule->getContentSize().height - addBtn->getContentSize().height / 2));
	addBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::addScheduleBtn, this));
	mLayoutSchedule->addChild(addBtn);

	Button* closeBtn = Button::create("schedule_close.png", "schedule_close.png");
	closeBtn->setPosition(Vec2(mLayoutSchedule->getContentSize().width - closeBtn->getContentSize().width / 2, mLayoutSchedule->getContentSize().height - closeBtn->getContentSize().height / 2));
	closeBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::closeScheduleBtn, this));
	mLayoutSchedule->addChild(closeBtn);

	//详细日程的list
	ListView* mListViewSchedule = ListView::create();

	mListViewSchedule->setDirection(ui::ScrollView::Direction::VERTICAL);
	mListViewSchedule->setBounceEnabled(true);
	mListViewSchedule->setBackGroundImage("green_edit.png");
	mListViewSchedule->setBackGroundImageScale9Enabled(true);
	mListViewSchedule->setContentSize(Size(visibleSize.width * 0.836f, (visibleSize.height - 2) * 0.875f));
	mListViewSchedule->setPosition(Vec2(mLayoutSchedule->getContentSize().width / 2 - mListViewSchedule->getContentSize().width / 2, 1));


	for (int i = 0; i < acts.size(); i++)
	{
		//单个容器
		Layout* layout = Layout::create();
		layout->setSize(Size(visibleSize.width * 0.836f, visibleSize.height * 0.121f));
		layout->setBackGroundImage("schedule_list_item.png");

		//事件图标
		CCSprite* sp = CCSprite::create("action_sleep_icon.png");
		sp->setContentSize(Size(layout->getContentSize().height, layout->getContentSize().height));
		sp->setPosition(layout->getContentSize().width * 0.04f, layout->getContentSize().height/2-1);
		layout->addChild(sp);

		//事件名称
		Label* lb = Label::createWithTTF(std::string("Sleep"), "fonts/STLITI.TTF", 46);
		lb->enableShadow(Color4B::GRAY, Size(10, 10));
		lb->enableOutline(Color4B::BLACK, 3);
		lb->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lb->setPosition(layout->getContentSize().width * 0.157f, layout->getContentSize().height / 2 - 1);
		lb->setDimensions(layout->getContentSize().width * 0.157f, layout->getContentSize().height);
		layout->addChild(lb);

		//持续时间
		Label* lbtime = Label::createWithTTF(acts.at(i).second, "fonts/STLITI.TTF", 46);
		lbtime->enableShadow(Color4B::GRAY, Size(10, 10));
		lbtime->enableOutline(Color4B::BLACK, 3);
		lbtime->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lbtime->setPosition(layout->getContentSize().width * 0.381f, layout->getContentSize().height / 2 - 1);
		lbtime->setDimensions(layout->getContentSize().width * 0.294f, layout->getContentSize().height);
		layout->addChild(lbtime);

		//开始时间和结束时间
		Label* lbstartl = Label::createWithTTF("Start", "fonts/STLITI.TTF", 30);
		lbstartl->enableShadow(Color4B::GRAY, Size(10, 10));
		lbstartl->enableOutline(Color4B::BLACK, 3);
		lbstartl->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lbstartl->setPosition(layout->getContentSize().width * 0.646f, layout->getContentSize().height *3 / 4 - 1);
		lbstartl->setDimensions(layout->getContentSize().width * 0.15f, layout->getContentSize().height / 2);
		layout->addChild(lbstartl);

		Label* lbendl = Label::createWithTTF("End", "fonts/STLITI.TTF", 30);
		lbendl->enableShadow(Color4B::GRAY, Size(10, 10));
		lbendl->enableOutline(Color4B::BLACK, 3);
		lbendl->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lbendl->setPosition(layout->getContentSize().width * 0.882f, layout->getContentSize().height*3 / 4 - 1);
		lbendl->setDimensions(layout->getContentSize().width * 0.15f, layout->getContentSize().height / 2);
		layout->addChild(lbendl);

		Label* lbstart;
		if (i == 0)
			lbstart = Label::createWithTTF("00:00:00", "fonts/STLITI.TTF", 30);
		else
			lbstart = Label::createWithTTF(acts.at(i - 1).second, "fonts/STLITI.TTF", 30);
		lbstart->enableShadow(Color4B::GRAY, Size(10, 10));
		lbstart->enableOutline(Color4B::BLACK, 3);
		lbstart->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lbstart->setPosition(layout->getContentSize().width * 0.646f, layout->getContentSize().height / 4 - 1);
		lbstart->setDimensions(layout->getContentSize().width * 0.15f, layout->getContentSize().height/2);
		layout->addChild(lbstart);
		
		Label* lbend;
		if (i == acts.size() - 1)
			lbend = Label::createWithTTF("00:00:00", "fonts/STLITI.TTF", 30);
		else
			lbend = Label::createWithTTF(acts.at(i + 1).second, "fonts/STLITI.TTF", 30);
		lbend->enableShadow(Color4B::GRAY, Size(10, 10));
		lbend->enableOutline(Color4B::BLACK, 3);
		lbend->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lbend->setPosition(layout->getContentSize().width * 0.882f, layout->getContentSize().height / 4 - 1);
		lbend->setDimensions(layout->getContentSize().width * 0.15f, layout->getContentSize().height / 2);
		layout->addChild(lbend);

		mListViewSchedule->addChild(layout);
	}

	// set all items layout gravity
	mListViewSchedule->setGravity(ListView::Gravity::CENTER_VERTICAL);
	// set items margin
	mListViewSchedule->setItemsMargin(22.0f);

	mLayoutSchedule->addChild(mListViewSchedule);
	mLayerUISchedule->addChild(mLayoutSchedule);
}

void GameScene::touchEvent(Ref *pSender, Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		mLabel_state->setString(StringUtils::format("Touch Down"));
		break;

	case Widget::TouchEventType::MOVED:
		mLabel_state->setString(StringUtils::format("Touch Move"));
		break;

	case Widget::TouchEventType::ENDED:
	{
		mLabel_state->setString(StringUtils::format("Touch Up"));
		mLayerManager->switchTo(1);
	}
		break;

	case Widget::TouchEventType::CANCELED:
		mLabel_state->setString(StringUtils::format("Touch Cancelled"));
		break;

	default:
		break;
	}
}
void GameScene::closeScheduleBtn(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		mLayerManager->switchTo(0);
	}
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void GameScene::addScheduleBtn(Ref* pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
	}
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}