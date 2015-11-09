#include "MessageBoxLayer.h"

PopupLayer::PopupLayer() :
m__pMenu(NULL)
, m_contentPadding(0)
, m_contentPaddingTop(0)
, m_callbackListener(NULL)
, m_callback(NULL)
, m__sfBackGround(NULL)
, m__s9BackGround(NULL)
, m__ltContentText(NULL)
, m__ltTitle(NULL)
{

}

PopupLayer::~PopupLayer()
{
	CC_SAFE_RELEASE(m__pMenu);
	CC_SAFE_RELEASE(m__sfBackGround);
	CC_SAFE_RELEASE(m__ltContentText);
	CC_SAFE_RELEASE(m__ltTitle);
	CC_SAFE_RELEASE(m__s9BackGround);
}

bool PopupLayer::init()
{
	if (!Layer::init()){
		return false;
	}
	this->setContentSize(Size(Vec2::ZERO));

	// 初始化需要的 Menu
	CCMenu* menu = CCMenu::create();
	menu->setPosition(Vec2::ZERO);
	setMenuButton(menu);

	setTouchEnabled(true);

	return true;
}

bool PopupLayer::onTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

PopupLayer* PopupLayer::create(const char *backgroundImage)
{
	PopupLayer* ml = PopupLayer::create();
	ml->setSpriteBackGround(CCSprite::create(backgroundImage));
	ml->setSprite9BackGround(ui::Scale9Sprite::create(backgroundImage));
	return ml;
}

void PopupLayer::setTitle(const char *title, int fontsize)
{
	CCLabelTTF* ltfTitle = CCLabelTTF::create(title, "STLITI.TTF", fontsize);
	setLabelTitle(ltfTitle);
}

void PopupLayer::setContentText(const char *text, int fontsize, int padding, int paddingTop)
{
	CCLabelTTF* ltf = CCLabelTTF::create(text, "STLITI.TTF", fontsize);
	setLabelContentText(ltf);
	m_contentPadding = padding;
	m_contentPaddingTop = paddingTop;
}

void PopupLayer::setCallbackFunc(cocos2d::CCObject *target, SEL_CallFuncN callfun)
{
	m_callbackListener = target;
	m_callback = callfun;
}


bool PopupLayer::addButton(const char *normalImage, const char *selectedImage, const char *title, int tag)
{
	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);

	// 创建图片菜单按钮
	CCMenuItemImage* menuImage = CCMenuItemImage::create(normalImage, selectedImage, this, menu_selector(PopupLayer::buttonCallback));
	menuImage->setTag(tag);
	menuImage->setPosition(pCenter);

	// 添加文字说明并设置位置
	CCSize imenu = menuImage->getContentSize();
	CCLabelTTF* ttf = CCLabelTTF::create(title, "STLITI.TTF", 20);
	ttf->setColor(ccc3(0, 0, 0));
	ttf->setPosition(ccp(imenu.width / 2, imenu.height / 2));
	menuImage->addChild(ttf);

	getMenuButton()->addChild(menuImage);
	return true;
}

void PopupLayer::buttonCallback(cocos2d::CCObject *pSender)
{
	CCNode* node = dynamic_cast<CCNode*>(pSender);

	if (m_callback && m_callbackListener){
		(m_callbackListener->*m_callback)(node);
	}
	this->removeFromParent();
}

void PopupLayer::onEnter()
{
	CCLayer::onEnter();

	CCSize winSize = CCDirector::sharedDirector()->getWinSize();
	CCPoint pCenter = ccp(winSize.width / 2, winSize.height / 2);

	CCSize contentSize;
	// 设定好参数，在运行时加载
	if (getContentSize().equals(Size(Vec2::ZERO))) 
	{
		getSpriteBackGround()->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(getSpriteBackGround(), 0, 0);
		contentSize = getSpriteBackGround()->getTexture()->getContentSize();
	}
	else
	{
		ui::Scale9Sprite *background = getSprite9BackGround();
		background->setContentSize(getContentSize());
		background->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		this->addChild(background, 0, 0);
		contentSize = getContentSize();
	}


	// 添加按钮，并设置其位置
	this->addChild(getMenuButton());
	float btnWidth = contentSize.width / (getMenuButton()->getChildrenCount() + 1);

	Vector<Node*> vecArray = getMenuButton()->getChildren();
	CCObject* pObj = NULL;
	int i = 0;
	for (auto& e : vecArray)
	{
		CCNode* node = dynamic_cast<CCNode*>(e);
		node->setPosition(Point(winSize.width / 2 - contentSize.width / 2 + btnWidth*(i + 1), winSize.height / 2 - contentSize.height / 3));
		i++;
	}


	// 显示对话框标题
	if (getLabelTitle()){
		getLabelTitle()->setPosition(ccpAdd(pCenter, ccp(0, contentSize.height / 2 - 35.0f)));
		this->addChild(getLabelTitle());
	}

	// 显示文本内容
	if (getLabelContentText()){
		CCLabelTTF* ltf = getLabelContentText();
		ltf->setPosition(ccp(winSize.width / 2, winSize.height / 2));
		ltf->setDimensions(CCSizeMake(contentSize.width - m_contentPadding * 2, contentSize.height - m_contentPaddingTop));
		ltf->setHorizontalAlignment(kCCTextAlignmentLeft);
		this->addChild(ltf);
	}

	// 弹出效果
	CCAction* popupLayer = CCSequence::create(CCScaleTo::create(0.0, 0.0),
		CCScaleTo::create(0.06, 1.05),
		CCScaleTo::create(0.08, 0.95),
		CCScaleTo::create(0.08, 1.0), NULL);
	this->runAction(popupLayer);

}

void PopupLayer::onExit()
{
	CCLayer::onExit();
}