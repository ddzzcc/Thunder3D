#pragma once

#include "craft.h"
#include "Bullet.h"
#include "BaseAI.h"
#include "Strip.h"
#include <list>

///////////////////////////////////////////
//Boss��Ϊģʽ��
//
//��һ�׶�(>50%)��λ�û���Ư�ƣ��ӵ�������ģʽ
//ģʽһ�������ٶ�ת��player�����һ��ָ��player��׷�����ߣ�һ��ʱ���ֹͣ׷�٣�
//��������ͣ�ٺ�������λ��������������չ����ʽ��������ӵ�
//ģʽ�����Լ����ٶ�ת��player������ͣ�ٺ����ʮ��ն��״���ӵ�
//
//�ڶ��׶�(10%-50%)��ÿ��һ��ʱ�����һ��˲�ƣ�˲�Ƶص������ʾ���ӵ�������ģʽ
//ģʽһ�������ٶ�ת��player�����һ��ָ��player��׷�����ߣ�һ��ʱ���ֹͣ׷�٣�
//��������ͣ�ٺ��������λ�� ���� ����������������չ����ʽ��������ӵ�
//ģʽ����������ת�����ӵ�����
//
//�����׶Σ�<10%��׼��ʱ��ϳ�����λ�ù̶���������ת׷��playerͬʱ��������ӵ���ÿ��5sֹͣ2s
//////////////////////////////////////////

class Boss : public BaseAI
{
public:
	~Boss();

	static Boss* create(Craft* craft, int hp);

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

	int GetHP();

	int Collide(_In_ const Vec4f& point, float radius, bool& isDead);

	bool Collide(_In_ const Vec4f& point);

private:
	Boss(Craft* craft, int hp);

	void DrawLockBeam();

	void DrawPyramin(float time);

	void Shoot(Vec4f sPos, Vec4f v, float lifetime);

	void RectShoot(Vec4f up, Vec4f right, Vec4f v);

	void RanV();

	void Status1(float interval);
	void Status2(float interval);
	void Status3(float interval);

	int m_status;
	int m_subStatus;

	Vec4f m_v;

	float m_cdTime;
	int m_flag[3][3];	//���ؿ��������״̬
	float m_timer[3][4];//���ؿ�������ʱ
	float m_shootCD[3][3];//���ؿ��������cd

	Vec4f m_lastPPos;	//��׷��������

	//״̬һ
	int m_frameCount1_2;
	
	//״̬��
	int m_movePosX, m_movePosY, m_movePosZ;
	float m_shootAng2_1;
	Vec4f m_tarVec2_2;
};

inline Boss* Boss::create(Craft* craft, int hp)
{
	if (!craft)
		return NULL;
	Boss* ret = new Boss(craft, hp);
	ret->AddRef();
	return ret;
}

inline int Boss::GetHP()
{
	return m_curHP;
}
