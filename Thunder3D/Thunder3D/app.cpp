#include "app.h"

LPCTSTR App::title = TEXT("Assignment");

typedef void (APIENTRY *PFNWGLEXTSWAPCONTROLPROC) (int);
PFNWGLEXTSWAPCONTROLPROC wglSwapIntervalEXT = NULL;

LRESULT App::StaticWndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	App* app;
	if(msg == WM_CREATE){
		LPCREATESTRUCT createStruct = reinterpret_cast<LPCREATESTRUCT>(lParam);
		app = reinterpret_cast<App*>(createStruct->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, reinterpret_cast<ULONG_PTR>(app));
		return 1;
	} else{
		app = reinterpret_cast<App*>(GetWindowLongPtr(hwnd, GWLP_USERDATA));
		return app->WndProc(hwnd, msg, wParam, lParam);
	}
}

LRESULT App::WndProc(_In_ HWND hwnd, _In_ UINT msg, _In_ WPARAM wParam, _In_ LPARAM lParam){
	switch(msg){
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if(control){
			control->OnKeyDown(wParam);
		}
		return 0;
	case WM_KEYUP:
		if(control){
			control->OnKeyUp(wParam);
		}
		return 0;
	case WM_LBUTTONDOWN:
		if(control){
			control->OnMouseDown(true, LOWORD(lParam), HIWORD(lParam));
		}
		return 0;
	case WM_LBUTTONUP:
		if(control){
			control->OnMouseUp(true, LOWORD(lParam), HIWORD(lParam));
		}
		return 0;
	case WM_RBUTTONDOWN:
		if(control){
			control->OnMouseDown(false, LOWORD(lParam), HIWORD(lParam));
		}
		return 0;
	case WM_RBUTTONUP:
		if(control){
			control->OnMouseUp(false, LOWORD(lParam), HIWORD(lParam));
		}
		return 0;
	case WM_SIZE:
		ReSizeGLScene(LOWORD(lParam), HIWORD(lParam));
		return 0;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}
}

bool App::CreateGLWindow(int width, int height){
	GLuint PixelFormat;
	WNDCLASS wc;
	DWORD dwExStyle;
	DWORD dwStyle;
	RECT WindowRect;
	static	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW |
		PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER,
		PFD_TYPE_RGBA,
		32,
		0, 0, 0, 0, 0, 0,
		0,
		0,
		0,
		0, 0, 0, 0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0, 0, 0
	};

	WindowRect.left = (long)0;
	WindowRect.right = (long)width;
	WindowRect.top = (long)0;
	WindowRect.bottom = (long)height;

	hInstance = GetModuleHandle(NULL);
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = (WNDPROC)StaticWndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = NULL;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = title;

	if(!RegisterClass(&wc))
	{
		MessageBox(NULL, TEXT("����ע�ᴰ����."), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
	dwStyle = WS_OVERLAPPEDWINDOW;

	AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);		// �������ڳߴ�

	// Create The Window
	hWnd = CreateWindowEx(dwExStyle,
		title,
		title,
		dwStyle |
		WS_CLIPSIBLINGS |
		WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT,
		WindowRect.right - WindowRect.left,
		WindowRect.bottom - WindowRect.top,
		NULL,
		NULL,
		hInstance,
		this);
	if(!hWnd)
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("���ڴ�������."), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;								// ����ʧ��
	}

	hDC = GetDC(hWnd);
	if(!hDC)
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("���ܴ���GL�豸������."), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	PixelFormat = ChoosePixelFormat(hDC, &pfd);
	if(!PixelFormat)
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("����ƥ�䵽���ʵ����ظ�ʽ."), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(!SetPixelFormat(hDC, PixelFormat, &pfd))
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("�����������ظ�ʽ."), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	hRC = wglCreateContext(hDC);
	if(!hRC)
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("���ܴ���GL��Ⱦ������."), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(!wglMakeCurrent(hDC, hRC))
	{
		KillGLWindow();
		MessageBox(NULL, TEXT("���ܼ���GL��Ⱦ������."), TEXT("����"), MB_OK | MB_ICONEXCLAMATION);
		return false;
	}

	if(wglSwapIntervalEXT == NULL){
		wglSwapIntervalEXT = (PFNWGLEXTSWAPCONTROLPROC)wglGetProcAddress("wglSwapIntervalEXT");
	}
	wglSwapIntervalEXT(1);

	ShowWindow(hWnd, SW_SHOW);
	SetForegroundWindow(hWnd);
	SetFocus(hWnd);
	ReSizeGLScene(width, height);

	return true;
}

void App::KillGLWindow()
{
	if(hRC)
	{
		if(!wglMakeCurrent(NULL, NULL))
		{
			LOGE(DC��RC�ͷ�ʧ��);
		}

		if(!wglDeleteContext(hRC))
		{
			LOGE(��Ⱦ�������ͷ�ʧ��);
		}
		hRC = NULL;
	}

	if(hDC && !ReleaseDC(hWnd, hDC))
	{
		LOGE(�豸�������ͷ�ʧ��);
	}
	hDC = NULL;

	if(hWnd && !DestroyWindow(hWnd))
	{
		LOGE(�����ͷ�hWnd);
	}
	hWnd = NULL;

	if(!UnregisterClass(title, hInstance))
	{
		LOGE(����ȡ��������);
	}
}

void App::ReSizeGLScene(GLsizei width, GLsizei height)
{
	if(height == 0){
		height = 1;
	}

	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//���㴰�ڱ���
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 900.0f);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool App::Init(){
	lastTick = GetTickCount();

	//��ʼ��GL����
	glClearColor(0, 0, 0, 0);
	glShadeModel(GL_SMOOTH);
	glFrontFace(GL_CCW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	//��ʼ�����ʣ�ȡ����������
	glEnable(GL_TEXTURE_2D);
	//glEnable(GL_TEXTURE_GEN_S);
	//glEnable(GL_TEXTURE_GEN_T);

	//��ʼ����Դ
	glEnable(GL_LIGHTING);

	//�ӵ�ǰ����Դ
	//���Դ��ģ���������

	//��ʼ������ͷ�Ϳ�����
	UseStandardView();

	//��ʼ����������
	sun = Star::create(Vec4f(0.f, 0.f, 0.f, 1.f), 4.f, 2.f);
	earth = Planet::create(10.f, Vec4f(-0.15f, 1.f, 0.f), 0.f, 0.5f, sun, 1.0f);
	moon = Planet::create(3.f, Vec4f(0.1f, 1.f, 0.f), 0.f, 1.f, earth, 0.8f);
	craft = Craft::create(Vec4f(3.f, 3.f, 3.f, 1.f), Vec4f(0.5f, 0.f, 0.f), Vec4f(0.f, 0.5f, 0.f));
	surbox = SurroundBox::create(NULL, camera);
	partisys = ParticleSystem::create(craft, camera);
	cubemap = CubeMap::create(500.0f);
	lightcone = LightCone::create(craft);

	sun->Initialize("config/sphere.bin", "config/red.bmp");
	earth->Initialize("config/sphere.bin", "config/blue.bmp");
	moon->Initialize("config/sphere.bin", "config/gray.bmp");
	craft->Initialize("config/craft.bin");
	partisys->Initialize("config/Particle.png");
	cubemap->Initialize("config/universe.bmp");
	lightcone->Initialize("config/cone.bin", "config/light.png");

	return true;
}

void App::Run(){
	DWORD now;
	float interval;


	MSG msg;
	CreateGLWindow(640, 480);
	Init();
	while(true){
		if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
			{
				break;
			} else
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);

				//�����Ϣ�������������ڣ������Ҫ������һ֡����ʱ��
				if(msg.message == WM_NCLBUTTONDOWN){
					lastTick = GetTickCount();
					continue;;
				}
			}
		}
		//����ʱ��
		now = GetTickCount();
		interval = (now - lastTick) * 0.001f;
		lastTick = now;

		//����������
		//��ΪҪ�����ѡ�����Բ�������
		camera->LoadMatrix();
		control->Frame(interval);

		// �����Ļ����Ȼ���
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		//ǰ��һ֡
		sun->Frame(interval);
		earth->Frame(interval);
		moon->Frame(interval);
		craft->Frame(interval);
		surbox->Frame(interval);
		partisys->Frame(interval);
		cubemap->Frame(interval);
		lightcone->Frame(interval);

		//�������Ҫ�������
		//��Ȼ����ָ��ٵĶ���
		camera->Frame(interval);

		camera->LoadMatrix();
		cubemap->Render();
		sun->Render();
		earth->Render();
		moon->Render();
		craft->Render();
		surbox->Render();
		partisys->Render();
		lightcone->Render();

		SwapBuffers(hDC);
	}
	KillGLWindow();
}

App::App() :
sun(NULL),
earth(NULL),
moon(NULL),
craft(NULL),
surbox(NULL),
partisys(NULL),
cubemap(NULL),
lightcone(NULL),
camera(NULL),
control(NULL)
{
}

App::~App()
{
	//�ͷų�������
	SafeRelease(&sun);
	SafeRelease(&earth);
	SafeRelease(&moon);
	SafeRelease(&craft);
	SafeRelease(&surbox);
	SafeRelease(&partisys);
	SafeRelease(&cubemap);
	SafeRelease(&lightcone);

	//�ͷ�����ͷ�Ϳ�����
	SafeRelease(&camera);
	if(control){
		delete control;
	}
}

void App::OnSelect(_In_ const Vec4f& point)
{
	IObject* obj = NULL;

	//��Ч�ĵ�ѡ
	if(point.w <= 0.001){
		surbox->UpdateObject(NULL);
		return;
	}

	//�ж���ײ����
	if(sun->Collide(point)){
		sun->QueryInterface(IObject::IID_IObject, (void**)&obj);
	} else if(earth->Collide(point)){
		earth->QueryInterface(IObject::IID_IObject, (void**)&obj);
	} else if(moon->Collide(point)){
		moon->QueryInterface(IObject::IID_IObject, (void**)&obj);
	}

	//����Ŀ�����
	surbox->UpdateObject(obj);

	//�ͷ���ʱ����
	SafeRelease(&obj);
}

void App::UseStandardView(){
	//����ɵ�����ͷ
	SafeRelease(&camera);

	//��ʼ������ͷ
	FreeCamera* fcamera = FreeCamera::create(Vec4f(0.f, 0.f, 35.f, 1.f), Vec4f(0.f, 0.f, 0.f, 1.f), Vec4f(0.f, 1.f, 0.f));
	fcamera->QueryInterface(ICamera::IID_ICamera, (void**)&camera);

	//��ʼ��������
	Controller* newcontrol = new CameraController(fcamera, this);

	//�޸���ض��������ͷ
	if(surbox){
		surbox->UpdateCamera(camera);
	}
	if(partisys){
		partisys->UpdateCamera(camera);
	}

	//�ͷ���ʱ����
	SafeRelease(&fcamera);

	//���ӿ�����
	if(control){
		newcontrol->CopyKeyState(control);
		delete control;
	}
	control = newcontrol;
}

void App::UseTraceView(_In_ IObject* obj){
	//����ɵ�����ͷ
	SafeRelease(&camera);

	//��ʼ������ͷ
	TraceCamera* tcamera = TraceCamera::create(obj);
	tcamera->QueryInterface(ICamera::IID_ICamera, (void**)&camera);

	//��ʼ��������
	Controller* newcontrol = new TraceController(this);

	//�޸���ض��������ͷ
	if(surbox){
		surbox->UpdateCamera(camera);
	}
	if(partisys){
		partisys->UpdateCamera(camera);
	}

	//�ͷ���ʱ����
	SafeRelease(&tcamera);

	//���ӿ�����
	if(control){
		newcontrol->CopyKeyState(control);
		delete control;
	}
	control = newcontrol;
}

void App::OnTrace(_In_ const Vec4f& point){
	IObject* obj = NULL;

	//��Ч�ĵ�ѡ
	if(point.w <= 0.001){
		return;
	}

	//�ж���ײ����
	if(sun->Collide(point)){
		sun->QueryInterface(IObject::IID_IObject, (void**)&obj);
	} else if(earth->Collide(point)){
		earth->QueryInterface(IObject::IID_IObject, (void**)&obj);
	} else if(moon->Collide(point)){
		moon->QueryInterface(IObject::IID_IObject, (void**)&obj);
	} else if(craft->Collide(point)){
		craft->QueryInterface(IObject::IID_IObject, (void**)&obj);
	} else{
		return;
	}

	//��������ͷ������
	UseTraceView(obj);

	//�ͷ���ʱ����
	SafeRelease(&obj);
}

void App::UseCraftView(){
	//����ɵ�����ͷ
	SafeRelease(&camera);

	//��ʼ������ͷ
	CraftCamera* ccamera = CraftCamera::create(craft);
	ccamera->QueryInterface(ICamera::IID_ICamera, (void**)&camera);

	//��ʼ��������
	Controller* newcontrol = new CraftController(this, craft);

	//�޸���ض��������ͷ
	if(surbox){
		surbox->UpdateCamera(camera);
	}
	if(partisys){
		partisys->UpdateCamera(camera);
	}

	//�ͷ���ʱ����
	SafeRelease(&ccamera);

	//���ӿ�����
	if(control){
		newcontrol->CopyKeyState(control);
		delete control;
	}
	control = newcontrol;
}
