#include "Clear_scene.h"
#include "HelloWorldScene.h"

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

	CCLOG("SCENE CHANGED");

	auto label_BMFont = LabelBMFont::create("Round Clear", "bitmapFontTest2.fnt");

	auto spr_star_1 = Sprite::create("star.png");
	spr_star_1->setRotation(-45);
	spr_star_1->setPosition(winsize.width / 2 - 150, winsize.height / 4);
	auto spr_star_2 = Sprite::create("star.png");
	spr_star_2->setPosition(winsize.width / 2, (winsize.height / 4) + 50);
	auto spr_star_3 = Sprite::create("star.png");
	spr_star_3->setRotation(45);
	spr_star_3->setPosition(winsize.width / 2 + 150, winsize.height / 4);

	star.push_back(spr_star_1); this->addChild(spr_star_1);
	star.push_back(spr_star_2); this->addChild(spr_star_2);
	star.push_back(spr_star_3); this->addChild(spr_star_3);

	for (auto i : star) {
		i->setScale(0.3);
	}
	label_BMFont->setPosition(winsize.width / 2, winsize.height - winsize.height / 4);
	this->addChild(label_BMFont);

	return true;
}
