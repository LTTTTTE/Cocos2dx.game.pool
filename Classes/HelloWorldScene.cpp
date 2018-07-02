#include "HelloWorldScene.h"

Scene* HelloWorld::createScene()
{
	Scene* scene = HelloWorld::create();

	return scene;
}

bool HelloWorld::init()
{

	if (!Scene::init())
	{
		return false;
	}

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

	auto listener_keyboard = EventListenerKeyboard::create();
	listener_keyboard->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	listener_keyboard->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_keyboard, this);

	this->init_other();
	this->createBox2dWorld();
	this->setBox2dWorld();
	this->schedule(schedule_selector(HelloWorld::tick));

	return true;
}

bool HelloWorld::init_other() {

	drawP1 = b2Vec2(200 / PTM_RATIO, 100 / PTM_RATIO);
	drawP2 = b2Vec2(200 / PTM_RATIO, 300 / PTM_RATIO);

	auto draw = DrawNode::create();
	draw->drawLine(Vec2(drawP1.x*PTM_RATIO, drawP1.y*PTM_RATIO), Point(drawP2.x*PTM_RATIO, drawP2.y*PTM_RATIO), Color4F::RED);
	this->addChild(draw, 5, "draw");

	auto spr_globe = Sprite::create("Globe_48px.png");
	spr_globe->setScale(0.2);
	spr_globe->setPosition(drawP2.x*PTM_RATIO, drawP2.y*PTM_RATIO);

	auto spr_globe_1 = Sprite::create("Globe_48px.png");
	spr_globe_1->setScale(0.2);
	spr_globe_1->setPosition(drawP1.x*PTM_RATIO, drawP1.y*PTM_RATIO);

	this->addChild(spr_globe);
	this->addChild(spr_globe_1);

	return true;
}

bool HelloWorld::createBox2dWorld() {

	world = new b2World(b2Vec2(0, 0));
	world->SetAllowSleeping(true);
	world->SetContinuousPhysics(true);


	return true;
}

bool HelloWorld::setBox2dWorld() {

	b2BodyDef groundBodyDef;

	groundBodyDef.position.Set(0, 0);
	b2Body* groundBody = world->CreateBody(&groundBodyDef);


	b2EdgeShape groundEdge;

	b2FixtureDef boxShapeDef;

	boxShapeDef.shape = &groundEdge;
	boxShapeDef.density = 1.0;
	boxShapeDef.friction = 0;
	boxShapeDef.restitution = 0;

	groundEdge.Set(b2Vec2(0, 0), b2Vec2(winsize.width / PTM_RATIO, 0));

	groundBody->CreateFixture(&boxShapeDef);

	groundEdge.Set(b2Vec2(0, 0), b2Vec2(0, winsize.height / PTM_RATIO));

	groundBody->CreateFixture(&boxShapeDef);


	groundEdge.Set(b2Vec2(0, winsize.height / PTM_RATIO),

		b2Vec2(winsize.width / PTM_RATIO, winsize.height / PTM_RATIO));

	groundBody->CreateFixture(&boxShapeDef);


	groundEdge.Set(b2Vec2(winsize.width / PTM_RATIO, winsize.height / PTM_RATIO),

		b2Vec2(winsize.width / PTM_RATIO, 0));

	groundBody->CreateFixture(&boxShapeDef);
	
	return true;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {

	Vec2 point = Vec2(touch->getLocation().x, touch->getLocation().y);
	CCLOG("TOUCH ( %.f , %.f )", point.x, point.y);

	this->addNewBody(point);

	return true;
}

bool HelloWorld::addNewBody(Vec2 point)
{
	if (toggle_keyboard == 1) {

		auto spr_ball_1 = Sprite::create("ball_1.png");
		spr_ball_1->setPosition(point);
		this->addChild(spr_ball_1, 5, "spr_ball_1");

		b2Body* body_dymic_ball;
		b2BodyDef bodydef_dymic_ball;

		bodydef_dymic_ball.userData = spr_ball_1;
		bodydef_dymic_ball.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
		bodydef_dymic_ball.type = b2_dynamicBody;
		bodydef_dymic_ball.linearDamping = 0.3f; //공기저항(운동벡터)
		bodydef_dymic_ball.angularDamping = 0.5f; //공기저항(회전벡터)

		body_dymic_ball = world->CreateBody(&bodydef_dymic_ball);

		b2FixtureDef fixdef_dymic_ball;
		b2CircleShape shape_dymic_ball;

		shape_dymic_ball.m_radius = (spr_ball_1->getContentSize().width) / 2.0 / PTM_RATIO;

		fixdef_dymic_ball.shape = &shape_dymic_ball;
		fixdef_dymic_ball.density = 1.0f;
		fixdef_dymic_ball.friction = 0.2f;
		fixdef_dymic_ball.restitution = 0.9f;
		
		body_dymic_ball->CreateFixture(&fixdef_dymic_ball);

	}
	return true;
}

void HelloWorld::onKeyPressed(EventKeyboard::KeyCode e, Event *){
	
	if (e == EventKeyboard::KeyCode::KEY_1) {
		toggle_keyboard = 1;
		CCLOG("toggle_keyboard = 1");
	}
	if (e == EventKeyboard::KeyCode::KEY_2) {
		toggle_keyboard = 2;
		CCLOG("toggle_keyboard = 2");
	}
	if (e == EventKeyboard::KeyCode::KEY_3) {
		toggle_keyboard = 3;
		CCLOG("toggle_keyboard = 3");
	}
}

void HelloWorld::onKeyReleased(EventKeyboard::KeyCode e, Event *){

	toggle_keyboard = 0;
	
	CCLOG("toggle_keyboard OFF");
}

void HelloWorld::tick(float dt) {

	world->Step(dt, 8, 3);
	world->RayCast(this, drawP1, drawP2);

	for (b2Body* b = world->GetBodyList(); b; b = b->GetNext()) {

		if (b->GetUserData() != nullptr) {

			auto spr = (Sprite*)b->GetUserData();

			spr->setPosition(Vec2((b->GetPosition().x)*(PTM_RATIO), (b->GetPosition().y)*(PTM_RATIO)));
			spr->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
		}
	}

}

float32 HelloWorld::ReportFixture(b2Fixture * fix, const b2Vec2 & point, const b2Vec2 & normal, float32 action)
{
	fix->GetBody()->ApplyForceToCenter(b2Vec2(100, 0), true);

	return 0.0f;
}
