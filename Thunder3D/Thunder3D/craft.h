#ifndef _CRAFT_H
#define _CRAFT_H

#include "object.h"
#include "materialModel.h"
#include "model.h"

class Craft : public Object
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
	static Craft* create(_In_ const Vec4f& pos, _In_ const Vec4f& velocity, _In_ const Vec4f& up);

	/**
	*  \brief	��ʼ����ģ������
	*
	*  \param  txtFile	ģ���ļ���
	*
	*  \return ��ʼ���Ƿ�ɹ�
	*/
	//ע����֤ģ�͵ĳ�����z������Ϊǰ��y������Ϊ��
	bool Initialize(_In_ const char*  txtFile);

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
	*  \brief  ȡ�÷ɴ����ٶ�����(w=0)
	*/
	Vec4f GetSpeed();

	/**
	*  \brief  ȡ�÷ɴ����Ϸ�������(w=0)
	*/
	Vec4f GetUp();

	void SetPos(Vec4f pos);


	/**
	*  \brief  ����Ļ������ת
	*
	*  \param  ang  �Ƕ�
	*/
	void RotateLR(_In_ float ang);

	/**
	*  \brief  ����Ļ������ת
	*
	*  \param  ang  �Ƕ�
	*/
	void RotateUD(_In_ float ang);

	/**
	*  \brief  ��ĳ������ת�ɴ�
	*
	*  \param  axis  ��ת��
	*
	*  \param  ang  �Ƕ�
	*/
	void Rotate(Vec4f axis, float ang);

	/**
	*  \brief  ǰ������ٶ�
	*
	*  \param  speed  ��Ҫ�Ӽ����ٶȣ����Ӹ���
	*/
	void Accelerate(_In_ float delta_speed);

	/**
	*  \brief  ��ײ���
	*
	*  \param  point Ŀ���
	*/
	//�ж�point�Ƿ��ڷɴ��ı���
	//�ð뾶ʲô�Ŀ�һ�¾��У���Ҫ��ܾ�ȷ
	bool Collide(_In_ const Vec4f& point);

protected:
	Craft(const Vec4f& pos, const Vec4f& velocity, const Vec4f& up);

	//����������ʵ�������ڲ���Ҫ�ķ����ͳ�Ա
	Vec4f _velocity;		/*�����ٶȣ�ʸ����*/
	Vec4f _up;				/*�Ϸ�������*/
	MaterialModel _model;			/*ģ���������*/

	static const float _LOWEST_SPEED;	/*�ɴ����е�����ٶȣ���ֹ���ٵ��µ�ͻȻ180��ת��*/
	static const float _HIGHEST_SPEED;	/*�ɴ����е�����ٶȣ�������ٶȶԳƣ�*/
};

inline Craft* Craft::create(_In_ const Vec4f& pos, _In_ const Vec4f& velocity, _In_ const Vec4f& up){
	Craft* ret = new Craft(pos, velocity, up);
	ret->AddRef();
	return ret;
}

#endif