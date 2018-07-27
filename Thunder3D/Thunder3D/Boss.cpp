#include "Boss.h"
#include "global.h"

#include <gl\GL.h>
#include <gl\GLU.h>
#include <random>
using namespace std;
//��ʱ��ʩ���ṩ���������Դ
static default_random_engine re(GetTickCount());

Boss::~Boss()
{
}


void Boss::Frame(_In_ float interval)
{
	for (auto iter : m_bullets) {
		iter->Frame(interval);
	}

	//��һ�׶�
	if (m_status == 0) {
		Status1(interval);
	}
	//�ڶ��׶�
	else if (m_status == 1) {
		Status2(interval);
	}
	//�����׶�
	else if (m_status == 2) {
		Status3(interval);
	}

	//�ӵ����������ж�
	for (auto iter = m_bullets.begin(); iter != m_bullets.end();) {
		if ((*iter)->GetLifeTime() < 0) {
			SafeRelease(&(*iter));
			iter = m_bullets.erase(iter);
			continue;
		}
		iter++;
	}
}

void Boss::Status1(float interval)
{
	//λ�û���Ư��
	auto pos = m_craft->GetPos();
	if ((pos + m_v * interval).length() > 5.f)
		RanV();
	else
		m_craft->SetPos(pos + m_v * interval);

	//ģʽһ
	if (m_subStatus == 0) {
		//ÿ��״̬��ʼ����cdһ��
		if (m_cdTime < 2.f) {
			m_cdTime += interval;
		}
		else {
			//׷�ٺ������׶ζ���ת����player
			if (m_flag[0][0] == 0 || m_flag[0][0] == 1) {
				const float ROT_SPEED = 0.6f;
				Vec4f moveTo = m_playerPos - GetPos();
				Vec4f v = m_craft->GetSpeed();
				float ang = moveTo.CalAng(v);
				if (fabs(ang) > 0.01f) {
					float rAng = interval * ROT_SPEED;
					Vec4f axis = moveTo.cross(v);
					//С����ֵ��ʼ������
					if (fabs(ang) < 0.2f) {
						m_flag[0][0] = 1;
					}
					if (fabs(ang) < rAng)
						rAng = fabs(ang);
					m_craft->Rotate(axis, -rAng);
				}
				m_craft->Rotate(v, interval * ROT_SPEED);
				m_lastPPos = m_playerPos;
			}
			//�����׶ηų�����
			if (m_flag[0][0] == 1) {
				DrawLockBeam();
				m_timer[0][0] += interval;
				if (m_timer[0][0] > 2.f) {
					m_flag[0][0] = 2;
				}
			}
			//ͣ1s(���߲���׷��)
			if (m_flag[0][0] == 2) {
				m_timer[0][1] += interval;
				if (m_timer[0][1] > 2.f) {
					m_flag[0][0] = 3;
					m_shootCD[0][0] = 0.2f;
					m_flag[0][1] = 0;
				}
			}
			//��������ӵ�
			if (m_flag[0][0] == 3) {
				if (m_shootCD[0][0] < 0.f) {
					m_shootCD[0][0] = 0.5f;
					auto v = m_craft->GetSpeed().normal();
					auto up = m_craft->GetUp().normal();
					auto right = up.cross(v);
					//������A
					if (m_flag[0][1] == 0) {
						Shoot(pos + up * 0.5f + right * 0.5f, v * 4, 4.f);
						Shoot(pos - up * 0.5f + right * 0.5f, v * 4, 4.f);
						Shoot(pos + up * 0.5f - right * 0.5f, v * 4, 4.f);
						Shoot(pos - up * 0.5f - right * 0.5f, v * 4, 4.f);
					}
					//������B
					else {
						Shoot(pos + up * 0.7f, v * 4, 4.f);
						Shoot(pos + right * 0.7f, v * 4, 4.f);
						Shoot(pos - right * 0.7f, v * 4, 4.f);
						Shoot(pos - up * 0.7f, v * 4, 4.f);
					}
					m_flag[0][1] = 1 - m_flag[0][1];
				}

				m_shootCD[0][0] -= interval;
				m_timer[0][2] += interval;
				if (m_timer[0][2] > 5.f) {
					memset(m_timer[0], 0, sizeof(m_timer[0]));
					memset(m_flag[0], 0, sizeof(m_flag[0]));
					m_shootCD[0][0] = 0.f;
					m_subStatus = 1;
					m_cdTime = 0.f;
					m_frameCount1_2 = 0;
				}
			}
		}
	}
	//ģʽ��
	else if (m_subStatus == 1) {
		//ÿ��״̬��ʼ����cdһ��
		if (m_cdTime < 2.f) {
			m_cdTime += interval;
		}
		else {
			if (m_flag[0][2] == 0) {
				const float ROT_SPEED = 0.6f;
				Vec4f moveTo = m_playerPos - GetPos();
				Vec4f v = m_craft->GetSpeed();
				float ang = moveTo.CalAng(v);
				if (fabs(ang) > 0.01f) {
					float rAng = interval * ROT_SPEED;
					Vec4f axis = moveTo.cross(v);
					//С����ֵ��ʼ������
					if (fabs(ang) < 0.2f) {
						m_flag[0][2] = 1;
					}
					if (fabs(ang) < rAng)
						rAng = fabs(ang);
					m_craft->Rotate(axis, -rAng);
				}
				m_craft->Rotate(v, interval * ROT_SPEED);
			}
			if (m_flag[0][2] == 1) {
				if (m_shootCD[0][1] < 0.f) {
					m_shootCD[0][1] = 0.2f;
					auto v = m_craft->GetSpeed().normal();
					auto up = m_craft->GetUp().normal();
					auto right = up.cross(v);
					//ʮ��ն��ÿ��������ɢһ��
					Shoot(pos + up * sqrt(m_frameCount1_2) * 0.7f, v * 4, 4.f);
					Shoot(pos + right * sqrt(m_frameCount1_2) * 0.7f, v * 4, 4.f);
					Shoot(pos - right * sqrt(m_frameCount1_2) * 0.7f, v * 4, 4.f);
					Shoot(pos - up * sqrt(m_frameCount1_2) * 0.7f, v * 4, 4.f);
					m_frameCount1_2 += 8;
				}
				m_shootCD[0][1] -= interval;
				m_timer[0][3] += interval;
				if (m_timer[0][3] > 2.f) {
					memset(m_timer[0], 0, sizeof(m_timer[0]));
					memset(m_flag[0], 0, sizeof(m_flag[0]));
					m_shootCD[0][0] = 0.f;
					m_subStatus = 0;
					m_cdTime = 0.f;
				}
			}
		}
	}

	//�ж�Ѫ����ת�׶�
	if (m_curHP <= m_MaxHP * 0.5f) {
		m_subStatus = 0;
		m_status = 1;
	}
}

void Boss::Status2(float interval)
{
	//˲��
	if (m_subStatus == 0) {
		//ÿ��״̬��ʼ����cdһ��
		if (m_cdTime < 2.f) {
			m_cdTime += interval;
		}
		else {
			if (m_flag[1][0] == 0) {

			}
		}
	}
	//ģʽһ
	else if (m_subStatus == 1) {
		//ÿ��״̬��ʼ����cdһ��
		if (m_cdTime < 2.f) {
			m_cdTime += interval;
		}
		else {

		}
	}
	//ģʽ��
	else if (m_subStatus == 2) {
		//ÿ��״̬��ʼ����cdһ��
		if (m_cdTime < 2.f) {
			m_cdTime += interval;
		}
		else {

		}
	}

	//�ж�Ѫ����ת�׶�
	if (m_curHP <= m_MaxHP * 0.1f) {
		m_subStatus = 0;
		m_status = 2;
	}
}

void Boss::Status3(float interval)
{
	//׼��ʱ��
	if (m_subStatus == 0) {

	}
	//׷��+���
	else if (m_subStatus == 1) {

	}
}

void Boss::Render()
{
	m_craft->Render();
	for (auto iter : m_bullets) {
		iter->Render();
	}
}

Boss::Boss(Craft* craft, int hp) :
	BaseAI(craft, hp),
	m_status(0),
	m_subStatus(0),
	m_cdTime(0.f),
	m_lastPPos(Vec4f())
{
	m_craft->SetPos(Vec4f(0.f, 0.f, 0.f));
	RanV();
	memset(m_flag, 0, sizeof(m_flag));
	memset(m_timer, 0, sizeof(m_timer));
}

void Boss::RanV()
{
	float x = uniform_real_distribution<float>(0.0f, 0.2f)(re);
	float y = uniform_real_distribution<float>(0.0f, 0.2f)(re);
	float z = uniform_real_distribution<float>(0.0f, 0.2f)(re);

	m_v = Vec4f(x, y, z);
}

void Boss::DrawLockBeam()
{
	Vec4f pos = GetPos();
	glPushMatrix();
	glTranslatef(0.f, 0.f, 0.f);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	glBegin(GL_LINES);

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(pos.x, pos.y, pos.z);
	glVertex3f(m_lastPPos.x, m_lastPPos.y, m_lastPPos.z);
	glEnd();
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glPopMatrix();
}

void Boss::Shoot(Vec4f sPos, Vec4f v, float lifetime)
{
	Bullet* bullet = Bullet::create(sPos, v, lifetime, 0.02f);
	bullet->Initialize("config/sphere.bin", "config/blue.bmp");
	m_bullets.push_back(bullet);
}

int Boss::Collide(_In_ const Vec4f& point, float radius, bool& isDead)
{
	int ret = 0;

	isDead = false;
	if (point.dist(m_craft->GetPos()) < T3D::MODEL_SIZE + radius) {
		ret += 100;
		isDead = true;
	}

	for (auto iter2 = m_bullets.begin(); iter2 != m_bullets.end();) {
		if (point.dist((*iter2)->GetPos()) < radius) {
			SafeRelease(&(*iter2));
			iter2 = m_bullets.erase(iter2);
			ret++;
			continue;
		}
		iter2++;
	}

	return ret;
}

bool Boss::Collide(_In_ const Vec4f& point)
{
	//ģ�ʹ�СԤ��ֵ

	return point.dist(m_craft->GetPos()) < T3D::MODEL_SIZE;
}