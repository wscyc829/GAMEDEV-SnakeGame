#pragma once
#include "cocos2d.h"

USING_NS_CC;

struct SnakeBody{
	int x;
	int y;
	Sprite* sprite;
	SnakeBody *pPrevious;
	SnakeBody *pNext;
};

class Snake
{
public:
	struct SnakeBody *snake_bodies = new SnakeBody;

	void initBody(struct SnakeBody *snake_bodies, int x, int y, Sprite* sprite);
	void addBack(struct SnakeBody *snake_bodies, int x, int y, Sprite* sprite);
	void insertFront(struct SnakeBody **snake_bodies, int x, int y, Sprite* sprite);
	struct SnakeBody* searchNode(struct SnakeBody *head, int x, int y);

	Snake();
	~Snake();
};

