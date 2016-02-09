#include "Snake.h"

//initialize the first body
void Snake::initBody(struct SnakeBody *snake_bodies, int x, int y, Sprite* sprite)
{
	snake_bodies->x = x;
	snake_bodies->y = y;
	snake_bodies->sprite = sprite;
	snake_bodies->pPrevious = nullptr;
	snake_bodies->pNext = nullptr;
}

// add from back
void Snake::addBack(struct SnakeBody *snake_bodies, int x, int y, Sprite* sprite) 
{
	SnakeBody *newBody = new SnakeBody;
	newBody->x = x;
	newBody->y = y;
	newBody->sprite = sprite;
	newBody->pPrevious = nullptr;
	newBody->pNext = nullptr;

	SnakeBody *cur = snake_bodies;

	while (cur)
	{
		if (cur->pNext == nullptr) 
		{
			cur->pNext = newBody;
			newBody->pPrevious = cur;
			return;
		}
		cur = cur->pNext;
	} 
}

//add from front
void Snake::insertFront(struct SnakeBody **snake_bodies, int x, int y, Sprite* sprite) 
{
	SnakeBody *newBody = new SnakeBody;
	newBody->x = x;
	newBody->y = y;
	newBody->sprite = sprite;
	newBody->pPrevious = nullptr;
	newBody->pNext = *snake_bodies;
	(*snake_bodies)->pPrevious = newBody;
	*snake_bodies = newBody;
}

//search if there exist a body in (x, y)
struct SnakeBody* Snake::searchNode(struct SnakeBody *head, int x, int y) 
{
	SnakeBody *cur = head;
	while (cur) 
	{
		if (cur->x == x && cur->y == y)
		{
			return cur;
		}
		cur = cur->pNext;
	}
	return nullptr;
}

Snake::Snake()
{
	snake_bodies->x = 0;
	snake_bodies->y = 0;
	snake_bodies->sprite = nullptr;
	snake_bodies->pPrevious = nullptr;
	snake_bodies->pNext = nullptr;
}


Snake::~Snake()
{
}
