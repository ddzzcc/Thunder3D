#ifndef _MATERIALMODEL_H
#define _MATERIALMODEL_H

#include "common.h"

class MaterialModel{
private:
	//���ͷ
	struct Header{
		float amb[3];  //��������ɫ
		float diff[3]; //�������ɫ
		unsigned long cnt; //����
	};
	//��������
	struct Point{
		float x, y, z, nx, ny, nz;
	};
	//�������
	struct Asset{
		Point* st;
		Point* ed;
	};

	Header* heads;
	Point* pnts;
	Asset* asts;
	unsigned long cnt;

public:
	MaterialModel();

	//��ʼ��һ��ģ�ͣ��������ļ���
	//binFile: ģ���ļ���
	//����ֵ : �Ƿ�ɹ�
	bool Initialize(_In_ const char* binFile);

	//�������ģ��
	void Render();

	~MaterialModel();
};

#endif