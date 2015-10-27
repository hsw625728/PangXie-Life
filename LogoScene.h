#ifndef __LOGO_SCENE_H__
#define __LOGO_SCENE_H__

#include "cocos2d.h"

class LogoScene : public cocos2d::Scene
{
public:
	virtual bool init();
	void onEnter();
	CREATE_FUNC(LogoScene);
	void Step(float dt);
};

#endif