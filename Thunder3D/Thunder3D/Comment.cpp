#include "Comment.h"

Comment::Comment(App* app):
	m_app(app)
{
}

Comment::~Comment()
{
	glDeleteLists(m_base, 96);					//ɾ��96����ʾ�б�
}

bool Comment::Initialize()
{
	HFONT	font;						// ������
	HFONT	oldfont;						// �ɵ�������

	m_base = glGenLists(96);
	font = CreateFont(-24,
		0,				// ������
		0,				// �������ת�Ƕ� Angle Of Escapement
		0,				// ������ߵ���ת�Ƕ�Orientation Angle
		FW_BOLD,				// ���������
		FALSE,				// �Ƿ�ʹ��б��
		FALSE,				// �Ƿ�ʹ���»���
		FALSE,				// �Ƿ�ʹ��ɾ����
		ANSI_CHARSET,			// �����ַ���
		OUT_TT_PRECIS,			// �������
		CLIP_DEFAULT_PRECIS,		// �ü�����
		ANTIALIASED_QUALITY,		// �������
		FF_DONTCARE | DEFAULT_PITCH,		// Family And Pitch
		"Courier New");			// ��������
	oldfont = (HFONT)SelectObject(m_app->GetHDC(), font);			// ѡ��������Ҫ������
	wglUseFontBitmaps(m_app->GetHDC(), 32, 96, m_base);			// ����96����ʾ�б����ƴ�ASCII��Ϊ32-128���ַ�
	SelectObject(m_app->GetHDC(), oldfont);					// ѡ��ԭ��������
	DeleteObject(font);					// ɾ������

	return true;
}

void Comment::Render()
{
	float tmp[4] = { 0.3f, 0.3f, 0.3f, 1.0f };
	glLightfv(GL_LIGHT5, GL_AMBIENT, tmp);
	glEnable(GL_LIGHT5);
	glDisable(GL_TEXTURE_2D);
	glPushMatrix();
	glLoadIdentity();							// ���õ�ǰ��ģ�͹۲����
	glTranslatef(0.0f, 0.0f, -1.0f);				// ������Ļһ����λ

	glPushAttrib(GL_LIST_BIT);					// ����ʾ�б�����ѹ�����Զ�ջ
	glListBase(m_base - 32);					// ������ʾ�б�Ļ���ֵ
	for (int i = 0; i < m_contexts.size(); i++) {
		glColor4f(m_colors[i].x, m_colors[i].y, m_colors[i].z, m_colors[i].w);
		// ���ù�դ��λ�ã��������λ��
		glRasterPos2f(m_pos[i].x, m_pos[i].y);
		glCallLists(m_contexts[i].size(), GL_UNSIGNED_BYTE, m_contexts[i].data());		// ������ʾ�б�����ַ���
	}
	//glRasterPos2f(pos.x, pos.y + 0.1);
	//glCallLists(strlen("test"), GL_UNSIGNED_BYTE, "test");		// ������ʾ�б�����ַ���
	glPopAttrib();						// �������Զ�ջ

	glPopMatrix();
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_LIGHT5);
}

void Comment::Set(
	std::vector<std::string> contexts,
	std::vector<Vec4f> colors,
	std::vector<Vec4f> pos,
	std::vector<float> scales)
{
	if (m_contexts.size() != m_colors.size() || m_colors.size() != m_pos.size())
		return;
	if (scales.size() != 0 && m_scales.size() != m_contexts.size())
		return;

	m_contexts = contexts;
	m_colors = colors;
	m_pos = pos;

	if (scales.size() == 0) {
		m_scales.clear();
		for (int i = 0; i < m_contexts.size(); i++) {
			m_scales.push_back(1.f);
		}
	}
	else {
		m_scales = scales;
	}
}