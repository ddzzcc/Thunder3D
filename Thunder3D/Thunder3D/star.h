#pragma once

#include "object.h"
#include "model.h"
#include "camera.h"

class Star : public Object
{
public:
	/**
	*  \brief  ���췽��
	*
	*  \param  pos λ�ã��������꣩
	*
	*  \param  speed ��ת�ٶ�
	*
	*  \param  radius �뾶
	*/
	static Star* create(Vec4f pos, float speed, float radius);

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
	Star(Vec4f pos, float speed, float radius);

	~Star();

	float _rotation;		/*��ǰ��ת�Ƕȣ���ת��*/
	float _rotSpeed;		/*��ת�ٶȣ���ת��*/

	float _radius; // �뾶

	Model _model;			/*ģ���������*/
};


inline Star* Star::create(Vec4f pos, float speed, float radius){
	Star* ret = new Star(pos, speed, radius);
	ret->AddRef();
	return ret;
}