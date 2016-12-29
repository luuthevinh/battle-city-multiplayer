#ifndef __EAGLE_H__
#define __EAGLE_H__

#include "Base\GameObject.h"

class Eagle : public GameObject
{
public:
	Eagle();
	~Eagle();

	CREATE_FUNC(Eagle);

	static Eagle* createWithBufer(Buffer& buffer);

	virtual bool init() override;
	
private:
	int _health;
	void updateWithStatus(eStatus status);
};

#endif // !__EAGLE_H__
