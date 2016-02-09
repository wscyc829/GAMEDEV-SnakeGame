#include "cocos2d.h"
#include "GameMenuScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

class SnakeGame : public cocos2d::Layer
{
	int SPRITE_WIDTH = 30;
	int SPRITE_HEIGHT = 30;
	int speed = 10;
	int speed_counter = 0;
	int direction;
	int score = 0;
	int hight_score = 0;

	std::vector<Sprite*> snake;
	Sprite* food;

	Label* score_label;
	Label* high_score_label;

public:
    static cocos2d::Scene* createScene();

    virtual bool init();
	void drawBackground();
	void newGame();
	void randomFood();
	int getDirection(int x1, int y1, int x2, int y2);
	bool isHitSnake(int x, int y);
	void update(float dt);
	void updateSprites();
	void updateScore();
	void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);
	
    // implement the "static create()" method manually
    CREATE_FUNC(SnakeGame);
};

