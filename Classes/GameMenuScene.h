#pragma once

#include "cocos2d.h"

class GameMenu : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();

	virtual bool init();

	// a selector callback
	void startGameCallback(cocos2d::Ref* pSender);
	void menuCloseCallback(cocos2d::Ref* pSender);
	// implement the "static create()" method manually
	CREATE_FUNC(GameMenu);
	GameMenu();
	~GameMenu();
};