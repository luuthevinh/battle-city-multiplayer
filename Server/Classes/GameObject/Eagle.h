#ifndef __EAGLE_H__
#define __EAGLE_H__

#include "..\Base\GameObject.h"

class Eagle : public GameObject
{
public:
	Eagle();
	~Eagle();

	virtual bool init() override;
	virtual void setPosition(const Vector2& position) override;

	virtual void setStatus(eStatus status) override;
	virtual void gotHit(Damage* damage) override;
private:
	int _health;

	void updateBoundingBoxPosition();
	void updateWithStatus(eStatus status);
};

#endif // !__EAGLE_H__
