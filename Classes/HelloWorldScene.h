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
	bool toggle_round_end = false;
	long sec;
	float sec_sub;
	static Scene* createScene();

	virtual bool init();
	bool init_other();
	bool init_round();
	bool init_background();
	bool init_ast(Vec2 point);

	bool createBox2dWorld();
	bool setBox2dWorld();

	bool addNewBody(Vec2 point);

	bool dragFunc();
	bool round_end();
	bool changeScene();

	bool onTouchBegan(Touch* touch, Event* event);
	bool onTouchMoved(Touch* touch, Event* event);
	bool onTouchEnded(Touch* touch, Event* event);
	virtual void onKeyPressed(EventKeyboard::KeyCode, Event*);
	virtual void onKeyReleased(EventKeyboard::KeyCode, Event*);

	void tick(float dt);

	virtual float32 ReportFixture(b2Fixture* fix, const b2Vec2& point, const b2Vec2& normal, float32 action);
	
	b2World* world;
	b2Vec2 drawP1;
	b2Vec2 drawP2;
	Vec2 TOUCH_POINT;
	Vec2 DRAG_POINT;
	Vec2 DRAG_VECTOR;
	Vec2 TOUCH_END_POINT;
	bool toggle_drag = false;

	CREATE_FUNC(HelloWorld);

};

class ITEMS : public HelloWorld{

	static ITEMS* item;
	static ITEMS* getInstance();
};
