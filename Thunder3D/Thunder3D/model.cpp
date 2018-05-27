#include "model.h"
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>
#include <Wincodec.h>
using namespace std;

Model::Model() :
node(NULL),
count(0),
texture(0)
{
}

Model::~Model(){
	if(node){
		delete[] node;
	}
	if(texture != 0){
		glDeleteTextures(1, &texture);
	}
}

bool Model::LoadTxtModel(_In_ const char* filename){
	ifstream fin;
	char input;
	int i;

	if(node){
		return false;
	}

	//���ļ�����ȡ�ڵ���
	fin.open(filename);
	if(fin.fail()){
		return false;
	}

	fin.get(input);
	while(input != ':'){
		fin.get(input);
	}

	fin >> count;
	node = new NODE[count];
	if(!node){
		fin.close();
		return false;
	}

	//����������ͷ
	fin.get(input);
	while(input != ':'){
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	//���붥������
	for(i = 0; i < count; i++)
	{
		fin >> node[i].x >> node[i].y >> node[i].z;
		fin >> node[i].tu >> node[i].tv;
		fin >> node[i].nx >> node[i].ny >> node[i].nz;
	}

	//�ر�ģ���ļ�
	fin.close();

	return true;
}

bool Model::LoadBinModel(_In_ const char* filename){
	HANDLE hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile == INVALID_HANDLE_VALUE){
		return false;
	}

	DWORD fileSize = GetFileSize(hFile, NULL);
	count = fileSize / sizeof(NODE);
	node = new NODE[count];
	if(!node){
		CloseHandle(hFile);
		return false;
	}

	DWORD dwSize;
	ReadFile(hFile, node, fileSize, &dwSize, NULL);
	if(dwSize != fileSize){
		CloseHandle(hFile);
		return false;
	}

	CloseHandle(hFile);
	return true;
}

bool Model::LoadGLTexture_Bak(_In_ const char* filename)
{
	HANDLE hFile;
	DWORD dwBytes;
	BITMAPFILEHEADER fhead;
	BITMAPINFOHEADER ihead;
	GLubyte *buffer;

	hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile == INVALID_HANDLE_VALUE)
		return false;

	ReadFile(hFile, &fhead, sizeof(fhead), &dwBytes, NULL);
	if(dwBytes < sizeof(fhead) || fhead.bfType != 0x4D42)  //ASCII�ַ�'BM'��ͷ��ʾλͼ
	{
		CloseHandle(hFile);
		return false;
	}

	ReadFile(hFile, &ihead, sizeof(ihead), &dwBytes, NULL);
	if(dwBytes < sizeof(ihead) || /*ihead.biSize != sizeof(ihead) ||*/ (ihead.biBitCount != 24 && ihead.biBitCount != 32)){
		//�ļ��𻵻��߲���֧�ֵ�Windows��չλͼ
		CloseHandle(hFile);
		return false;
	}

	SetFilePointer(hFile, fhead.bfOffBits, NULL, FILE_BEGIN);
	buffer = (GLubyte*)VirtualAlloc(NULL, ihead.biSizeImage, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
	if(buffer == NULL){
		CloseHandle(hFile);
		return false;
	}
	ReadFile(hFile, buffer, ihead.biSizeImage, &dwBytes, NULL);
	if(dwBytes < ihead.biSizeImage){
		VirtualFree(buffer, 0, MEM_RELEASE);
		CloseHandle(hFile);
		return false;
	}

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	if(ihead.biBitCount == 24)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, ihead.biWidth, ihead.biHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, buffer);
	} else{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ihead.biWidth, ihead.biHeight, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer);
	}
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // �����˲�
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // �����˲�

	VirtualFree(buffer, 0, MEM_RELEASE);
	CloseHandle(hFile);
	return true;
}

_Success_(return) bool Model::LoadGLTexture(_In_ const char* filename, _Out_ unsigned int *texture){
	HRESULT hr = S_OK;
	HANDLE hFile = INVALID_HANDLE_VALUE;
	IWICImagingFactory* wicFactory = NULL;
	IWICBitmapDecoder* bitmapDecoder = NULL;
	IWICBitmapFrameDecode* frameDecode = NULL;
	IWICFormatConverter* WICBitmap = NULL;
	BYTE* buffer = NULL;
	UINT ow, oh;

	if(SUCCEEDED(hr)){
		hr = CoCreateInstance(CLSID_WICImagingFactory, NULL, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&wicFactory));
		if(FAILED(hr)){
			LOGE(����WIC����ʧ��);
		}
	}

	if(SUCCEEDED(hr)){
		hFile = CreateFileA(filename, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
		if(hFile == INVALID_HANDLE_VALUE){
			LOGE(���ļ�ʧ��);
			hr = E_FAIL;
		}
	}

	if(SUCCEEDED(hr)){
		//����ͼƬ
		hr = wicFactory->CreateDecoderFromFileHandle((ULONG_PTR)hFile, NULL, WICDecodeMetadataCacheOnLoad, &bitmapDecoder);
		if(FAILED(hr)){
			LOGE(���ļ�����������ʧ��);
		}
	}

	if(SUCCEEDED(hr)){
		//�����ʼ֡
		hr = bitmapDecoder->GetFrame(0, &frameDecode);
		if(FAILED(hr)){
			LOGE(�����һ֡ʧ�ܡ�);
		}
	}

	if(SUCCEEDED(hr)){
		//ת��ͼƬ��ʽ
		//(DXGI_FORMAT_B8G8R8A8_UNORM + D2D1_ALPHA_MODE_PREMULTIPLIED)
		hr = wicFactory->CreateFormatConverter(&WICBitmap);
		if(FAILED(hr)){
			LOGE(������ʽת����ʧ�ܡ�);
		}
	}

	if(SUCCEEDED(hr)){
		//�õ�ԭʼ�ߴ�
		hr = frameDecode->GetSize(&ow, &oh);
		if(FAILED(hr)){
			LOGE(����ͼƬʧ�ܡ�);
		}
	}

	if(SUCCEEDED(hr)){
		hr = WICBitmap->Initialize(
			frameDecode,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.0f,
			WICBitmapPaletteTypeMedianCut
			);
		if(FAILED(hr)){
			LOGE(��ʼ��λͼ����ʧ��);
		}
	}

	if(SUCCEEDED(hr)){
		buffer = new BYTE[oh * ow * 4];
	}

	if(SUCCEEDED(hr)){
		hr = WICBitmap->CopyPixels(NULL, ow * 4, oh * ow * 4, buffer);
		if(FAILED(hr)){
			LOGE(��������ʧ��);
		}
	}

	if(SUCCEEDED(hr)){
		glGenTextures(1, texture);
		glBindTexture(GL_TEXTURE_2D, *texture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, ow, oh, 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, buffer);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);  // �����˲�
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);  // �����˲�
	}

	if(buffer){
		delete[] buffer;
	}
	SafeRelease(&frameDecode);
	SafeRelease(&bitmapDecoder);
	SafeRelease(&wicFactory);
	CloseHandle(hFile);

	return SUCCEEDED(hr);
}

bool Model::LoadGLTexture(_In_ const char* filename){
	return LoadGLTexture(filename, &texture);
}

bool Model::InitializeTxt(_In_ const char* txtFile, _In_ const char* bmpFile){
	return LoadTxtModel(txtFile) && LoadGLTexture(bmpFile);
}

bool Model::Initialize(_In_ const char* binFile, _In_ const char* bmpFile){
	return LoadBinModel(binFile) && LoadGLTexture(bmpFile);
}

void Model::Render(){
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_TRIANGLES);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
	for(int i = 0; i < count; i++){
		glTexCoord2f(node[i].tu, node[i].tv);
		glNormal3f(node[i].nx, node[i].ny, node[i].nz);
		glVertex3f(node[i].x, node[i].y, node[i].z);
	}
	glEnd();
}

bool Model::SaveBinModel(_In_ const char* filename){
	HANDLE hFile = CreateFileA(filename, GENERIC_READ | GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, 0);
	if(hFile == INVALID_HANDLE_VALUE){
		return false;
	}

	DWORD fileSize = count * sizeof(NODE);
	DWORD dwSize;
	WriteFile(hFile, node, fileSize, &dwSize, NULL);
	if(dwSize != fileSize){
		CloseHandle(hFile);
		return false;
	}

	CloseHandle(hFile);
	return true;
}
