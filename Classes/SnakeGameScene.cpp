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
                                           "arrow.png",
                                           "arrow_clicked.png",
                                           CC_CALLBACK_1(SnakeGame::menuCloseCallback, this));
    
	closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

	//score
	score_label = Label::createWithTTF("Score: 0", "fonts/Marker Felt.ttf", 24);
	score_label->setColor(Color3B(0, 0, 0));
	score_label->setPosition(Vec2(score_label->getContentSize().width / 2 + 10,
		origin.y + visibleSize.height - score_label->getContentSize().height));
	this->addChild(score_label, 1);
	
	//high score
	high_score_label = Label::createWithTTF("High Score: 0", "fonts/Marker Felt.ttf", 24);
	high_score_label->setColor(Color3B(0, 0, 0));
	high_score_label->setPosition(Vec2(origin.x + visibleSize.width - high_score_label->getContentSize().width / 2 - 10,
		origin.y + visibleSize.height - high_score_label->getContentSize().height));
	this->addChild(high_score_label, 1);

	drawBackground();

	apple = Sprite::create("snakes.png", Rect(0, SPRITE_HEIGHT * 3, SPRITE_WIDTH, SPRITE_HEIGHT));
	apple->setAnchorPoint(Vec2(0, 0));
	this->addChild(apple);

	iPhone = Sprite::create("phone.png");
	iPhone->setAnchorPoint(Vec2(0, 0));
	this->addChild(iPhone);

	mac = Sprite::create("mac.png");
	mac->setAnchorPoint(Vec2(0, 0));
	this->addChild(mac);

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

void SnakeGame::drawBackground()
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

	Size visibleSize = Director::getInstance()->getVisibleSize();
	int n = 5;
	int origin_x = 0;
	int origin_y = 0;

	for (int i = 0; i < n; i++)
	{
		if (i == 0)
		{
			origin_x = SPRITE_WIDTH * 4;
			origin_y = 0;
		}
		else if (i == n - 1)
		{
			origin_x = SPRITE_WIDTH * 4;
			origin_y = SPRITE_HEIGHT * 2;
		}
		else
		{
			origin_x = SPRITE_WIDTH;
			origin_y = 0;
		}
		auto mySprite = Sprite::create("snakes.png", Rect(origin_x, origin_y,
			SPRITE_WIDTH, SPRITE_HEIGHT));
		mySprite->setAnchorPoint(Vec2(0, 0));
		mySprite->setPosition(Vec2(150 + SPRITE_WIDTH * (n - i), visibleSize.height/2));
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
	bool isAppleDone = false;
	bool isIPhoneDone = false;
	bool isMacDone = false;

	while (!isAppleDone || !isIPhoneDone || !isMacDone)
	{
		Size s = Director::getInstance()->getVisibleSize();
		int x = rand() % ((int)s.width / SPRITE_WIDTH - 1) * SPRITE_WIDTH;
		int y = rand() % ((int)s.height / SPRITE_HEIGHT - 1) * SPRITE_HEIGHT;

		if (!isHitSnake(x, y))
		{
			if (!isAppleDone)
			{
				auto moveTo = MoveTo::create(0, Vec2(x, y));
				apple->runAction(moveTo);
				isAppleDone = true;
			}
			else if (!isIPhoneDone)
			{
				auto moveTo = MoveTo::create(0, Vec2(x, y));
				iPhone->runAction(moveTo);
				isIPhoneDone = true;
			}
			else if (!isMacDone)
			{
				auto moveTo = MoveTo::create(0, Vec2(x, y));
				mac->runAction(moveTo);
				isMacDone = true;
			}
			
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
		
		//delete the last body if didnt eat food
		Vec2 phone_p = iPhone->getPosition();
		Vec2 apple_p = apple->getPosition();
		Vec2 mac_p = mac->getPosition();
		if (x == apple_p.x && y == apple_p.y || x == phone_p.x && y == phone_p.y)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
				"gotItem.mp3");

			randomFood();
			score++;

			updateScore();
		}
		else if (x == mac_p.x && y == mac_p.y)
		{
			CocosDenshion::SimpleAudioEngine::sharedEngine()->playEffect(
				"gotItem.mp3");

			score += 5;
			randomFood();
			updateScore();
			if (snake.size() > 5)
			{
				this->removeChild(snake.at(snake.size() - 1));
				snake.pop_back();
			}
			this->removeChild(snake.at(snake.size() - 1));
			snake.pop_back();
		}
		else
		{
			this->removeChild(snake.at(snake.size() - 1));
			snake.pop_back();

			if (x < 0 || x > Director::getInstance()->getVisibleSize().width ||
				y < 0 || y > Director::getInstance()->getVisibleSize().height)
			{
				newGame();
				return;
			}
			else if (isHitSnake(x, y))
			{
				newGame();
				return;
			}
			
		}

		
		//add a new body in front
		auto mySprite = Sprite::create("snakes.png", Rect(SPRITE_WIDTH * 3, 0,
			SPRITE_WIDTH, SPRITE_HEIGHT));
		mySprite->setAnchorPoint(Vec2(0, 0));
		mySprite->setPosition(Vec2(x, y));
		this->addChild(mySprite);

		snake.insert(snake.begin(), mySprite);
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
	
	switch (keyCode)
	{
		case EventKeyboard::KeyCode::KEY_W :
		case EventKeyboard::KeyCode::KEY_UP_ARROW :
			if (direction == 3 || direction == 4)
			{
				direction = 1;
			}
			break;
		case EventKeyboard::KeyCode::KEY_S :
		case EventKeyboard::KeyCode::KEY_DOWN_ARROW :
			if (direction == 3 || direction == 4)
			{
				direction = 2;
			}
			break;
		case EventKeyboard::KeyCode::KEY_A :
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW :
			if (direction == 1 || direction == 2)
			{
				direction = 3;
			}
			break;
		case EventKeyboard::KeyCode::KEY_D:
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW :
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
	auto director = Director::getInstance();
	auto scene = GameMenu::createScene();

	// run
	director->replaceScene(scene);
}



