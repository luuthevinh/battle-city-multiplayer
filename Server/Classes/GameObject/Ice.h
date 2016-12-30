#ifndef __ICE_H__
#define __ICE_H__

#include "Wall.h"

class Ice : public Wall
{
public:
	Ice();
	~Ice();

	virtual bool init() override;
	virtual void update(float dt) override;

private:

};


#endif // !__ICE_H__
