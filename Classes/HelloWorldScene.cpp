#include "Clear_scene.h"
#include "HelloWorldScene.h"

std::random_device rd;
std::mt19937_64 _MT19937(rd());

ITEMS* ITEMS::item = nullptr;

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
	listener->onTouchMoved = CC_CALLBACK_2(HelloWorld::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(HelloWorld::onTouchEnded, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);


	auto listener_keyboard = EventListenerKeyboard::create();
	listener_keyboard->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	listener_keyboard->onKeyReleased = CC_CALLBACK_2(HelloWorld::onKeyReleased, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener_keyboard, this);


	this->createBox2dWorld();
	this->setBox2dWorld();
	this->init_other();
	this->init_background();
	this->init_round();
	this->schedule(schedule_selector(HelloWorld::tick));

	return true;
}

bool HelloWorld::init_other() {

	drawP1 = b2Vec2(-100 / PTM_RATIO, -200 / PTM_RATIO);
	drawP2 = b2Vec2(-300 / PTM_RATIO, -200 / PTM_RATIO);

	auto draw = DrawNode::create();
	draw->drawLine(Vec2(drawP1.x*PTM_RATIO, drawP1.y*PTM_RATIO), Point(drawP2.x*PTM_RATIO, drawP2.y*PTM_RATIO), Color4F::RED);
	this->addChild(draw, 5, "draw");

	

	return true;
}

bool HelloWorld::init_round() {

	auto spr_endzone = Sprite::create("EndZone.png");
	MT19937 num(0, winsize.width - 10);
	MT19937 num2(0, winsize.height - 10);
	spr_endzone->setPosition(num(_MT19937), num2(_MT19937));
	spr_endzone->setScale(0.3);
	this->addChild(spr_endzone, 3, "endzone");

	
	for(int i=0;i<10;i++)
		this->init_ast(Vec2((float)num(_MT19937), (float)num2(_MT19937)));

	timeval tv;
	gettimeofday(&tv, nullptr);
	sec = tv.tv_usec / 1000 + tv.tv_sec * 1000;

	return true;
}

bool HelloWorld::init_background() {

	auto layer_background = Layer::create();
	this->addChild(layer_background);

	auto spr_bg1 = Sprite::create("bg_space.png");
	spr_bg1->setAnchorPoint(Point(0, 0));
	layer_background->addChild(spr_bg1);
	auto spr_bg2 = Sprite::create("bg_space_1.png");
	spr_bg2->setAnchorPoint(Point(0, 0));
	spr_bg2->setPosition(1900, 0);
	layer_background->addChild(spr_bg2);
	auto spr_bg3 = Sprite::create("bg_space.png");
	spr_bg3->setAnchorPoint(Point(0, 0));
	spr_bg3->setPosition(3800, 0);
	layer_background->addChild(spr_bg3);
	auto act_rep = RepeatForever::create(Sequence::create(MoveBy::create(500, Point(-3800, 0)), Place::create(Vec2(0, 0)), NULL));
	layer_background->runAction(act_rep);


	return true;
}

bool HelloWorld::init_ast(Vec2 point) {

	auto spr_astronauts = Sprite::create("astronauts.png");
	spr_astronauts->setScale(0.1);
	spr_astronauts->setPosition(point);
	MT19937 num(0, 360);
	spr_astronauts->setRotation(static_cast<float>(num(_MT19937)));
	CCLOG("%f", spr_astronauts->getRotation());
	this->addChild(spr_astronauts, 1, "astronauts");

	b2Body* body_dymic_ast;
	b2BodyDef bodydef_dymic_ast;
	
	bodydef_dymic_ast.userData = spr_astronauts;
	bodydef_dymic_ast.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
	bodydef_dymic_ast.type = b2_dynamicBody;
	bodydef_dymic_ast.linearDamping = 0.3f; //공기저항(운동벡터)
	bodydef_dymic_ast.angularDamping = 0.8f; //공기저항(회전벡터)
	
	body_dymic_ast = world->CreateBody(&bodydef_dymic_ast);

	b2FixtureDef fixdef_dymic_ast;
	b2CircleShape shape_dymic_ast;

	shape_dymic_ast.m_radius = (spr_astronauts->getContentSize().width) / 20 / PTM_RATIO;

	fixdef_dymic_ast.shape = &shape_dymic_ast;
	fixdef_dymic_ast.density = 1.0f;
	fixdef_dymic_ast.friction = 0.5f;
	fixdef_dymic_ast.restitution = 0.5f;

	body_dymic_ast->CreateFixture(&fixdef_dymic_ast);
	
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

bool HelloWorld::addNewBody(Vec2 point)
{
	if (toggle_keyboard == 1) {

		auto spr_ball_1 = Sprite::create("moon.png");
		spr_ball_1->setPosition(point);
		spr_ball_1->setScale(0.5);
		this->addChild(spr_ball_1, 5, "spr_ball_1");

		b2Body* body_dymic_ball;
		b2BodyDef bodydef_dymic_ball;

		bodydef_dymic_ball.userData = spr_ball_1;
		bodydef_dymic_ball.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
		bodydef_dymic_ball.type = b2_dynamicBody;
		bodydef_dymic_ball.linearDamping = 0.65f; //공기저항(운동벡터)
		bodydef_dymic_ball.angularDamping = 0.5f; //공기저항(회전벡터)

		body_dymic_ball = world->CreateBody(&bodydef_dymic_ball);

		b2FixtureDef fixdef_dymic_ball;
		b2CircleShape shape_dymic_ball;

		shape_dymic_ball.m_radius = (spr_ball_1->getContentSize().width) / 4.0 / PTM_RATIO;

		fixdef_dymic_ball.shape = &shape_dymic_ball;
		fixdef_dymic_ball.density = 0.5f;
		fixdef_dymic_ball.friction = 0.2f;
		fixdef_dymic_ball.restitution = 0.9f;

		body_dymic_ball->CreateFixture(&fixdef_dymic_ball);

	}

	if (toggle_keyboard == 2) {

		auto spr_ball_2 = Sprite::create("earth.png");
		spr_ball_2->setPosition(point);
		spr_ball_2->setScale(0.5);
		this->addChild(spr_ball_2, 5, "spr_ball_2");

		b2Body* body_dymic_ball_2;
		b2BodyDef bodydef_dymic_ball_2;

		bodydef_dymic_ball_2.userData = spr_ball_2;
		bodydef_dymic_ball_2.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
		bodydef_dymic_ball_2.type = b2_dynamicBody;
		bodydef_dymic_ball_2.linearDamping = 0.7f; 
		bodydef_dymic_ball_2.angularDamping = 0.7f;

		body_dymic_ball_2 = world->CreateBody(&bodydef_dymic_ball_2);

		b2FixtureDef fixdef_dymic_ball_2;
		b2CircleShape shape_dymic_ball_2;

		shape_dymic_ball_2.m_radius = (spr_ball_2->getContentSize().width) / 4.0 / PTM_RATIO;

		fixdef_dymic_ball_2.shape = &shape_dymic_ball_2;
		fixdef_dymic_ball_2.density = 1.0f;
		fixdef_dymic_ball_2.friction = 0.5f;
		fixdef_dymic_ball_2.restitution = 0.7f;

		body_dymic_ball_2->CreateFixture(&fixdef_dymic_ball_2);

	}

	if (toggle_keyboard == 3) {

		auto spr_astronauts = Sprite::create("astronauts.png");
		spr_astronauts->setScale(0.1);
		spr_astronauts->setPosition(point);
		MT19937 num(0, 360);
		spr_astronauts->setRotation(static_cast<float>(num(_MT19937)));
		CCLOG("%f", spr_astronauts->getRotation());
		this->addChild(spr_astronauts, 1, "astronauts");

		b2Body* body_dymic_ast;
		b2BodyDef bodydef_dymic_ast;

		bodydef_dymic_ast.userData = spr_astronauts;
		bodydef_dymic_ast.position.Set(point.x / PTM_RATIO, point.y / PTM_RATIO);
		bodydef_dymic_ast.type = b2_dynamicBody;
		bodydef_dymic_ast.linearDamping = 0.3f; //공기저항(운동벡터)
		bodydef_dymic_ast.angularDamping = 0.8f; //공기저항(회전벡터)

		body_dymic_ast = world->CreateBody(&bodydef_dymic_ast);

		b2FixtureDef fixdef_dymic_ast;
		b2CircleShape shape_dymic_ast;

		shape_dymic_ast.m_radius = (spr_astronauts->getContentSize().width) / 20 / PTM_RATIO;

		fixdef_dymic_ast.shape = &shape_dymic_ast;
		fixdef_dymic_ast.density = 1.0f;
		fixdef_dymic_ast.friction = 0.5f;
		fixdef_dymic_ast.restitution = 0.5f;

		body_dymic_ast->CreateFixture(&fixdef_dymic_ast);

	}

	return true;
}

bool HelloWorld::dragFunc(){


	return true;
}

bool HelloWorld::round_end() {

	auto spr_1 = (Sprite*)this->getChildByName("spr_ball_2"); if (spr_1 == nullptr)return true;
	auto spr_2 = (Sprite*)this->getChildByName("endzone");

	if (spr_1->getBoundingBox().containsPoint(spr_2->getPosition())&&!toggle_round_end) {

		toggle_round_end = true;
		auto particle = ParticleGalaxy::create();
		particle->setScale(2.5);
		particle->setPosition(spr_2->getPosition());
		this->addChild(particle, 5);

		CCLOG("ROUND_END");
		auto act_delay = DelayTime::create(2);
		auto seq_end = Sequence::create(act_delay, CallFunc::create(CC_CALLBACK_0(HelloWorld::changeScene, this)), nullptr);

		this->runAction(seq_end);

	}
	return false;
}

bool HelloWorld::changeScene()
{
	CCLOG("Change Scene");

	timeval tv;
	gettimeofday(&tv, nullptr);
	long sec_1 = tv.tv_usec / 1000 + tv.tv_sec * 1000;
	sec_sub = ((float)(sec_1 - sec) / (float)1000) - 2.0;
	
	CCLOG("sec :  %.3f ", sec_sub);

	Director::getInstance()->getEventDispatcher()->removeAllEventListeners();
	auto scene = TransitionFade::create(1, Clear_scene::createScene());
	Director::getInstance()->replaceScene(scene);

	return false;
}

bool HelloWorld::onTouchBegan(Touch* touch, Event* event) {

	Vec2 point = Vec2(touch->getLocation().x, touch->getLocation().y);
	CCLOG("TOUCH ( %.f , %.f )", point.x, point.y);
	
	TOUCH_POINT = point;

	this->addNewBody(point);

	return true;
}

bool HelloWorld::onTouchMoved(Touch * touch, Event * event){
	
	Vec2 point = Vec2(touch->getLocation().x, touch->getLocation().y);
	Vec2 drag_point = Vec2(point.x - TOUCH_POINT.x, point.y - TOUCH_POINT.y);

	auto spr = ((Sprite*)this->getChildByName("spr_ball_1"));
	if (spr == nullptr)return true;

	if (spr->getBoundingBox().containsPoint(TOUCH_POINT)) {

		toggle_drag = true;
		DRAG_POINT = point;
		DRAG_VECTOR = drag_point;
		CCLOG("DRAG : ( %.f , %.f )", drag_point.x, drag_point.y);

		drawP1 = b2Vec2(TOUCH_POINT.x / PTM_RATIO, TOUCH_POINT.y / PTM_RATIO);
		drawP2 = drawP1 + 2 * (drawP1 - b2Vec2(DRAG_POINT.x / PTM_RATIO, DRAG_POINT.y / PTM_RATIO));

		auto draw = ((DrawNode*)this->getChildByName("draw"));
		draw->clear();
		draw->drawLine(Vec2(drawP1.x*PTM_RATIO, drawP1.y*PTM_RATIO), Point(drawP2.x*PTM_RATIO, drawP2.y*PTM_RATIO), Color4F::RED);


	}

	return true;
}

bool HelloWorld::onTouchEnded(Touch * touch, Event * event){

	Vec2 point = Vec2(touch->getLocation().x, touch->getLocation().y);

	TOUCH_END_POINT = point;
	CCLOG("Ended : ( %.f , %.f )", point.x, point.y);

	if (toggle_drag) {

		for (b2Body*b = world->GetBodyList(); b; b = b->GetNext()) {

			if (b->GetUserData() != nullptr) {

				auto spr = (Sprite*)b->GetUserData();
				if (spr->getBoundingBox().containsPoint(TOUCH_POINT)) {

					b->ApplyForceToCenter(b2Vec2(-(DRAG_VECTOR.x) * 1, -(DRAG_VECTOR.y) * 1), true);
				}

			}
		}
	}
	
	toggle_drag = false;

	auto draw = ((DrawNode*)this->getChildByName("draw"));
	draw->clear();

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

	this->round_end();

}

float32 HelloWorld::ReportFixture(b2Fixture * fix, const b2Vec2 & point, const b2Vec2 & normal, float32 action)
{
	//fix->GetBody()->ApplyForceToCenter(b2Vec2(200, 0), true);

	return 0.0f;
}

ITEMS * ITEMS::getInstance()
{
	if (item == nullptr)
		item - new ITEMS;

	return item;
}
