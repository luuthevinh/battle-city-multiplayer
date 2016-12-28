#ifndef _SPRITE_MANAGER_
#define	_SPRITE_MANAGER_

#include <map>

#include "cocos2d.h"
#include "Definitions.h"

using namespace std;
USING_NS_CC;

class SpriteManager
{
public:
	static SpriteManager* getInstance();

	// Call this init at scene init
	void init();

	// get animate by name
	Animate* getAnimate(const std::string &animateName, int level = 0);

	// Get specifi Sprite by Id.
	Sprite* getSprite(eSpriteId id);

	std::string getObjectName(eObjectId id);

	~SpriteManager();

private:
	SpriteManager();

	static SpriteManager* _instance;

	// Init plist file
	void initResourceFile();

	// Init Animate and push to _animates list
	void initAnimate();

	void initSingleSprite();

	void mappingNames();
	
	Animate* createAniamte(char* framename, ...);
	Animate* createAniamte(float time, char* framename, ...);
	void addFrameCache(const std::string& plist);
	void addFrameCache(const std::string& plist, const std::string& imagefile);

	// list animate theo thứ tự cũng là level cho tank [0][1]...
	map<std::string, vector<Animate*>> _animates;

	// sprite
	map<eSpriteId, Sprite*> _sprites;

	map<eObjectId, std::string> _objectNames;
};
#endif // !_SPRITE_MANAGER_
