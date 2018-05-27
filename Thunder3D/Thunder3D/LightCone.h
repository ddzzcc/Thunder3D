#ifndef _LIGHTCONE_H
#define _LIGHTCONE_H

#include "object.h"
#include "model.h"
#include "craft.h"

class LightCone : public Object{
private:
	//�ɴ�
	Craft* craft;

	//ģ��
	Model model;

	LightCone(_In_ Craft* craft);

public:
	~LightCone();

	//��ʼ��
	void Initialize(_In_ char* modelFile, _In_ char* texFile);

	//ǰ��һ֡
	void Frame(_In_ float interval);

	//����
	void Render();

	static LightCone* create(_In_ Craft* craft);
};

#endif