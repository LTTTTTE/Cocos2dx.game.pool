#include "Clear_scene.h"
#include "HelloWorldScene.h"

/*void Clear_scene::instance(HelloWorld *p)
{
	sec = p->sec_sub;
}*/
//HelloWorld* hell = new HelloWorld();

Scene * Clear_scene::createScene()
{
	Scene* scene = Scene::create();

	Clear_scene* layer = Clear_scene::create();

	scene->addChild(layer);

	return scene;
}

bool Clear_scene::init()
{
	if (!Layer::init()) {

		return false;
	}
	//this->instance(hell);
	CCLOG("SCENE CHANGED");

	auto label_BMFont = LabelBMFont::create("Round Clear", "bitmapFontTest2.fnt");
	label_BMFont->setPosition(winsize.width / 2, winsize.height - winsize.height / 4);
	this->addChild(label_BMFont);

	auto label = Label::create("", "", 30);
	label->setString(StringUtils::format("½Ã°£ : %.3f sec", sec));
	label->setPosition(winsize.width / 2, winsize.height / 2);
	this->addChild(label);

	auto spr_star_1 = Sprite::create("star.png");
	spr_star_1->setRotation(-45);
	spr_star_1->setPosition(winsize.width / 2 - 150, winsize.height / 4);
	auto spr_star_2 = Sprite::create("star.png");
	spr_star_2->setPosition(winsize.width / 2, (winsize.height / 4) + 50);
	auto spr_star_3 = Sprite::create("star.png");
	spr_star_3->setRotation(45);
	spr_star_3->setPosition(winsize.width / 2 + 150, winsize.height / 4);

	star.push_back(spr_star_1);  this->addChild(spr_star_1);
	star.push_back(spr_star_2);  this->addChild(spr_star_2);
	star.push_back(spr_star_3);  this->addChild(spr_star_3);

	for (auto i : star) {
		i->setScale(0.001);
	}

	auto act_1 = ScaleTo::create(0.3f, 0.3);

	auto seq_1 = Sequence::create(DelayTime::create(1), act_1, nullptr);
	auto seq_2 = Sequence::create(DelayTime::create(1.5), act_1->clone(), nullptr);
	auto seq_3 = Sequence::create(DelayTime::create(2), act_1->clone(), nullptr);

	spr_star_1->runAction(seq_1);
	spr_star_2->runAction(seq_2);
	spr_star_3->runAction(seq_3);

	return true;
}
