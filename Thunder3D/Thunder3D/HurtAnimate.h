#pragma once

#include "object.h"
#include "camera.h"

class HurtAnimate : public Object
{
public:
	/**
	*  \brief  ���췽��
	*/
	static HurtAnimate* create(Camera* camera);

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

	void NewHurt();

private:
	HurtAnimate(Camera* camera);

	Camera* m_camera;
	float m_curAniTime;
	const float ANI_TIME;
};

inline HurtAnimate* HurtAnimate::create(Camera* camera) {
	HurtAnimate* ret = new HurtAnimate(camera);
	ret->AddRef();
	return ret;
}