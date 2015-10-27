#include "LoadingScene.h"
#include "GameScene.h"
#include "ui/UILoadingBar.h"
#include "NetWork.h"

using namespace cocos2d;
using namespace ui;

bool LoadingScene::init()
{
	if (Scene::init())
	{
		_totalNum = 10;
		_loadedNum = 0;
		return true;
	}
	else
	{
		return false;
	}
}
void LoadingScene::onEnter()
{
	Scene::onEnter();
	visibleSize = Director::getInstance()->getVisibleSize();
	origin = Director::getInstance()->getVisibleOrigin();
	
	//���ñ���
	layer = Layer::create();
	//��ͼ������ͼƬ��߱�����2��n���ݣ�128x128
	auto bg = Sprite::create("progress_bg_128.png", Rect(0, 0, visibleSize.width, visibleSize.height));
	//��ͼ�����������ˮƽ�ظ�ƽ�̣���ֱ�ظ�ƽ��
	Texture2D::TexParams tp = { GL_LINEAR, GL_LINEAR, GL_REPEAT, GL_REPEAT };
	bg->getTexture()->setTexParameters(tp);
	bg->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height / 2));
	layer->addChild(bg, 0);

	//���ý���������
	auto pr_con = Sprite::create("progress_container.png");
	pr_con->setPosition(origin + Vec2(visibleSize.width / 2, visibleSize.height / 2 - 200));
	layer->addChild(pr_con, 1);

	//���ý�����
	loadingBar = LoadingBar::create("Progress.png");
	loadingBar->setTag(0);
	loadingBar->setScale9Enabled(true);
	loadingBar->setCapInsets(Rect(4, 0, 115, 0));
	loadingBar->setContentSize(Size(320, 10));
	loadingBar->setPosition(origin + Vec2(visibleSize.width / 2 + 6, visibleSize.height / 2 - 200));
	loadingBar->setDirection(LoadingBar::Direction::LEFT);
	layer->addChild(loadingBar, 2);

	//���ý�����ͷ��
	loadinghead = LoadingBar::create("progress_head.png");
	loadinghead->setScale9Enabled(true);
	loadinghead->setCapInsets(Rect(1,1,1,1));
	loadinghead->setContentSize(Size(320, 10));
	loadinghead->setPosition(origin + Vec2(visibleSize.width / 2 + 27, visibleSize.height / 2 - 222));
	loadinghead->setDirection(LoadingBar::Direction::LEFT);
	layer->addChild(loadinghead, 3);

	auto label = Label::createWithTTF("L o a d i n g  . . .  . . .", "fonts/STLITI.TTF", 24);
	// position the label on the center of the screen
	label->setPosition(origin + Vec2(visibleSize.width / 2 + 4, visibleSize.height / 2 - 226));
	// add the label as a child to this layer
	layer->addChild(label, 4);


	percentLabel = Label::createWithTTF("0", "fonts/STLITI.TTF", 18);
	percentLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - percentLabel->getContentSize().height - 24));
	layer->addChild(percentLabel, 5);

	this->addChild(layer);
	//Ԥ������Դ
	this->loadingResource();
}

void LoadingScene::loadingResource(void)
{
	for (int i = 0; i < _totalNum; i++)
	{
		char string[32];
		sprintf(string, "%d.png", i);
		CCTextureCache::sharedTextureCache()->addImageAsync(string, CC_CALLBACK_0(LoadingScene::loadingCallBack, this));
	}
}
void LoadingScene::loadingCallBack(void)
{
	char string[32];
	sprintf(string, "%d", _loadedNum);
	percentLabel->setString(string);
	
	loadingBar->setPercent(_loadedNum);
	loadinghead->setPercent(_loadedNum);
	percentLabel->setPosition(origin.x + visibleSize.width / 2 - 156 + 3.2f * _loadedNum, visibleSize.height / 2 - 152);
	++_loadedNum;
	if (_loadedNum == _totalNum)
	{
		CCScene* scene = GameScene::create();
		//LogoScene ������ LoadingScene ����Ч�����ﴦ��
		CCDirector::sharedDirector()->replaceScene(CCTransitionFade::create(0.001f, scene));
	}
}
//�������첽���ز�ͬ��Դ�ķ���
/*
���ظ�һ����ػ��ƣ����е���Դ�ǰ�����صģ�����ͬʱ���ض���ļ���ִ���첽���صĴ���϶���д��void update(float f)�У�������֡���ء�
1.png���첽����

?
1
2
Director::getInstance()->getTextureCache()->addImageAsync(reloadImages[curReloadImgNum],
CC_CALLBACK_1(LoadingScene::imageAsyncCallback, this));



2.plist��pngһ��Ĵ����Դ
�ϻ����ֱ࣬���Ϸ�����û�м���plist��api������ʹ����1�ķ�������png��Ȼ���ڻص������м���plist��

?
1
2
Director::getInstance()->getTextureCache()->addImageAsync(reloadPlists[curReloadPlistNum]+"png",
CC_CALLBACK_1(LoadingScene::plistImageAsyncCallback, this));



ע��ص������Ĳ���������plist�첽���صĹؼ����β���png����

?
1
2
3
4
5
6
void LoadingScene::plistImageAsyncCallback(cocos2d::Texture2D* texture)
{
SpriteFrameCache::getInstance()->addSpriteFramesWithFile(reloadPlists[curReloadPlistNum].append("plist"), texture);
curReloadPlistNum++;
loading = true;
}


3.plist��pvr.cczһ��Ĵ����Դ
����2�ķ�����

?
1
2
3
4
5
6
7
8
Director::getInstance()->getTextureCache()->addImageAsync(reloadPvrPlists[curReloadPvrPlistNum] + "pvr.ccz",
CC_CALLBACK_1(LoadingScene::pvrPlistImageAsyncCallback, this));
void LoadingScene::pvrPlistImageAsyncCallback(cocos2d::Texture2D* texture)
{
SpriteFrameCache::getInstance()->addSpriteFramesWithFile(reloadPvrPlists[curReloadPvrPlistNum].append("plist"), texture);
curReloadPvrPlistNum++;
loading = true;
}

4.CocosStudio������.ExportJson��׺�Ķ����ļ�
?
1
2
3
4
5
6
7
ArmatureDataManager::getInstance()->addArmatureFileInfoAsync(reloadExportJsons[curReloadExportJsonNum],
this, schedule_selector(LoadingScene::jsonAsyncCallback));
void LoadingScene::jsonAsyncCallback(float f)
{
curReloadExportJsonNum++;
loading = true;
}

*/