#ifndef _SURROUND_BOX
#define _SURROUND_BOX

#include "object.h"
#include "camera.h"

class SurroundBox : public Object{
private:
	IObject* obj;
	ICamera* camera;

	SurroundBox(_In_opt_ IObject* obj, _In_ ICamera* camera);

public:
	~SurroundBox();

	// �ж�Ŀ������Ƿ��ǵ�ǰѡ�еĶ���
	// target : Ŀ�����
	// return : �Ƿ�ѡ��
	bool SameObject(_In_opt_ IObject* target);

	// ����ѡ�еĶ���
	// target : Ŀ�����
	void UpdateObject(_In_opt_ IObject* target);

	//���������
	// newcamera : �������
	void UpdateCamera(_In_opt_ ICamera* newcamera);

	// ���л���
	void Render();

	// ������Χ�ж���
	static SurroundBox* create(_In_opt_ IObject* obj, _In_ ICamera* camera);
};

inline bool SurroundBox::SameObject(_In_opt_ IObject* target){
	return Equal(obj, target);
}

inline SurroundBox* SurroundBox::create(_In_opt_ IObject* obj, _In_ ICamera* camera){
	SurroundBox* ret = new SurroundBox(obj, camera);
	ret->AddRef();
	return ret;
}

#endif