#ifndef _VEC4F_H
#define _VEC4F_H

#include "common.h"
#include <cmath>

struct Mat4f;

/**
* ͨ��SSE�㡢��������Ԫ����
* ����3D�ռ��еĵ㣬Ҫ��w=1.0
* �����ض���������������Ҫ��w=0.0
*/
struct Vec4f{
	union{
		float v[4];
		struct{
			float x;
			float y;
			float z;
			float w;
		};
	};

	//��ʼ��һ��������
	Vec4f();

	//��ʼ��һ���ض��������
	Vec4f(_In_ float x, _In_ float y, _In_ float z, _In_ float w = 0.0f);

	//�����ĸ���
	Vec4f(_In_ const Vec4f& rhs);

	//�������ڻ�
	float dot(_In_ const Vec4f &rhs) const;

	//�����ӷ�
	Vec4f operator+(_In_ const Vec4f &rhs) const;

	//��������ļ���
	Vec4f operator-(_In_ const Vec4f &rhs) const;

	//�����ķ���
	Vec4f operator-() const;

	//����������
	Vec4f operator*(_In_ const float scalar) const;

	//������������
	Vec4f operator/(_In_ const float scalar) const;

	//��λ����ǰ����
	Vec4f normal();

	//�����Ĳ��
	Vec4f cross(_In_ const Vec4f &rhs) const;

	//������������֮�������
	float CalAng(_In_ const Vec4f &rhs);

	//�����ĳ���
	float length() const;

	//����ľ���
	float dist(_In_ const Vec4f& rhs) const;

	//������������
	Mat4f tensormul(_In_ const Vec4f& rhs) const;

	//��ת����Ԫ��
	//axis: ��ת�ᣬ���ص�λ�������ת��
	//ang : ��ʱ����ת��
	static Vec4f rotate(_Inout_ Vec4f& axis, _In_ const float ang);

	//ƽ��������ֵ
	//v0: �������
	//v1: �յ�����
	//t : ʱ��ֵ
	static Vec4f lerp(_In_ const Vec4f& v0, _In_ const Vec4f& v1, _In_ const float t);

	//��Ԫ����ֵ
	//v0: �����Ԫ��
	//v1: �յ���Ԫ��
	//t : ʱ��ֵ
	static Vec4f slerp(_In_ const Vec4f& q0, _In_ const Vec4f& q1, _In_ const float t);

	//��Ԫ��ת��ת����
	Mat4f QuatToMat() const;

	//��ת����ת��Ԫ��
	static Vec4f QuatFromMat(_In_ const Mat4f& mat);
};

inline Vec4f::Vec4f(){
	v[0] = v[1] = v[2] = v[3] = 0.0f;
}

inline Vec4f::Vec4f(_In_ float x, _In_ float y, _In_ float z, _In_ float w){
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = w;
}

inline Vec4f::Vec4f(_In_ const Vec4f& rhs){
	v[0] = rhs.v[0];
	v[1] = rhs.v[1];
	v[2] = rhs.v[2];
	v[3] = rhs.v[3];
}

inline float Vec4f::dot(_In_ const Vec4f &rhs) const{
	return v[0] * rhs.v[0] + v[1] * rhs.v[1] + v[2] * rhs.v[2] + v[3] * rhs.v[3];
}

inline Vec4f Vec4f::operator+(_In_ const Vec4f &rhs) const{
	return Vec4f(v[0] + rhs.v[0], v[1] + rhs.v[1], v[2] + rhs.v[2], v[3] + rhs.v[3]);
}

inline Vec4f Vec4f::operator-(_In_ const Vec4f &rhs) const{
	return Vec4f(v[0] - rhs.v[0], v[1] - rhs.v[1], v[2] - rhs.v[2], v[3] - rhs.v[3]);
}

inline Vec4f Vec4f::operator-() const{
	return Vec4f(-v[0], -v[1], -v[2], -v[3]);
}

inline Vec4f Vec4f::operator*(_In_ const float scalar) const{
	return Vec4f(v[0] * scalar, v[1] * scalar, v[2] * scalar, v[3] * scalar);
}

inline Vec4f Vec4f::operator/(_In_ const float scalar) const{
	return *this * (1.0f / scalar);
}

inline Vec4f Vec4f::normal(){
	*this = *this / this->length();
	return *this;
}

inline Vec4f Vec4f::cross(_In_ const Vec4f &rhs) const{
	return Vec4f(
		v[1] * rhs.v[2] - v[2] * rhs.v[1],
		v[2] * rhs.v[0] - v[0] * rhs.v[2],
		v[0] * rhs.v[1] - v[1] * rhs.v[0],
		0.0f);
}

inline float Vec4f::CalAng(_In_ const Vec4f &rhs)
{
	return acos(this->dot(rhs) / this->length() / rhs.length());
}

inline float Vec4f::length() const{
	return sqrt(this->dot(*this));
}

inline float Vec4f::dist(_In_ const Vec4f& rhs) const{
	return (*this - rhs).length();
}

#endif