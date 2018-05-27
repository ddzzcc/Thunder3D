#ifndef _OBJECT_H
#define _OBJECT_H

#include "common.h"
#include "mat4f.h"
#include <string>
#include <Unknwn.h>

/**
* һ�����ӿ���
*/
interface IObject : public IUnknown
{
public:
	static const GUID IID_IObject;

	/**
	* �����������λ��
	*/
	virtual Vec4f GetPos() PURE;

	/**
	* ǰ��һ֡
	* interval : ǰ����ʱ�䣨�룩
	*/
	virtual void Frame(_In_ float interval) PURE;

	/**
	* ��Ⱦ����
	*/
	virtual void Render() PURE;

	/**
	* �����ֻ��ĳ����������
	* �����ƻ�������ķ�װ
	* name : ���Ե�����
	*/
	virtual void* operator[](_In_opt_ const std::string& name) PURE;
};

class Object : public IObject
{
protected:
	//���ü���
	ULONG refcnt;

	//����λ��
	Vec4f pos;

public:
	Object();

	virtual ~Object();

	ULONG STDMETHODCALLTYPE AddRef() ;

	ULONG STDMETHODCALLTYPE Release() ;

	HRESULT STDMETHODCALLTYPE QueryInterface(_In_ const IID&id, _Outptr_ void**ppv) ;

	Vec4f GetPos() ;

	void Frame(_In_ float interval) ;

	void Render() ;

	void* operator[](_In_opt_ const std::string& name);
};

#endif
