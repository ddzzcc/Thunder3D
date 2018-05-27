#ifndef _MAT4F_H
#define _MAT4F_H

#include "vec4f.h"

/**
* ͨ��SSE������
* ��������˳��洢
*/
struct Mat4f{
	union{
		float m[16];
		struct{
			Vec4f c[4];
		};
	};

	//��ʼ��һ���������
	Mat4f(){};

	//����Ԫ��ʽ��16��ֵ��ʼ������
	//mAB => B��A��
	Mat4f(
		_In_ float m11, _In_ float m21, _In_ float m31, _In_ float m41,
		_In_ float m12, _In_ float m22, _In_ float m32, _In_ float m42,
		_In_ float m13, _In_ float m23, _In_ float m33, _In_ float m43,
		_In_ float m14, _In_ float m24, _In_ float m34, _In_ float m44
		);

	Mat4f(const Mat4f&);

	//���쵥λ����
	static Mat4f eye();

	//����ȫ0����
	static Mat4f zeros();

	//����ȫ1����
	static Mat4f ones();

	//����ӷ�
	Mat4f operator+(_In_ const Mat4f& rhs) const;

	//�������
	Mat4f operator-(_In_ const Mat4f& rhs) const;

	//����˷�
	Mat4f operator*(_In_ const Mat4f& rhs) const;

	//���������
	Vec4f operator*(_In_ const Vec4f& rhs) const;

	//�����Ӧ�����
	Mat4f mul(_In_ const Mat4f& rhs) const;

	//ƽ�ƾ���
	//pos: ƽ������
	static Mat4f translate(_In_ const Vec4f& pos);

	//�۲����
	//eye: ��������ڵĵ�
	//at : ��������Եĵ㣬���ص�λ��������Ե�
	//up : ��������Ϸ�����������������λ������Ϸ�����
	static Mat4f lookat(_In_ const Vec4f& eye, _Inout_ Vec4f& at, _Inout_ Vec4f& up);

	//��ת����
	//axis: ��ת�ᣬ���ص�λ�������ת��
	//ang : ��ʱ����ת��
	static Mat4f rotate(_Inout_ Vec4f& axis, _In_ const float ang);

	//����ת��
	Mat4f transpose() const;

	//����ȡ��
	//row: ������(0..3)
	//col: ������(0..3)
	float& at(const int row, const int col);
	const float& at(const int row, const int col) const;

	//�ֽ�һ���۲���󣬷�����������ڵ�λ��
	//���������ת����ֱ����QuatFromMatrix
	Vec4f FindEyePos() const;

private:
	//�����������������£�SSE�㷨��Ч�ʵ���ֱ������
	static Mat4f rotate_sse(_Inout_ Vec4f& axis, _In_ const float ang);
};

inline Mat4f::Mat4f(
	_In_ float m11, _In_ float m21, _In_ float m31, _In_ float m41,
	_In_ float m12, _In_ float m22, _In_ float m32, _In_ float m42,
	_In_ float m13, _In_ float m23, _In_ float m33, _In_ float m43,
	_In_ float m14, _In_ float m24, _In_ float m34, _In_ float m44
	)
{
	m[0] = m11;
	m[1] = m12;
	m[2] = m13;
	m[3] = m14;
	m[4] = m21;
	m[5] = m22;
	m[6] = m23;
	m[7] = m24;
	m[8] = m31;
	m[9] = m32;
	m[10] = m33;
	m[11] = m34;
	m[12] = m41;
	m[13] = m42;
	m[14] = m43;
	m[15] = m44;
}

inline Mat4f::Mat4f(const Mat4f& mat)
{
	memcpy_s(m, sizeof(m), mat.m, sizeof(mat.m));
}

inline Mat4f Mat4f::eye(){
	return Mat4f(
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
		);
}

inline Mat4f Mat4f::zeros(){
	return Mat4f(
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
		);
}

inline Mat4f Mat4f::ones(){
	return Mat4f(
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1,
		1, 1, 1, 1
		);
}

inline Mat4f Mat4f::operator+(_In_ const Mat4f& rhs) const{
	Mat4f ret;
	for(int i = 0; i < 16; i += 4){
		ret.m[i + 0] = m[i + 0] + rhs.m[i + 0];
		ret.m[i + 1] = m[i + 1] + rhs.m[i + 1];
		ret.m[i + 2] = m[i + 2] + rhs.m[i + 2];
		ret.m[i + 3] = m[i + 3] + rhs.m[i + 3];
	}
	return ret;
}

inline Mat4f Mat4f::operator-(_In_ const Mat4f& rhs) const{
	Mat4f ret;
	for(int i = 0; i < 16; i += 4){
		ret.m[i + 0] = m[i + 0] - rhs.m[i + 0];
		ret.m[i + 1] = m[i + 1] - rhs.m[i + 1];
		ret.m[i + 2] = m[i + 2] - rhs.m[i + 2];
		ret.m[i + 3] = m[i + 3] - rhs.m[i + 3];
	}
	return ret;
}

inline Vec4f Mat4f::operator*(_In_ const Vec4f& rhs) const{
	return ((c[0] * rhs.v[0]) + (c[1] * rhs.v[1]))
		+ ((c[2] * rhs.v[2]) + (c[3] * rhs.v[3]));
}

inline Mat4f Mat4f::operator*(_In_ const Mat4f& rhs) const{
	Mat4f ret;
	for(int i = 0; i < 4; i ++){
		ret.c[i] = *this * rhs.c[i];
	}
	return ret;
}

inline Mat4f Mat4f::mul(_In_ const Mat4f& rhs) const{
	Mat4f ret;
	for(int i = 0; i < 16; i += 4){
		ret.m[i + 0] = m[i + 0] * rhs.m[i + 0];
		ret.m[i + 1] = m[i + 1] * rhs.m[i + 1];
		ret.m[i + 2] = m[i + 2] * rhs.m[i + 2];
		ret.m[i + 3] = m[i + 3] * rhs.m[i + 3];
	}
	return ret;
}

inline Mat4f Mat4f::transpose() const{
	return Mat4f(
		m[0], m[1], m[2], m[3],
		m[4], m[5], m[6], m[7],
		m[8], m[9], m[10], m[11],
		m[12], m[13], m[14], m[15]
		);
}

inline float& Mat4f::at(const int row, const int col){
	return m[col * 4 + row];
}

inline const float& Mat4f::at(const int row, const int col) const{
	return m[col * 4 + row];
}

#endif