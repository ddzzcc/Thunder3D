#ifndef _MODEL_H
#define _MODEL_H

#include "common.h"

class Model{
private:
	struct NODE{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	//����
	NODE* node;

	//������
	int count;

	//����
	unsigned int texture;

	//���ļ��������ı�ģ��
	//filename: �ļ���
	//����ֵ  : �Ƿ�ɹ�
	bool LoadTxtModel(_In_ const char* filename);

	//�Ӷ������ļ�������ģ��
	//filename: �ļ���
	//����ֵ  : �Ƿ�ɹ�
	bool LoadBinModel(_In_ const char* filename);

	//���浽�������ļ�
	//filename: �ļ���
	//����ֵ  : �Ƿ�ɹ�
	bool SaveBinModel(_In_ const char* filename);

	//���ļ�������BMP����
	//filename: �ļ���
	//����ֵ  : �Ƿ�ɹ�
	bool LoadGLTexture_Bak(_In_ const char* filename);

	//���ļ�����������ͼƬ����
	//filename: �ļ���
	//����ֵ  : �Ƿ�ɹ�
	bool LoadGLTexture(_In_ const char* filename);

public:
	Model();

	//ͨ�÷�������������ͼƬ
	//filename: �ļ���
	//texture : ���ʺ�
	//����ֵ  : �Ƿ�ɹ�
	_Success_(return) static bool LoadGLTexture(_In_ const char* filename, _Out_ unsigned int *texture);

	//��ʼ��һ��ģ�ͣ��ı��ļ���
	//txtFile: ģ���ļ���
	//bmpFile: �����ļ���
	//����ֵ : �Ƿ�ɹ�
	bool InitializeTxt(_In_ const char* txtFile, _In_ const char* bmpFile);

	//��ʼ��һ��ģ�ͣ��������ļ���
	//binFile: ģ���ļ���
	//bmpFile: �����ļ���
	//����ֵ : �Ƿ�ɹ�
	bool Initialize(_In_ const char* binFile, _In_ const char* bmpFile);

	//�������ģ��
	void Render();

	~Model();
};

#endif