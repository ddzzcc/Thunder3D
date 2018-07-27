#pragma once

#include "vec4f.h"
#include "camera.h"

struct Particle {
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

	Particle() {};

	Particle(_In_ const Vec4f& pos, _In_ const Vec4f& speed, _In_ const Vec4f& force, _In_ const float fade);
};