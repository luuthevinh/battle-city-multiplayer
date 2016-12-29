#ifndef __OVERSCENE_H__
#define __OVERSCENE_H__

#include "..\Base\Scene.h"

class OverScene : public Scene
{
public:
	OverScene();
	~OverScene();

	// Inherited via Scene
	virtual bool init() override;
	virtual void update(float dt) override;
	virtual void destroy() override;

	void handleData(Serializable * object) override;

private:


};

#endif // !__OVERSCENE_H__
