#pragma once

#include "craft.h"
#include "Bullet.h"
#include <list>

class BaseAI : public Object
{
public:
	BaseAI(Craft* craft, int hp);

	~BaseAI();

	bool Damage(int damage);

	Vec4f GetPos();

	void SetPlayerPos(Vec4f pos);

	/**
	*  \brief  ��ײ���
	*
	*  \param  point Ŀ���
	*
	*  \param  radius Ŀ�����ײ�뾶
	*
	*  \param  isDead ��û�л�ɱthis
	*
	*  \return ������ײ����������СΪ0
	*/
	//�ж�point�Ƿ��ڷɴ��ı���
	//�ð뾶ʲô�Ŀ�һ�¾��У���Ҫ��ܾ�ȷ
	int Collide(_In_ const Vec4f& point, float radius, bool& isDead);

	bool Collide(_In_ const Vec4f& point);

protected:
	Vec4f m_playerPos;

	Craft* m_craft;
	std::list<Bullet*> m_bullets;

	int m_MaxHP;
	int m_curHP;
};