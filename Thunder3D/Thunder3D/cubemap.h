#pragma once

#include "object.h"

class CubeMap : public Object
{
public:
	/**
	*  \brief  �������󷽷�
	*
	*  \param  size ��պгߴ� 
	*/
	static CubeMap* create(float size);

	/**
	*  \brief  ��������
	*/
	~CubeMap();

	/**
	*  \brief  ��Ⱦ
	*/
	void Render();

	/**
	*  \brief	��ʼ������������
	*
	*  \param  bmpFile �����ļ���
	*
	*  \return ��ʼ���Ƿ�ɹ�
	*/
	bool Initialize(_In_ const char*  bmpFile);

private:
	CubeMap(float size);

	float _size;		/*��պгߴ磨������߳���һ�룩*/
	unsigned int _texture;	/*����*/
};

inline CubeMap* CubeMap::create(float size){
	CubeMap* ret = new CubeMap(size);
	ret->AddRef();
	return ret;
}