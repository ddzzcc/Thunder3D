#ifndef _APP_H
#define _APP_H

#include "common.h"
#include <windows.h>
#include <gl\GL.h>
#include <gl\GLU.h>

#include "planet.h"
#include "star.h"
#include "craft.h"
#include "camera.h"
#include "controller.h"
#include "SurroundBox.h"
#include "ParticleSystem.h"
#include "cubemap.h"
#include "LightCone.h"

/**
* Ӧ�ó���ͳ�����
* ��Ϊֻ��һ���������Լ򻯳�һ������
*/
class App
{
private:
	static LPCTSTR title;

	HWND hWnd;
	HINSTANCE hInstance;
	HDC hDC;
	HGLRC hRC;
	DWORD lastTick;

	static LRESULT CALLBACK StaticWndProc(HWND, UINT, WPARAM, LPARAM);
	LRESULT WndProc(_In_ HWND hwnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam);

	bool CreateGLWindow(int width, int height);

	void KillGLWindow();

	void ReSizeGLScene(GLsizei width, GLsizei height);

	bool Init();
public:
	void Run();

	App();

	~App();

	HWND GetHwnd();

	//ѡ������¼�
	//point : ѡ���
	void OnSelect(_In_ const Vec4f& point);

	//׷�ٶ����¼�
	//point : ѡ���
	void OnTrace(_In_ const Vec4f& point);

	//������׼�������������
	//ʹ�������ӽ�
	//��Щ��������Ʋ������������
	void UseStandardView();

	//�������ٿ������������
	void UseTraceView(_In_ IObject* obj);

	//�����ɴ��������������
	void UseCraftView();

private:
	Star* sun;
	Planet* earth;
	Planet* moon;
	Craft* craft;
	SurroundBox* surbox;
	ParticleSystem* partisys;
	CubeMap* cubemap;
	LightCone* lightcone;

	ICamera* camera;
	Controller* control;
};

inline HWND App::GetHwnd(){
	return hWnd;
}

#endif