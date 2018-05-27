#pragma once

#include "object.h"
#include "model.h"
#include "Trace.h"

class Planet : public Object
{
public:
	/**
	*  \brief	���캯��
	*
	*  \param  radius ����뾶
	*
	*  \param  nVec	��ת���������
	*
	*  \param  speedR1  ��ת���ٶ�
	*
	*  \param  speedR2  ��ת���ٶ�
	*
	*  \param  parent ���ڵ㣨Բ���˶����ģ�
	*
	*  \param  size ����뾶
	*/
	static Planet* create(float radius, const Vec4f& nVec, float speedR1, float speedR2, IObject* parent, float size);

	/**
	*  \brief	��ʼ����ģ������
	*
	*  \param  txtFile	ģ���ļ���
	*
	*  \param  bmpFile	bmp��ʽ�����ļ���
	*
	*  \return ��ʼ���Ƿ�ɹ�
	*/
	bool Initialize(const char*  txtFile, const char* bmpFile);

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

	/**
	*  \brief  ��ײ���
	*
	*  \param  point Ŀ���
	*/
	bool Collide(_In_ const Vec4f& point);

	/**
	*  \brief  �������
	*  @override
	*/
	void* operator[](_In_opt_ const std::string& name);

private:
	void UpdatePos();
	
	float _radius;			/*��ת����뾶*/
	Vec4f _nVec;			/*��ת������*/	
	float _rotation;		/*��ǰ��ת�Ƕȣ���ת��*/
	float _revolution;		/*��ǰ��ת�Ƕȣ���ת��*/
	float _rotSpeed;		/*��ת�ٶ�*/
	float _revSpeed;		/*��ת�ٶ�*/
	
	IObject *_parent;		/*���ڵ㣨Բ���˶����ģ�*/

	Model _model;			/*ģ���������*/
	Trace* _trace;			/*�켣����*/

	float _size; //����뾶

	Planet(float radius, const Vec4f& nVec, float speedR1, float speedR2, IObject* parent, float size);

	~Planet();
};

inline Planet* Planet::create(float radius, const Vec4f& nVec, float speedR1, float speedR2, IObject* parent, float size){
	Planet* ret = new Planet(radius, nVec, speedR1, speedR2, parent, size);
	ret->AddRef();
	return ret;
}
