#pragma once

#include "craft.h"
#include "Bullet.h"
#include "BaseAI.h"
#include <list>

class SimpleEnemy : public BaseAI
{
public:
	~SimpleEnemy();

	static SimpleEnemy* create(Craft* craft, int hp);

	/**
	*  \brief  ˢ��һ֡
	*
	*  \param  interval	��֮֡���ʱ����
	*/
	void Frame(_In_ float interval);

	/**
	*  \brief  ��Ⱦ
	*/
	void Render();

	void Shoot();

private:
	SimpleEnemy(Craft* craft, int hp);

	float m_shootCD;
	const float SHOOT_CD;
};

inline SimpleEnemy* SimpleEnemy::create(Craft* craft, int hp){
	if (!craft)
		return NULL;
	SimpleEnemy* ret = new SimpleEnemy(craft, hp);
	ret->AddRef();
	return ret;
}