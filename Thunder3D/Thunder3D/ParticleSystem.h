#ifndef _PARTICLESYSTEM_H
#define _PARTICLESYSTEM_H

#include "object.h"
#include "camera.h"
#include "craft.h"
#include <list>

class ParticleSystem : public Object{
private:
	struct Particle{
		//λ��
		Vec4f pos;

		//�ٶ�
		Vec4f speed;

		//����
		Vec4f force;

		//������
		float life;

		//����ʱ��
		float fade;

		void DoWrap(_In_ const Vec4f& force, _In_ float turbul, _In_ float resist);

		//ǰ��һ֡
		void Frame(_In_ float interval);

		//��Ⱦ
		void Render(_In_ ICamera* camera);

		Particle(){};

		Particle(_In_ const Vec4f& pos, _In_ const Vec4f& speed, _In_ const Vec4f& force, _In_ const float fade);
	};

	//��������
	std::list<Particle> plist;

	//�ɴ�
	Craft* craft;

	//�����
	ICamera* camera;

	//�ٶ�
	Vec4f speed;
	
	//��ʱ����
	Vec4f up, nVec;

	//��ǰʱ��
	float tim;

	//����ʱ��
	static const float TIME_THRES;

	//������ͼ
	unsigned int texture;

	//���һ������
	void AddParticle();

	ParticleSystem(_In_ Craft* craft, _In_ ICamera* camera);

public:
	~ParticleSystem();

	//ǰ��һ֡
	void Frame(_In_ float interval);

	//��Ⱦ
	void Render();

	//��ʼ��
	bool Initialize(_In_ char* filename);

	//���������
	void UpdateCamera(_In_ ICamera* newcamera);

	static ParticleSystem* create(_In_ Craft* craft, _In_ ICamera* camera);
};

#endif