#ifndef __LOADING_SCENE_H__
#define __LOADING_SCENE_H__

#include "cocos2d.h"
#include "ui/UILoadingBar.h"

class LoadingScene : public cocos2d::Scene
{
public:
	virtual bool init();
	void onEnter();
	CREATE_FUNC(LoadingScene);
private:
	void loadingResource(void);
	void loadingCallBack(void);

private:
	cocos2d::Size visibleSize;
	cocos2d::Vec2 origin;
	int _totalNum;
	int _loadedNum;
	cocos2d::Layer* layer;
	cocos2d::Label* percentLabel;
	cocos2d::ui::LoadingBar* loadingBar;
	cocos2d::ui::LoadingBar* loadinghead;
};

#endif