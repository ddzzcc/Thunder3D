#pragma once

#include "object.h"
#include "camera.h"

class FrontSight : public Object
{
public:
	/**
	*  \brief  ���췽��
	*/
	static FrontSight* create(FreeCamera* camera);

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

private:
	FrontSight(FreeCamera* camera);

	FreeCamera* m_camera;
};

inline FrontSight* FrontSight::create(FreeCamera* camera) {
	FrontSight* ret = new FrontSight(camera);
	ret->AddRef();
	return ret;
}