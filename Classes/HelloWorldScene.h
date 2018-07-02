#include "cocos2d.h"
#include "Box2D\Box2D.h"

#pragma execution_character_set("utf-8")

USING_NS_CC;

#define winsize Director::getInstance()->getWinSize()
#define MT19937 std::uniform_int_distribution<__int64>
#define PTM_RATIO 76.8

class HelloWorld : public Scene, public b2RayCastCallback {

public:

	int toggle_keyboard = 0;

	static Scene* createScene();

	virtual bool init();
	bool init_other();
	b2World* world;

	bool createBox2dWorld();
	bool setBox2dWorld();

	bool onTouchBegan(Touch* touch, Event* event);
	bool addNewBody(Vec2 point);
	virtual void onKeyPressed(EventKeyboard::KeyCode, Event*);
	virtual void onKeyReleased(EventKeyboard::KeyCode, Event*);

	void tick(float dt);



	virtual float32 ReportFixture(b2Fixture* fix, const b2Vec2& point, const b2Vec2& normal, float32 action);

	b2Vec2 drawP1;
	b2Vec2 drawP2;


	CREATE_FUNC(HelloWorld);


};

