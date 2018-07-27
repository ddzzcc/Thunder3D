#pragma once

#include "object.h"
#include "camera.h"
#include "Partical.h"

#include <list>

class CollisionPartSys : public Object
{
private:
	//��������
	std::list<Particle> plist;

	//�����
	ICamera* camera;

	//��ʱ����
	Vec4f up, nVec;

	//��ǰʱ��
	float tim;

	//����ʱ��
	static const float TIME_THRES;

	//������ͼ
	unsigned int texture;

	float m_life;

	float m_collisionR;

	float m_lifeTime;

	//���һ������
	void AddParticle();

	CollisionPartSys(_In_ ICamera* camera, Vec4f pos);

public:
	~CollisionPartSys();

	//ǰ��һ֡
	void Frame(_In_ float interval);

	//��Ⱦ
	void Render();

	//��ʼ��
	bool Initialize(_In_ char* filename);

	void SetPos(Vec4f pos);

	void SetCollisionR(float r);

	void SetLifeTime(float time);

	bool IsExist();

	static CollisionPartSys* create(_In_ ICamera* camera, Vec4f pos);
};