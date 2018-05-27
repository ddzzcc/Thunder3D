#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include "craft.h"
#include "mat4f.h"

const float Craft::_LOWEST_SPEED = 0.01f;
const float Craft::_HIGHEST_SPEED = 5.f;

const float EPS = 0.000001f;
const float PI = 3.141592654f;


Craft::Craft(const Vec4f& pos, const Vec4f& velocity, const Vec4f& up) :
_velocity(velocity),
_up(up)
{
	this->pos = pos;
}

bool Craft::Initialize(_In_ const char*  txtFile)
{
	return _model.Initialize(txtFile);
	//return _model.InitializeTxt("config/cone.txt", "config/blank.bmp");
}

void Craft::Frame(_In_ float interval)
{
	pos = pos + _velocity * interval;
}

void Craft::Render()
{
	glPushMatrix();

	glTranslatef(pos.x, pos.y, pos.z);

	/*��ʼģ�ͳ����Լ��Ϸ���*/
	Vec4f oriVec(0.f, 0.f, -1.f);
	Vec4f oriUpVec(0.f, 1.f, 0.f);
	Vec4f zaxis(0.f, 0.f, 1.f);

	/*�Ȱѳ���ת�ԣ��ٴ����Ϸ���*/
	float phi = acosf(_velocity.dot(oriVec) / _velocity.length());
	if (fabs(phi) > EPS){
		Vec4f nVec = - _velocity.cross(oriVec);
		glRotatef(phi * 180 / PI, nVec.x, nVec.y, nVec.z);

		/*������ת��Ҫ���Ϸ���Ҳ��ת��ȥ*/
		Mat4f mat = Mat4f::rotate(nVec, phi);
		Vec4f tmpUpVec2 = mat.transpose() * _up;
		phi = acosf(oriUpVec.dot(tmpUpVec2) / oriUpVec.length() / tmpUpVec2.length());
		if(fabs(phi) > EPS){
			//���ǵ������з��ţ�����foraxis������zaxisҲ������-zaxis
			Vec4f foraxis = tmpUpVec2.cross(oriUpVec);
			//Ȼ����phi�ӽ�PI��ʱ����з�������������ǿ��ת��zaxis��-zaxis
			foraxis.x = foraxis.y = 0.0f;
			foraxis.z /= fabs(foraxis.z);
			/*����Ӧ�����ٶȷ�������ת������֮ǰ��ת��һ�Σ�����Ҫ����ת��ȥ���ٶȷ�������ת*/
			glRotatef(-phi * 180 / PI, foraxis.x, foraxis.y, foraxis.z);
		}
	}
	else{
		phi = acos(oriUpVec.dot(_up) / oriUpVec.length() / _up.length());
		if (fabs(phi) > EPS){
			Vec4f foraxis = _up.cross(oriUpVec);
			foraxis.x = foraxis.y = 0.0f;
			foraxis.z /= fabs(foraxis.z);
			glRotatef(-phi * 180 / PI, zaxis.x, zaxis.y, zaxis.z);
		}
	}

	glScalef(0.05f, 0.05f, 0.05f);
	_model.Render();

	glPopMatrix();
}

Vec4f Craft::GetSpeed()
{
	return _velocity;
}

Vec4f Craft::GetUp()
{
	_up.normal();
	return _up;
}

void Craft::RotateLR(_In_ float ang)
{
	Mat4f mat = Mat4f::rotate(_up, -ang);
	_velocity = mat * _velocity;
}

void Craft::RotateUD(_In_ float ang)
{
	Vec4f nVec = _velocity.cross(_up);
	Mat4f mat = Mat4f::rotate(nVec, -ang);

	_velocity = mat * _velocity;
	_up = mat * _up;
}

void Craft::Accelerate(_In_ float delta_speed)
{
	float speed = _velocity.length();
	if (speed + delta_speed < _LOWEST_SPEED)
		speed = _LOWEST_SPEED;
	if (speed + delta_speed > _HIGHEST_SPEED)
		speed = _HIGHEST_SPEED;
	speed += delta_speed;

	_velocity.normal();
	_velocity = _velocity * speed;
}

bool Craft::Collide(_In_ const Vec4f& point)
{
	//ģ�ʹ�СԤ��ֵ
	const float MODEL_SIZE = 2.f;

	if (point.dist(pos) < MODEL_SIZE)
		return true;
	return false;
}