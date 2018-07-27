#pragma once

#include "object.h"
#include "model.h"

class Bullet : public Object
{
public:
	/**
	*  \brief  ���췽��
	*
	*  \param  pos λ�ã��������꣩
	*
	*  \param  velocity �ٶȣ�ʸ����
	*
	*  \param  up �Ϸ�������
	*/
	static Bullet* create(
		_In_ const Vec4f& pos, 
		_In_ const Vec4f& velocity, 
		_In_ const float lifeTime,
		_In_ const float size);

	/**
	*  \brief	��ʼ����ģ������
	*
	*  \param  txtFile	ģ���ļ���
	*
	*  \return ��ʼ���Ƿ�ɹ�
	*/
	//ע����֤ģ�͵ĳ�����z������Ϊǰ��y������Ϊ��
	bool Initialize(const char*  txtFile, const char* bmpFile);

	/**
	*  \brief	ΪͶ��������˶�ʱ����ת
	*
	*  \param  axis	��ת��
	*
	*  \param speed ��ת�ٶ�
	*/
	void AddRotation(Vec4f axis, float rotSpeed);

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
	*  \brief  ��ȡʣ����������
	*
	*  \return ��������
	*/
	float GetLifeTime();

private:
	Bullet(Vec4f pos, Vec4f velocity, float lifeTime, float size);

	Vec4f m_v;
	Model m_model;			/*ģ���������*/
	float m_lifeTime;
	Vec4f m_rotAxis;
	float m_rotSpeed;
	float m_rotAng;
	float m_size;
};

inline Bullet* Bullet::create(
	_In_ const Vec4f& pos, 
	_In_ const Vec4f& velocity, 
	_In_ const float lifeTime,
	_In_ const float size) {
	Bullet* ret = new Bullet(pos, velocity, lifeTime, size);
	ret->AddRef();
	return ret;
}