#include "SnakeGameScene.h"
#include "iostream"
#include "SimpleAudioEngine.h"

Scene* SnakeGame::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = SnakeGame::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool SnakeGame::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    Size visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(SnakeGame::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	//score
	score_label = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
	score_label->setPosition(Vec2(score_label->getContentSize().width / 2 + 10,
		origin.y + visibleSize.height - score_label->getContentSize().height));
	this->addChild(score_label, 1);
	
	//high score
	high_score_label = Label::createWithTTF("High Score: 0", "fonts/Marker Felt.ttf", 24);
	high_score_label->setPosition(Vec2(origin.x + visibleSize.width - high_score_label->getContentSize().width / 2 - 10,
		origin.y + visibleSize.height - high_score_label->getContentSize().height));
	this->addChild(high_score_label, 1);

	drawBackGround();

	food = Sprite::create("snakes.png", Rect(0, SPRITE_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT));
	food->setAnchorPoint(Vec2(0, 0));
	this->addChild(food);

	srand(time(NULL));

	newGame();

	this->scheduleUpdate();

	// creating a keyboard event listener
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(SnakeGame::onKeyPressed, this);
	listener->onKeyReleased = CC_CALLBACK_2(SnakeGame::onKeyReleased, this);

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void SnakeGame::drawBackGround()
{
	Size visibleSize = Director::getInstance()->getVisibleSize();

	for (int i = 0; i < visibleSize.width; i += SPRITE_WIDTH)
	{
		for (int j = 0; j < visibleSize.height; j += SPRITE_HEIGHT)
		{
			int origin_x = 0;
			int origin_y = 0;

			if (i == 0)
			{
				if (j == 0)
				{
					origin_y = SPRITE_HEIGHT * 2;
				}
				else if (j > 0 && j < visibleSize.height - SPRITE_HEIGHT)
				{
					origin_y = SPRITE_HEIGHT;
				}
			}
			else if (i == visibleSize.width - SPRITE_WIDTH)
			{
				if (j == 0){
					origin_x = SPRITE_WIDTH * 2;
					origin_y = SPRITE_HEIGHT * 2;
				}
				else if (j > 0 && j < visibleSize.height - SPRITE_HEIGHT)
				{
					origin_x = SPRITE_WIDTH * 2;
					origin_y = SPRITE_HEIGHT;
				}
				else if (j == visibleSize.height - SPRITE_HEIGHT)
				{
					origin_x = SPRITE_WIDTH * 2;
				}
			}
			else if (j == 0)
			{
				if (i > 0 && i < visibleSize.width - SPRITE_WIDTH)
				{
					origin_x = SPRITE_WIDTH;
					origin_y = SPRITE_HEIGHT * 2;
				}
			}
			else if (j == visibleSize.height - SPRITE_HEIGHT)
			{
				if (i > 0 && i < visibleSize.width - SPRITE_WIDTH)
				{
					origin_x = SPRITE_WIDTH;
				}
			}
			else
			{
				origin_x = SPRITE_WIDTH;
				origin_y = SPRITE_HEIGHT;
			}
			auto mySprite = Sprite::create("bg.png", Rect(origin_x, origin_y, SPRITE_WIDTH, SPRITE_HEIGHT));
			mySprite->setAnchorPoint(Vec2(0, 0));
			mySprite->setPosition(Vec2(i, j));
			this->addChild(mySprite);
		}
	}
}
void SnakeGame::newGame()
{
	score = 0;
	direction = 4;
	randomFood();
	updateScore();
	//remove all sprites muna
	for (int i = 0; i < snake.size(); i++)
	{
		this->removeChild(snake.at(i));
	}

	snake.clear();
	int n = 5;
	for (int i = 0; i < n; i++)
	{
		auto mySprite = Sprite::create("snakes.png", Rect(SPRITE_WIDTH * 4, 0,
			SPRITE_WIDTH, SPRITE_HEIGHT));
		mySprite->setAnchorPoint(Vec2(0, 0));
		mySprite->setPosition(Vec2(SPRITE_WIDTH * (n - i), 0));
		snake.push_back(mySprite);
		this->addChild(mySprite);
	}
}

int SnakeGame::getDirection(int x1, int y1, int x2, int y2)
{
	if (x1 == x2)
	{
		if (y1 < y2)
		{
			return 1;
		}
		else
		{
			return 2;
		}
	}
	else
	{
		if (x1 < x2)
		{
			return 4;
		}
		else
		{
			return 3;
		}
	}
}

void SnakeGame::randomFood()
{
	bool isDone = false;

	while (!isDone)
	{
		Size s = Director::getInstance()->getVisibleSize();
		int x = rand() % ((int)s.width / SPRITE_WIDTH - 1) * SPRITE_WIDTH;
		int y = rand() % ((int)s.height / SPRITE_HEIGHT - 1) * SPRITE_HEIGHT;

		if (!isHitSnake(x, y))
		{
			auto moveTo = MoveTo::create(0, Vec2(x, y));
			food->runAction(moveTo);
			isDone = true;
		}
	}
}

bool SnakeGame::isHitSnake(int x, int y)
{
	for (int i = 0; i < snake.size(); i++)
	{
		Vec2 p = snake.at(i)->getPosition();

		if (x == p.x && y == p.y)
		{
			return true;
		}
	}
	return false;
}

void SnakeGame::update(float dt)
{
	
	if (speed_counter == speed)
	{
		Vec2 p = snake.at(0)->getPosition();

		int x = p.x;
		int y = p.y;

		switch (direction){
			case 1:
				y += SPRITE_HEIGHT;
				break;
			case 2:
				y -= SPRITE_HEIGHT;
				break;
			case 3:
				x -= SPRITE_WIDTH;
				break;
			case 4:
				x += SPRITE_WIDTH;
				break;
			default:
				break;
		}

		if (x < 0 || x > Director::getInstance()->getVisibleSize().width ||
			y < 0 || y > Director::getInstance()->getVisibleSize().height)
		{
			newGame();
		}
		else if (isHitSnake(x, y))
		{
			newGame();
		}
		else
		{
			//add a new body in front
			auto mySprite = Sprite::create("snakes.png", Rect(SPRITE_WIDTH * 3, 0,
				SPRITE_WIDTH, SPRITE_HEIGHT));
			mySprite->setAnchorPoint(Vec2(0, 0));
			mySprite->setPosition(Vec2(x, y));
			this->addChild(mySprite);

			snake.insert(snake.begin(), mySprite);

			//delete the last body if didnt eat food
			Vec2 p = food->getPosition();

			if (x == p.x && y == p.y)
			{
				CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
					"gotItem.mp3");
				
				randomFood();
				score++;

				updateScore();
			}
			else
			{
				this->removeChild(snake.at(snake.size() - 1));
				snake.pop_back();
			}
		}
		speed_counter = 0;

		updateSprites();
	}
	else
	{
		speed_counter++;
	}	
}

void SnakeGame::updateSprites()
{
	for (int i = 0; i < snake.size(); i++)
	{	
		int origin_x = 0;
		int origin_y = 0;

		Sprite* spr = snake.at(i);
		Vec2 cur = spr->getPosition();

		if (i == 0) //it is head
		{
			switch (direction)
			{
				case 1:
					origin_x = SPRITE_WIDTH * 3;
					origin_y = 0;
					break;
				case 2:
					origin_x = SPRITE_WIDTH * 4;
					origin_y = SPRITE_HEIGHT;
					break;
				case 3:
					origin_x = SPRITE_WIDTH * 3;
					origin_y = SPRITE_HEIGHT;
					break;
				case 4:
					origin_x = SPRITE_WIDTH * 4;
					origin_y = 0;
					break;
				default:
					break;
			}
		}
		else if (i == snake.size() - 1) //it is tail
		{
			Vec2 prev = snake.at(i - 1)->getPosition();
			int dir_front = getDirection(cur.x, cur.y, prev.x, prev.y);

			switch (dir_front)
			{
				case 1:
					origin_x = SPRITE_WIDTH * 3;
					origin_y = SPRITE_HEIGHT * 2;
					break;
				case 2:
					origin_x = SPRITE_WIDTH * 4;
					origin_y = SPRITE_HEIGHT * 3;
					break;
				case 3:
					origin_x = SPRITE_WIDTH * 3;
					origin_y = SPRITE_HEIGHT * 3;
					break;
				case 4:
					origin_x = SPRITE_WIDTH * 4;
					origin_y = SPRITE_HEIGHT * 2;
					break;
				default:
					break;
			}
		}
		else
		{
			Vec2 prev = snake.at(i - 1)->getPosition();
			Vec2 next = snake.at(i + 1)->getPosition();
			int dir_front = getDirection(cur.x, cur.y, prev.x, prev.y);
			int dir_back = getDirection(next.x, next.y, cur.x, cur.y);

			if (dir_front == dir_back)
			{
				switch (dir_front)
				{
					case 1:
					case 2:
						origin_x = SPRITE_WIDTH * 2;
						origin_y = SPRITE_HEIGHT;
						break;
					case 3:
					case 4:
						origin_x = SPRITE_WIDTH;
						origin_y = 0;
						break;
					default:
						break;
				}
			}
			else
			{
				if ((dir_front == 1 && dir_back == 3) || (dir_front == 4 && dir_back == 2))
				{
					origin_x = 0;
					origin_y = SPRITE_HEIGHT;
				}
				else if ((dir_front == 1 && dir_back == 4) || (dir_front == 3 && dir_back == 2))
				{
					origin_x = SPRITE_WIDTH * 2;
					origin_y = SPRITE_HEIGHT * 2;
				}
				else if ((dir_front == 2 && dir_back == 3) || (dir_front == 4 && dir_back == 1))
				{
					origin_x = 0;
					origin_y = 0;
				}
				else if ((dir_front == 2 && dir_back == 4) || (dir_front == 3 && dir_back == 1))
				{
					origin_x = SPRITE_WIDTH * 2;
					origin_y = 0;
				}
			}
		}
		spr->setTextureRect(Rect(origin_x, origin_y,
			SPRITE_WIDTH, SPRITE_HEIGHT));
	}
}

void SnakeGame::updateScore()
{
	std::string s = "Score: ";
	s += std::to_string(score);
	score_label->setString(s);

	if (score > hight_score)
	{
		hight_score = score;
		s = "High Score: ";
		s += std::to_string(hight_score);
		high_score_label->setString(s);
	}
}
// Implementation of the keyboard event callback function prototype
void SnakeGame::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event)
{

	switch ((int)keyCode)
	{
		case 28:
			if (direction == 3 || direction == 4)
			{
				direction = 1;
			}
			break;
		case 29:
			if (direction == 3 || direction == 4)
			{
				direction = 2;
			}
			break;
		case 26:
			if (direction == 1 || direction == 2)
			{
				direction = 3;
			}
			break;
		case 27:
			if (direction == 1 || direction == 2)
			{
				direction = 4;
			}
			break;
		default:
			break;
	}
}

void SnakeGame::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}


