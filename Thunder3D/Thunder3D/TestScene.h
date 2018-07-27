#pragma once

#include "basescene.h"

#include "planet.h"
#include "star.h"
#include "craft.h"
#include "LightCone.h"
#include "ParticleSystem.h"
#include "SurroundBox.h"
#include "cubemap.h"
#include "controller.h"

class TestScene : public BaseScene
{
public:
	TestScene();

	~TestScene();

	bool Init(App* app) override;

	T3D::SCENE Update(float interval) override;

	void Reset() override;

	//������׼�������������
	//ʹ�������ӽ�
	//��Щ��������Ʋ������������
	void UseStandardView();

	//�������ٿ������������
	void UseTraceView(_In_ IObject* obj);

	//�����ɴ��������������
	void UseCraftView();

	//ѡ������¼�
	//point : ѡ���
	void OnSelect(_In_ const Vec4f& point);

	//׷�ٶ����¼�
	//point : ѡ���
	void OnTrace(_In_ const Vec4f& point);

private:
	Star* sun;
	Planet* earth;
	Planet* moon;
	Craft* craft;
	ParticleSystem* partisys;
	LightCone* lightcone;
	SurroundBox* surbox;
	CubeMap* cubemap;

	App* m_app;
};