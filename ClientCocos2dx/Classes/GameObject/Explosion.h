#ifndef __EXPLOSION_H__
#define __EXPLOSION_H__

#include "Base\Definitions.h"
#include "Base\GameObject.h"

class Explosion : public GameObject
{
public:
	Explosion();
	~Explosion();

	static Explosion* create(bool big = false);

	virtual bool init() override;

	void removeSelf(float dt);
	void setBig(bool big);
private:
	bool _isBig;
};

#endif // !__EXPLOSION_H__
