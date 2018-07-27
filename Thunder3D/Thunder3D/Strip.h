#pragma once

#include "object.h"
#include "camera.h"

class Strip : public Object
{
public:
	/**
	*  \brief  ���췽��
	*/
	static Strip* create(Camera* camera);

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

	void SetValue(float val);

	void SetPos(Vec4f p1, Vec4f p2);

	void SetColor(Vec4f color);

private:
	Strip(Camera* camera);

	Camera* m_camera;
	float m_value;
	Vec4f m_p1, m_p2;
	Vec4f m_color;

	static const float MAX_VALUE;
};

inline Strip* Strip::create(Camera* camera) {
	Strip* ret = new Strip(camera);
	ret->AddRef();
	return ret;
}