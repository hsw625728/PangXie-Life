#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Singleton.h"
#include "cocos2d.h"
#include "cocos2d\cocos\ui\UIWidget.h"
#include "cocos2d\cocos\ui\UIListView.h"
#include "cocos2d\cocos\2d\CCLayer.h"

using namespace cocos2d;
using namespace ui;

class GameScene : public cocos2d::Scene, public Singleton<GameScene>
{
public:
	GameScene();
	~GameScene();
	virtual bool init();
	void onEnter();
	void updatePlayerState(std::string& msg);
	CREATE_FUNC(GameScene);
	void updateScheduleListView();
	void touchEvent(Ref *pSender, ui::Widget::TouchEventType type);
	void closeScheduleBtn(Ref *pSender, ui::Widget::TouchEventType type);
	void addScheduleBtn(Ref* pSender, ui::Widget::TouchEventType type);
private:
	Label* mLabel_state;
	Layer* mLayerUI;
	Layer* mLayerUISchedule;

	Layer* mLayerFI;
	Layer* mLayerSP;
	Layer* mLayerBI;
	Layer* mLayerBG;
	LayerMultiplex* mLayerManager;
};

#endif