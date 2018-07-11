#include "cocos2d.h"
#pragma execution_character_set("utf-8")

USING_NS_CC;

//class HelloWorld;

class Clear_scene : public Layer{

public:

//	HelloWorld * hell;
	float sec = 123.0;
	std::vector<Sprite*> star;

//	void instance(HelloWorld* p); 
	static Scene* createScene();
	virtual bool init();

	CREATE_FUNC(Clear_scene);
	

};

