#ifndef _CAMERA_H
#define _CAMERA_H

#include "common.h"
#include "object.h"
#include "craft.h"

interface ICamera : public IObject
{
public:
	static const GUID IID_ICamera;

	/**
	* ���������������OpenGL
	*/
	virtual void LoadMatrix() PURE;

	/**
	* ����Billboard���������������ת
	*/
	virtual void MakeBillBoard() PURE;
};

class Camera : public ICamera
{
protected:
	//���ü���
	ULONG refcnt;

	//����λ��
	Vec4f pos;

	Camera(_In_ const Vec4f& eye);

	virtual ~Camera(){};
public:
	ULONG STDMETHODCALLTYPE AddRef();

	ULONG STDMETHODCALLTYPE Release();

	HRESULT STDMETHODCALLTYPE QueryInterface(_In_ const IID&id, _Outptr_ void**ppv);

	Vec4f GetPos();

	void Frame(_In_ float interval);

	void Render();

	void* operator[](_In_opt_ const std::string& name);
};

//////////////////////////////////////////////////////////////////////////

//�����ƶ������
class FreeCamera : public Camera
{
protected:
	//ʱ�䳣��
	static const float SLERP_TIME;

	//����ע�ӵ�
	Vec4f at;

	//�Ϸ���ʸ��
	Vec4f up;

	//��ͨ�۲����
	Mat4f lookMat;

	//������˶�����
	Mat4f motionMat;

	//�˶�������ʷ��¼
	Mat4f motion0, motion1;

	//�Ƿ��ڲ�ֵ״̬
	bool inSlerp;

	//��ֵʱ��
	float tim;

	FreeCamera(_In_ const Vec4f& eye, _In_ const Vec4f& at, _In_ const Vec4f& up);

public:
	/**
	* ���������Ļ�����ƶ�
	*/
	void MoveOnScreen(_In_ const Vec4f& trans);

	/**
	* ˮƽת������ͷ����Ļ����ϵ��
	*/
	void RotateLR(_In_ float ang);

	/**
	* ��ֱת������ͷ����Ļ����ϵ��
	*/
	void RotateUD(_In_ float ang);

	/**
	* ����һ����ʷλ��
	*/
	void RecordPos();

	/**
	* ���в�ֵ�ƶ�
	*/
	void StartSlerp();

	void Frame(_In_ float interval);

	void LoadMatrix();

	void MakeBillBoard();

	static FreeCamera* create(_In_ const Vec4f& eye, _In_ const Vec4f& at, _In_ const Vec4f& up);

	/**
	* �Ƿ��ڲ�ֵ�ƶ���
	*/
	bool InSlerp() const;
};

inline bool FreeCamera::InSlerp() const{
	return inSlerp;
}

//////////////////////////////////////////////////////////////////////////

//׷�ٶ��������
class TraceCamera : public Camera{
protected:
	//��׷�ٵĶ���
	IObject* obj;

	//�۲����
	Mat4f lookMat;

	TraceCamera(_In_ IObject* obj);

	~TraceCamera();

public:
	void Frame(_In_ float interval);

	void LoadMatrix();

	void MakeBillBoard();

	static TraceCamera* create(_In_ IObject* obj);
};

//////////////////////////////////////////////////////////////////////////

//���ٷɴ������
class CraftCamera : public Camera{
protected:
	//��׷�ٵķɴ�
	Craft* craft;

	//�۲����
	Mat4f lookMat;

	CraftCamera(_In_ Craft* craft);

	~CraftCamera();

public:
	void Frame(_In_ float interval);

	void LoadMatrix();

	void MakeBillBoard();

	static CraftCamera* create(_In_ Craft* craft);
};

#endif
