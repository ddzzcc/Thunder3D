#pragma once

#include "object.h"
#include "app.h"

#include <gl\gl.h>		// �������µ�gl.h,glu.h��
#include <gl\glu.h>		// ����OpenGLʵ�ÿ�

#include <string>
#include <vector>

class Comment : public Object
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
	static Comment* create(App* app);

	~Comment();

	/**
	*  \brief	��ʼ��
	*
	*  \return ��ʼ���Ƿ�ɹ�
	*/
	bool Initialize();

	/**
	*  \brief  ��Ⱦ
	*/
	void Render();

	void Set(
		std::vector<std::string> contexts,
		std::vector<Vec4f> colors,
		std::vector<Vec4f> pos,
		std::vector<float> scales = std::vector<float>());

private:
	Comment(App* app);

	GLuint	m_base;			// �����������ʾ�б�Ŀ�ʼλ��
	App* m_app;
	std::vector<std::string> m_contexts;
	std::vector<Vec4f> m_colors;
	std::vector<Vec4f> m_pos;
	std::vector<float> m_scales;
};


inline Comment* Comment::create(App* app) {
	Comment* ret = new Comment(app);
	ret->AddRef();

	return ret;
}