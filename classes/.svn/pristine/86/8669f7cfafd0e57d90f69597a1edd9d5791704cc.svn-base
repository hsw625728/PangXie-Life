#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "Singleton.h"
#include "cocos2d.h"
#include "ui\UIWidget.h"
#include "ui\UIListView.h"
#include "2d\CCLayer.h"
#include "ui\UIRadioButton.h"

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
	void updateEditorUIMenuLayer();
	void updateEditorUILayer();
	void updateEditorLayer();
	void onEditorLayerTouchesMoved(const std::vector<Touch*>& touches, Event  *event);
	void onEditorLayerTouchesBegan(const std::vector<Touch*>& touches, Event  *event);
	void onChangedEditorUIRadioButtonGroup(ui::RadioButton* radioButton, int index, ui::RadioButtonGroup::EventType type);
	void touchEvent(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventPH(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventBJ(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventMY(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventEditorUIMenuClose(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventEditorUIMenuEditor(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventEditorUIMenuClone(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventEditorUIMenuTest(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventEditorUISave(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventEditorUICancel(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventEditorUIRemove(Ref *pSender, ui::Widget::TouchEventType type);
	void touchEventEditorUIDelete(Ref *pSender, ui::Widget::TouchEventType type);
	void closeScheduleBtn(Ref *pSender, ui::Widget::TouchEventType type);
	void addScheduleBtn(Ref* pSender, ui::Widget::TouchEventType type);

private:
	Label* mLabel_state;
	Layer* mLayerUI;
	Layer* mLayerUISchedule;
	Layer* mLayerUIEditorMenu;
	Layer* mLayerUIEditor;
	Layer* mLayerEditor;

	Layer* mLayerFI;
	Layer* mLayerSP;
	Layer* mLayerBI;
	Layer* mLayerBG;
	LayerMultiplex* mUILayerManager;
	LayerMultiplex* mLayerManager;
};

#endif