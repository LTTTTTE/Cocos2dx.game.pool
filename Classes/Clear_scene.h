#include "cocos2d.h"
#pragma execution_character_set("utf-8")

USING_NS_CC;

class Clear_scene : public Layer {

public:

		static Scene* createScene();
		virtual bool init();
		CREATE_FUNC(Clear_scene);

		std::vector<Sprite*> star;

};

