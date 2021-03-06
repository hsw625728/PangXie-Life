#include "GameScene.h"
#include "Player.h"
#include "DominoManager.h"
#include "NetWork.h"
#include "MessageBoxLayer.h"
#include "ui/UIButton.h"
#include "ui/UIRadioButton.h"

using namespace cocos2d;
using namespace ui;

#define kTagTileMap 100
template<> GameScene* Singleton<GameScene>::msSingleton = 0;
GameScene::GameScene()
{
	new NetManager();
	new Player();
	new DominoManager();
}
GameScene::~GameScene()
{
	delete NetManager::getSingletonPtr();
	delete Player::getSingletonPtr();
	delete DominoManager::getSingletonPtr();
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
	mLayerUIEditorMenu = Layer::create();
	mLayerUIEditor = Layer::create();
	mLayerUITestDomino = Layer::create();
	mLayerEditor = Layer::create();
	mLayerFI = Layer::create();
	mLayerSP = Layer::create();
	mLayerBI = Layer::create();
	mLayerBG = LayerColor::create(Color4B(122, 122, 122, 255));

	/////////////////////////////////////////////////////////////////////////////////////
	//mLayerUI  Begin
	/////////////////////////////////////////////////////////////////////////////////////
	//左上角名字Label
	std::string spider(Player::getSingleton().getNickname());
	Label* lb_name = Label::createWithTTF(spider, "STLITI.TTF", 72);
	lb_name->setDimensions(400, 100);
	lb_name->setColor(Color3B(125, 0, 0));
	lb_name->setPosition(origin + Vec2(250, visibleSize.height - 60));
	lb_name->enableShadow(Color4B(0, 0, 0, 128), Size(4, -4));
	mLayerUI->addChild(lb_name);

	//中间状态Label
	mLabel_state = Label::createWithTTF("Sleep ... ...", "STLITI.TTF", 72);
	mLabel_state->setDimensions(1000, 100);
	mLabel_state->setColor(Color3B(125, 0, 0));
	mLabel_state->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height - 60));
	mLabel_state->enableShadow(Color4B(0, 0, 0, 128), Size(4, -4));
	mLayerUI->addChild(mLabel_state);

	//右侧金钱Label
	//char moneyStr[64];
	//memset(moneyStr, 0, 64);
	//_i64toa(Player::getSingleton().getMoney(), moneyStr, 10);
	//Label* lb_money = Label::createWithTTF(std::string(moneyStr), "STLITI.TTF", 72);
	//lb_money->setDimensions(400, 100);
	//lb_money->setColor(Color3B(125, 0, 0));
	//lb_money->setPosition(origin + Vec2(visibleSize.width - 250, visibleSize.height - 60));
	//lb_money->enableShadow(Color4B(0, 0, 0, 128), Size(4, -4));
	//mLayerUI->addChild(lb_money);

	//右侧日程显示按钮
	Button* button = Button::create("button_schedule.png", "button_schedule.png");
	button->setContentSize(Size(400, 100));
	button->setPosition(origin + Vec2(visibleSize.width - 250, visibleSize.height - 60));
	button->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEvent, this));
	button->setZoomScale(0.4f);
	button->setPressedActionEnabled(true);
	mLayerUI->addChild(button);
	//button->setOpacity(100);//设置透明度
	/////////////////////////////////////////////////////////////////////////////////////
	//mLayerUI  End
	/////////////////////////////////////////////////////////////////////////////////////

	//排行榜
	Button* button_ph = Button::create("button_ranklist.png", "button_ranklist.png");
	//button->setContentSize(Size(400, 100));
	button_ph->setPosition(origin + Vec2(visibleSize.width/2, visibleSize.height/2 - 260));
	button_ph->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventPH, this));
	button_ph->setZoomScale(0.4f);
	button_ph->setPressedActionEnabled(true);
	mLayerUI->addChild(button_ph);

	//编辑器
	Button* button_bj = Button::create("button_editor.png", "button_editor.png");
	//button->setContentSize(Size(400, 100));
	button_bj->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height / 2));
	button_bj->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventBJ, this));
	button_bj->setZoomScale(0.4f);
	button_bj->setPressedActionEnabled(true);
	mLayerUI->addChild(button_bj);
	updateEditorUIMenuLayer();
	updateEditorUILayer();
	updateEditorLayer();
	//我的骨牌
	Button* button_my = Button::create("button_play_domino.png", "button_play_domino.png");
	//button->setContentSize(Size(400, 100));
	button_my->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height / 2 + 260));
	button_my->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventMY, this));
	button_my->setZoomScale(0.4f);
	button_my->setPressedActionEnabled(true);
	mLayerUI->addChild(button_my);

	/////////////////////////////////////////////////////////////////////////////////////
	//mLayerSp Begin
	/////////////////////////////////////////////////////////////////////////////////////
	std::string plist_content;
	{
		std::string fullPath = FileUtils::getInstance()->fullPathForFilename("yintama_animation.plist");
		Data data = FileUtils::getInstance()->getDataFromFile(fullPath);
		if (!data.isNull())
			plist_content.assign((const char*)data.getBytes(), data.getSize());
	}

	std::string image_content;
	{
		std::string fullPath = FileUtils::getInstance()->fullPathForFilename("yintama_animation.png");
		Data data = FileUtils::getInstance()->getDataFromFile(fullPath);
		if (!data.isNull())
			image_content.assign((const char*)data.getBytes(), data.getSize());
	}

	Image image;
	image.initWithImageData((const uint8_t*)image_content.c_str(), image_content.size());
	Texture2D* texture = new (std::nothrow) Texture2D();
	texture->initWithImage(&image);
	texture->autorelease();

	auto cache = SpriteFrameCache::getInstance();
	cache->addSpriteFramesWithFileContent(plist_content, texture);

	//
	// Animation using Sprite BatchNode
	//
	Sprite * sprite = Sprite::createWithSpriteFrameName("image_yin_01.png");
	//sprite->setScale(4.0f);
	sprite->setPosition(Vec2(visibleSize.width / 2 - 80, visibleSize.height / 2));
	mLayerSP->addChild(sprite);

	Vector<SpriteFrame*> animFrames(4);

	char str[100] = { 0 };
	for (int i = 1; i < 4; i++)
	{
		sprintf(str, "image_yin_%02d.png", i);
		auto frame = cache->getSpriteFrameByName(str);
		animFrames.pushBack(frame);
	}
	auto frame = cache->getSpriteFrameByName("image_yin_02.png");
	animFrames.pushBack(frame);

	auto animation = Animation::createWithSpriteFrames(animFrames, 0.3f);
	sprite->runAction(RepeatForever::create(Animate::create(animation)));
	/////////////////////////////////////////////////////////////////////////////////////
	//mLayerSP End
	/////////////////////////////////////////////////////////////////////////////////////
	this->addChild(mLayerBG);

	mUILayerManager = LayerMultiplex::create(mLayerUI, mLayerUISchedule, mLayerUIEditorMenu, mLayerUIEditor, mLayerUITestDomino, nullptr);
	mUILayerManager->switchTo(0);

	mLayerManager = LayerMultiplex::create(mLayerBI, mLayerSP, mLayerFI, mLayerEditor, nullptr);
	mLayerManager->switchTo(0);

	this->addChild(mLayerManager);
	this->addChild(mUILayerManager);
	NetManager::getSingleton().cs_Login();
}
void GameScene::updatePlayerState(std::string& msg)
{
	mLabel_state->setString(msg);
}
void GameScene::updateEditorUIMenuLayer()
{
	mLayerUIEditorMenu->removeAllChildren();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//界面整体背景
	Layout* layoutBG = Layout::create();
	layoutBG->setSize(Size(visibleSize.width*0.95f, visibleSize.height*0.95f));
	layoutBG->setPosition(Vec2(visibleSize.width*0.025f, visibleSize.height*0.025f));
	layoutBG->setBackGroundColorType(Layout::BackGroundColorType::SOLID);
	layoutBG->setBackGroundColor(Color3B(255, 255, 255));
	mLayerUIEditorMenu->addChild(layoutBG);

	//标题栏
	Layout* layout_title = Layout::create();
	layout_title->setSize(Size(visibleSize.width * 0.94f, visibleSize.height * 0.05f));
	layout_title->setPosition(Vec2(visibleSize.width * 0.005f, visibleSize.height * 0.88f));
	layout_title->setBackGroundImageScale9Enabled(true);
	layout_title->setBackGroundImage("gray.png");
	layoutBG->addChild(layout_title);

	//关闭按钮
	Button* button_close = Button::create("CloseNormal.png", "CloseSelected.png");
	button_close->setPosition(Vec2(layout_title->getSize().width * 0.9f , layout_title->getSize().height*0.1f));
	button_close->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventEditorUIMenuClose, this));
	button_close->setScale(2.0f);
	layout_title->addChild(button_close);

	//缩略图
	ListView* mListViewThumbnail = ListView::create();

	mListViewThumbnail->setDirection(ui::ScrollView::Direction::HORIZONTAL);
	mListViewThumbnail->setBounceEnabled(true);
	mListViewThumbnail->setBackGroundImage("list_view_bg.png");
	mListViewThumbnail->setBackGroundImageScale9Enabled(true);
	mListViewThumbnail->setContentSize(Size(layoutBG->getSize().width*0.96f, layoutBG->getSize().width*0.25f));
	mListViewThumbnail->setPosition(Vec2(layoutBG->getSize().width*0.02f, layoutBG->getSize().height*0.3f));
	mListViewThumbnail->setGravity(ListView::Gravity::CENTER_VERTICAL);
	mListViewThumbnail->setItemsMargin(22.0f);

	RadioButtonGroup* radioButtonGroup = RadioButtonGroup::create();
	mListViewThumbnail->addChild(radioButtonGroup);
	radioButtonGroup->addEventListener(CC_CALLBACK_3(GameScene::onChangedEditorUIRadioButtonGroup, this));

	const int COUNT = DominoManager::getSingleton().getTileMapsCount();
	for (int i = 0; i < COUNT; i++)
	{
		Layout* layout = Layout::create();	
		layout->setBackGroundImageScale9Enabled(true);
		layout->setBackGroundImage("gray.png");
		layout->setSize(Size(mListViewThumbnail->getSize().height*0.9f, mListViewThumbnail->getSize().height*0.9f));

		//auto filePathNormal = StringUtils::format("editor_ui_radio_button%1d.png", i + 1);
		//auto filePathSelected = StringUtils::format("editor_ui_radio_button%1ds.png", i + 1);
		//RadioButton* rb = RadioButton::create(filePathNormal, filePathSelected);
		//rb->setPosition(Vec2(visibleSize.width*0.045f, visibleSize.width*0.045f));
		////rb->setScale();
		////rb->setZoomScale();
		//radioButtonGroup->addRadioButton(rb);

		TMXTiledMap* map = TMXTiledMap::create("tile.tmx");
		map->setContentSize(layout->getSize());
		map->setScale(0.16f);

		Size visibleSize = Director::getInstance()->getVisibleSize();
		auto ts = map->getTileSize();
		auto ms = map->getMapSize();
		map->setPosition(visibleSize.width / 2 - ts.width*ms.width / 2, visibleSize.height / 2 - ts.height*ms.height / 2);
		{
			auto layer = map->getLayer("map");
			TileMap& tilemap = DominoManager::getSingleton().getTileMap(i);
			TileMapStr::iterator iter = tilemap.map.begin();
			int index = 0;
			int size = tilemap.map.size();
			for (; iter != tilemap.map.end(); ++iter)
			{
				unsigned char tile = *iter;
				if ((tile & 0x01) == 0)
				{
					tile = 1;
				}
				else
				{
					tile = (0x03 & (tile >> 1));
					switch (tile)
					{
					case 0:
						tile = 8;
						break;
					case 1:
						tile = 5;
						break;
					case 2:
						tile = 6;
						break;
					case 3:
						tile = 7;
						break;
					default:
						tile = 1;
						break;
					}
				}
				layer->setTileGID(tile, Vec2(index / tilemap.mTileWidth, index%tilemap.mTileWidth));
				index++;
			}
		}
		layout->addChild(map);
		mListViewThumbnail->addChild(layout);
	}
	layoutBG->addChild(mListViewThumbnail);

	//编辑按钮
	Button* button_editor = Button::create("button_editor.png", "button_editor.png");
	button_editor->setPosition(Vec2(visibleSize.width * 0.25f, visibleSize.height * 0.1f));
	button_editor->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventEditorUIMenuEditor, this));
	layoutBG->addChild(button_editor);
	//复制按钮
	Button* button_clone = Button::create("button_copy.png", "button_copy.png");
	button_clone->setPosition(Vec2(visibleSize.width / 2, visibleSize.height * 0.1f));
	button_clone->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventEditorUIMenuClone, this));
	layoutBG->addChild(button_clone);
	//测试按钮
	Button* button_test = Button::create("button_test.png", "button_test.png");
	button_test->setPosition(Vec2(visibleSize.width * 0.75f, visibleSize.height * 0.1f));
	button_test->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventEditorUIMenuTest, this));
	layoutBG->addChild(button_test);

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
	mLayoutSchedule->setBackGroundColor(Color3B(255, 255, 255));
	//mLayoutSchedule->setBackGroundImage("SCloseNormal.png");

	//界面添加和关闭按钮
	Button* addBtn = Button::create("button_addlistitem.png", "button_addlistitem.png");
	addBtn->setPosition(Vec2(addBtn->getContentSize().width / 2, mLayoutSchedule->getContentSize().height - addBtn->getContentSize().height / 2));
	addBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::addScheduleBtn, this));
	mLayoutSchedule->addChild(addBtn);

	Button* closeBtn = Button::create("button_close.png", "button_close.png");
	closeBtn->setPosition(Vec2(mLayoutSchedule->getContentSize().width - closeBtn->getContentSize().width / 2, mLayoutSchedule->getContentSize().height - closeBtn->getContentSize().height / 2));
	closeBtn->addTouchEventListener(CC_CALLBACK_2(GameScene::closeScheduleBtn, this));
	mLayoutSchedule->addChild(closeBtn);

	//详细日程的list
	ListView* mListViewSchedule = ListView::create();

	mListViewSchedule->setDirection(ui::ScrollView::Direction::VERTICAL);
	mListViewSchedule->setBounceEnabled(true);
	mListViewSchedule->setBackGroundImage("list_view_bg.png");
	mListViewSchedule->setBackGroundImageScale9Enabled(true);
	mListViewSchedule->setContentSize(Size(visibleSize.width * 0.836f, (visibleSize.height - 2) * 0.875f));
	mListViewSchedule->setPosition(Vec2(mLayoutSchedule->getContentSize().width / 2 - mListViewSchedule->getContentSize().width / 2, 1));


	for (int i = 0; i < acts.size(); i++)
	{
		//单个容器
		Layout* layout = Layout::create();
		layout->setSize(Size(visibleSize.width * 0.83f, visibleSize.height * 0.121f));
		layout->setBackGroundImageScale9Enabled(true);
		layout->setBackGroundImage("gray.png");

		//事件图标
		char num[64];
		memset(num, 0, 8);
		sprintf(num, "icon_%d.png", i + 1);
		CCSprite* sp = CCSprite::create(num);
		//sp->setContentSize(Size(layout->getContentSize().height, layout->getContentSize().height));
		sp->setPosition(layout->getContentSize().width * 0.04f, layout->getContentSize().height/2-1);
		layout->addChild(sp);

		//事件名称
		Label* lb = Label::createWithTTF(std::string("Sleep"), "STLITI.TTF", 46);
		lb->enableShadow(Color4B::GRAY, Size(10, 10));
		lb->enableOutline(Color4B::BLACK, 3);
		lb->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lb->setPosition(layout->getContentSize().width * 0.157f, layout->getContentSize().height / 2 - 1);
		lb->setDimensions(layout->getContentSize().width * 0.157f, layout->getContentSize().height);
		layout->addChild(lb);

		//持续时间
		Label* lbtime = Label::createWithTTF(acts.at(i).second, "STLITI.TTF", 46);
		lbtime->enableShadow(Color4B::GRAY, Size(10, 10));
		lbtime->enableOutline(Color4B::BLACK, 3);
		lbtime->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lbtime->setPosition(layout->getContentSize().width * 0.381f, layout->getContentSize().height / 2 - 1);
		lbtime->setDimensions(layout->getContentSize().width * 0.294f, layout->getContentSize().height);
		layout->addChild(lbtime);

		//开始时间和结束时间
		Label* lbstartl = Label::createWithTTF("Start", "STLITI.TTF", 30);
		lbstartl->enableShadow(Color4B::GRAY, Size(10, 10));
		lbstartl->enableOutline(Color4B::BLACK, 3);
		lbstartl->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lbstartl->setPosition(layout->getContentSize().width * 0.646f, layout->getContentSize().height *3 / 4 - 1);
		lbstartl->setDimensions(layout->getContentSize().width * 0.15f, layout->getContentSize().height / 2);
		layout->addChild(lbstartl);

		Label* lbendl = Label::createWithTTF("End", "STLITI.TTF", 30);
		lbendl->enableShadow(Color4B::GRAY, Size(10, 10));
		lbendl->enableOutline(Color4B::BLACK, 3);
		lbendl->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lbendl->setPosition(layout->getContentSize().width * 0.882f, layout->getContentSize().height*3 / 4 - 1);
		lbendl->setDimensions(layout->getContentSize().width * 0.15f, layout->getContentSize().height / 2);
		layout->addChild(lbendl);

		Label* lbstart;
		if (i == 0)
			lbstart = Label::createWithTTF("00:00:00", "STLITI.TTF", 30);
		else
			lbstart = Label::createWithTTF(acts.at(i - 1).second, "STLITI.TTF", 30);
		lbstart->enableShadow(Color4B::GRAY, Size(10, 10));
		lbstart->enableOutline(Color4B::BLACK, 3);
		lbstart->setAlignment(TextHAlignment::CENTER, TextVAlignment::CENTER);
		lbstart->setPosition(layout->getContentSize().width * 0.646f, layout->getContentSize().height / 4 - 1);
		lbstart->setDimensions(layout->getContentSize().width * 0.15f, layout->getContentSize().height/2);
		layout->addChild(lbstart);
		
		Label* lbend;
		if (i == acts.size() - 1)
			lbend = Label::createWithTTF("00:00:00", "STLITI.TTF", 30);
		else
			lbend = Label::createWithTTF(acts.at(i + 1).second, "STLITI.TTF", 30);
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
void GameScene::updateEditorUILayer()
{
	mLayerUIEditor->removeAllChildren();
	Size visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	//底部物件UI
	Layout* layout_title = Layout::create();
	layout_title->setSize(Size(visibleSize.width, visibleSize.width * 0.1f));
	layout_title->setPosition(Vec2(0.0f, visibleSize.height * 0.02f));
	layout_title->setBackGroundImageScale9Enabled(true);
	layout_title->setBackGroundImage("sliderProgress.png");
	layout_title->setOpacity(50);
	mLayerUIEditor->addChild(layout_title);

	//RadioButtonGroup
	RadioButtonGroup* radioButtonGroup = RadioButtonGroup::create();
	layout_title->addChild(radioButtonGroup);
	radioButtonGroup->addEventListener(CC_CALLBACK_3(GameScene::onChangedEditorUIRadioButtonGroup, this));
	for (int i = 0; i < 8; i++)
	{
		Layout* layout = Layout::create();
		layout->setSize(Size(visibleSize.width * 0.09f, visibleSize.width*0.09f));
		layout->setPosition(Vec2(visibleSize.width*0.125f*i + visibleSize.width*0.0175f, 0.0f));
		layout->setBackGroundImageScale9Enabled(true);
		layout->setBackGroundImage("gray.png");
		layout->setOpacity(50);
		
		auto filePathNormal = StringUtils::format("editor_ui_radio_button%1d.png", i + 1);
		auto filePathSelected = StringUtils::format("editor_ui_radio_button%1ds.png", i + 1);
		RadioButton* rb = RadioButton::create(filePathNormal, filePathSelected);
		rb->setPosition(Vec2(visibleSize.width*0.045f, visibleSize.width*0.045f));
		//rb->setScale();
		//rb->setZoomScale();
		radioButtonGroup->addRadioButton(rb);

		layout->addChild(rb);
		layout_title->addChild(layout);
	}
	radioButtonGroup->setSelectedButton(0);

	//保存按钮
	Button* button_save = Button::create("button_save.png", "button_save.png");
	button_save->setPosition(Vec2(visibleSize.width*0.05f, visibleSize.height*0.26f));
	button_save->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventEditorUISave, this));
	mLayerUIEditor->addChild(button_save);

	//取消按钮
	Button* button_cancel = Button::create("button_cancel.png", "button_cancel.png");
	button_cancel->setPosition(Vec2(visibleSize.width*0.05f, visibleSize.height*0.22f));
	button_cancel->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventEditorUICancel, this));
	mLayerUIEditor->addChild(button_cancel);

	//移除按钮
	Button* button_remove = Button::create("button_remove.png", "button_remove.png");
	button_remove->setPosition(Vec2(visibleSize.width*0.95f, visibleSize.height*0.26f));
	button_remove->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventEditorUIRemove, this));
	mLayerUIEditor->addChild(button_remove);

	//删除模式按钮
	Button* button_delete = Button::create("button_delete.png", "button_delete.png");
	button_delete->setPosition(Vec2(visibleSize.width*0.95f, visibleSize.height*0.22f));
	button_delete->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventEditorUIDelete, this));
	mLayerUIEditor->addChild(button_delete);
}
void GameScene::updateUITestDominoLayer()
{
	mLayerUITestDomino->removeAllChildren();

	Button* button_go = Button::create("button_play_domino.png", "button_play_domino.png");
	button_go->setPosition(Vec2(0.0f, 0.0f));
	button_go->addTouchEventListener(CC_CALLBACK_2(GameScene::touchEventUITestDominoGo, this));
	button_go->setZoomScale(0.4f);
	button_go->setPressedActionEnabled(true);
	mLayerUITestDomino->addChild(button_go);
}
void GameScene::updateEditorLayer()
{
	mLayerEditor->removeAllChildren();

	//这个是临时测试数据
	//地图上布满朝向为↘的牌
	char mapStr[900] = {0};
	memset(mapStr, 7, 900);

	int mapSize = sqrt(sizeof(mapStr));

	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesMoved = CC_CALLBACK_2(GameScene::onEditorLayerTouchesMoved, this);
	listener->onTouchesBegan = CC_CALLBACK_2(GameScene::onEditorLayerTouchesBegan, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, mLayerEditor);

	TMXTiledMap* map = TMXTiledMap::create("tile.tmx");
	auto layer = map->getLayer("map");
	map->setScale(0.9275f);

	Size visibleSize = Director::getInstance()->getVisibleSize();
	
	auto ts = map->getTileSize();
	auto ms = map->getMapSize();
	map->setPosition(visibleSize.width / 2 - ts.width*ms.width / 2, visibleSize.height / 2 - ts.height*ms.height / 2);

	mLayerEditor->addChild(map, 0, kTagTileMap);

	//初始化DominoManager
	DominoManager::getSingleton().setTileWidth(0, ms.width);
	DominoManager::getSingleton().setTileHeight(0, ms.height);
	DominoManager::getSingleton().setCurEditorTileIndex(0);
}
void GameScene::updateDominoTilemap()
{
	TMXTiledMap* map = static_cast<TMXTiledMap*>(mLayerEditor->getChildByTag(kTagTileMap));
	auto layer = map->getLayer("map");
	int width = DominoManager::getSingleton().getTileWidth(0);
	int height = DominoManager::getSingleton().getTileHeight(0);

	TileMap& tilemap = DominoManager::getSingleton().getTileMap(0);
	TileMapStr::iterator iter = tilemap.map.begin();
	int index = 0;
	int size = tilemap.map.size();
	for (; iter != tilemap.map.end(); ++iter)
	{
		unsigned char tile = *iter;
		if ((tile & 0x01) == 0)
		{
			tile = 1;
		}
		else
		{
			tile = (0x03 & (tile >> 1));
			switch (tile)
			{
			case 0:
				tile = 8;
				break;
			case 1:
				tile = 5;
				break;
			case 2:
				tile = 6;
				break;
			case 3:
				tile = 7;
				break;
			default:
				tile = 1;
				break;
			}
		}
		layer->setTileGID(tile, Vec2(index/width, index%width));
		index++;
	}
}

void GameScene::onEditorLayerTouchesMoved(const std::vector<Touch*>& touches, Event  *event)
{
	auto winSize = Director::getInstance()->getWinSize();
	TMXTiledMap* node = static_cast<TMXTiledMap*>(mLayerEditor->getChildByTag(kTagTileMap));

	if (touches.size() == 1)
	{
		Touch* touch = touches[0];

		auto diff = touch->getDelta();
		auto currentPos = node->getPosition();
		node->setPosition(currentPos + diff);
	}
	else if (touches.size() > 1)
	{
		Vec2 bgOrigin = Vec2(Vec2::ZERO);
		auto point1 = touches[0]->getLocation();
		auto point2 = touches[1]->getLocation();
		auto curDistance = point1.distance(point2);
		auto preDistance = touches[0]->getPreviousLocation().distance(touches[1]->getPreviousLocation());
		//两触摸点与原点的差向量，pointVec1和pointVec2是相对于bgSprite的位置
		auto pointVec1 = point1 - bgOrigin;
		auto pointVec2 = point2 - bgOrigin;
		//两触摸点的相对中点
		auto relMidx = (pointVec1.x + pointVec2.x) / 2;
		auto relMidy = (pointVec1.y + pointVec2.y) / 2;
		// 计算bgSprite的锚点
		auto anchorX = relMidx / node->getBoundingBox().size.width;
		auto anchorY = relMidy / node->getBoundingBox().size.height;
		// 相对屏幕的中点
		auto absMidx = (point2.x + point1.x) / 2;
		auto absMidy = (point2.y + point1.y) / 2;
		// 缩放时，为了避免出现空白的区域，需要做以下的边界处理。
		// 当bgSprite快要进入到屏幕时，修改bgSprite的位置（既absMidx和absMidy）。
		if ( bgOrigin.x > 0)
		{
			absMidx -= bgOrigin.x;
		}
		if (bgOrigin.x < -node->getBoundingBox().size.width + winSize.width)
		{
			absMidx += -node->getBoundingBox().size.width + winSize.width - bgOrigin.x;
		}
		if ( bgOrigin.y > 0)
		{
			absMidy -= bgOrigin.y;
		}
		if (bgOrigin.y < -node->getBoundingBox().size.height + winSize.height)
		{
			absMidy += -node->getBoundingBox().size.height + winSize.height - bgOrigin.y;
		}
		// 重设bgSprite锚点和位置
		node->setAnchorPoint(Vec2(anchorX, anchorY));
		node->setPosition(Vec2(absMidx, absMidy));
		// 根据两触摸点前后的距离计算缩放倍率
		auto scale = node->getScale() * (curDistance / preDistance);
		// 控制缩放倍率在1～4倍之间，最小倍率不能太小，不让背景将不能填充满整个屏幕。
		scale = MIN(4, MAX(1, scale));
		node->setScale(scale);
		// 更新原点位置
		bgOrigin = Vec2(absMidx, absMidy) - Vec2(node->getBoundingBox().size.width * anchorX, node->getBoundingBox().size.height * anchorY);
	}
}
void GameScene::onEditorLayerTouchesBegan(const std::vector<Touch*>& touches, Event  *event)
{
	auto winSize = Director::getInstance()->getWinSize();
	TMXTiledMap* map = static_cast<TMXTiledMap*>(mLayerEditor->getChildByTag(kTagTileMap));
	auto point = map->getPosition();
	auto ms = map->getMapSize();

	CCPoint beginpos = touches[0]->getLocation();
	float x = beginpos.x - point.x;
	float y = beginpos.y - point.y;

	int mapWidth = map->getMapSize().width * map->getTileSize().width;
	int mapHeight = map->getMapSize().height * map->getTileSize().height;
	CCPoint pos = DominoManager::getSingleton().calculateCoordinate(x, y, mapWidth, mapHeight);
	int xx = pos.x;
	int yy = pos.y;

	auto layer = map->getLayer("map");

	if (xx >= 0 && yy >= 0 && xx < ms.width && yy < ms.height)
	{
		int type = DominoManager::getSingleton().getCurEditorTileType();
		if (type != -1)
			layer->setTileGID(type, Vec2(xx, yy));
	}
}
void GameScene::onChangedEditorUIRadioButtonGroup(cocos2d::ui::RadioButton* radioButton, int index, cocos2d::ui::RadioButtonGroup::EventType type)
{
	DominoManager::getSingleton().setCurEditorTileIndex(index);
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
		mUILayerManager->switchTo(1);
	}
		break;

	case Widget::TouchEventType::CANCELED:
		mLabel_state->setString(StringUtils::format("Touch Cancelled"));
		break;

	default:
		break;
	}
}
void GameScene::touchEventPH(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void GameScene::touchEventBJ(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		mUILayerManager->switchTo(2);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void GameScene::touchEventMY(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void GameScene::touchEventEditorUIMenuClose(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		mUILayerManager->switchTo(0);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void GameScene::touchEventEditorUIMenuEditor(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		mUILayerManager->switchTo(3);
		mLayerManager->switchTo(3);
		NetManager::getSingleton().cs_DominoDownloadTilemap();
	}
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void GameScene::touchEventEditorUIMenuClone(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void GameScene::touchEventEditorUIMenuTest(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		mUILayerManager->switchTo(4);
		mLayerManager->switchTo(3);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void GameScene::touchEventEditorUISave(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
	{
		TMXTiledMap* map = static_cast<TMXTiledMap*>(mLayerEditor->getChildByTag(kTagTileMap));
		auto ms = map->getMapSize();
		auto layer = map->getLayer("map");
		TileMap& tilemap = DominoManager::getSingleton().getTileMap(0);
		tilemap.map.clear();
		for (int i = 0; i < ms.height; i++)
			for (int j = 0; j < ms.width; j++)
			{
				uint32_t gid = layer->getTileGIDAt(Vec2(i, j));
				unsigned char tile = (0x01 << 7);
				switch (gid)
				{
				case 1:
				case 2:
				case 3:
				case 4:
					break;
				case 5:
					tile |= (0x01 << 1 | 0x01);
					break;
				case 6:
					tile |= (0x02 << 1 | 0x01);
					break;
				case 7:
					tile |= (0x03 << 1 | 0x01);
					break;
				case 8:
					tile |= 0x01;
					break;
				}
				tilemap.map.push_back(tile);
			}
		NetManager::getSingleton().cs_DominoUploadTilemap();
	}
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void GameScene::touchEventEditorUISaveRes(int state)
{
	// 定义一个弹出层，传入一张背景图
	PopupLayer* pl = PopupLayer::create("gray.png");
	// ContentSize 是可选的设置，可以不设置，如果设置把它当作 9 图缩放
	pl->setContentSize(CCSizeMake(400, 350));
	pl->setTitle("Title");
	pl->setContentText("Message!", 20, 60, 250);
	// 设置回调函数，回调传回一个 CCNode 以获取 tag 判断点击的按钮
	// 这只是作为一种封装实现，如果使用 delegate 那就能够更灵活的控制参数了
	pl->setCallbackFunc(this, callfuncN_selector(GameScene::touchEventEditorUISaveResButtonCallback));
	// 添加按钮，设置图片，文字，tag 信息
	pl->addButton("gray.png", "gray.png", "OK", 0);
	pl->addButton("gray.png", "gray.png", "Cancel", 1);
	// 添加到当前层
	mLayerEditor->addChild(pl);

}
void GameScene::touchEventEditorUISaveResButtonCallback(Node *pNode)
{
}
void GameScene::touchEventEditorUICancel(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		mUILayerManager->switchTo(2);
		mLayerManager->switchTo(1);
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void GameScene::touchEventEditorUIRemove(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}
void GameScene::touchEventEditorUIDelete(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		break;

	case Widget::TouchEventType::CANCELED:
		break;

	default:
		break;
	}
}

void GameScene::touchEventUITestDominoGo(Ref *pSender, ui::Widget::TouchEventType type)
{
	switch (type)
	{
	case Widget::TouchEventType::BEGAN:
		break;

	case Widget::TouchEventType::MOVED:
		break;

	case Widget::TouchEventType::ENDED:
		break;

	case Widget::TouchEventType::CANCELED:
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
		mUILayerManager->switchTo(0);
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