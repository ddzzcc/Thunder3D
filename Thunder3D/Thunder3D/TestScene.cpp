#include "TestScene.h"

TestScene::TestScene():
	sun(NULL),
	earth(NULL),
	moon(NULL),
	craft(NULL),
	surbox(NULL),
	partisys(NULL),
	cubemap(NULL),
	lightcone(NULL)
{
}

TestScene::~TestScene()
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
	SafeRelease(&m_camera);
};

bool TestScene::Init(App* app)
{
	m_app = app;

	//��ʼ������ͷ�Ϳ�����
	UseStandardView();

	//��ʼ����������
	sun = Star::create(Vec4f(0.f, 0.f, 0.f, 1.f), 4.f, 2.f);
	earth = Planet::create(10.f, Vec4f(-0.15f, 1.f, 0.f), 0.f, 0.5f, sun, 1.0f);
	moon = Planet::create(3.f, Vec4f(0.1f, 1.f, 0.f), 0.f, 1.f, earth, 0.8f);
	craft = Craft::create(Vec4f(3.f, 3.f, 3.f, 1.f), Vec4f(0.5f, 0.f, 0.f), Vec4f(0.f, 0.5f, 0.f));
	partisys = ParticleSystem::create(craft, m_camera);
	lightcone = LightCone::create(craft);
	cubemap = CubeMap::create(500.0f);
	surbox = SurroundBox::create(NULL, m_camera);

	sun->Initialize("config/sphere.bin", "config/red.bmp");
	earth->Initialize("config/sphere.bin", "config/blue.bmp");
	moon->Initialize("config/sphere.bin", "config/gray.bmp");
	craft->Initialize("config/craft.bin");
	partisys->Initialize("config/Particle.png");
	lightcone->Initialize("config/cone.bin", "config/light.png");
	cubemap->Initialize("config/universeTexture.bmp");

	return true;
};

T3D::SCENE TestScene::Update(float interval)
{
	//����������
	//��ΪҪ�����ѡ�����Բ�������
	m_camera->LoadMatrix();

	//ǰ��һ֡
	sun->Frame(interval);
	earth->Frame(interval);
	moon->Frame(interval);
	craft->Frame(interval);
	surbox->Frame(interval);
	partisys->Frame(interval);
	cubemap->Frame(interval);
	lightcone->Frame(interval);

	m_ctrl->Frame(interval);

	//�������Ҫ�������
	//��Ȼ����ָ��ٵĶ���
	m_camera->Frame(interval);

	m_camera->LoadMatrix();
	cubemap->Render();
	sun->Render();
	earth->Render();
	moon->Render();
	craft->Render();
	surbox->Render();
	partisys->Render();
	lightcone->Render();

	return T3D::SCENE::TEST;
};

void TestScene::Reset()
{}

//������׼�������������
//ʹ�������ӽ�
//��Щ��������Ʋ������������
void TestScene::UseStandardView()
{
	//����ɵ�����ͷ
	SafeRelease(&m_camera);

	//��ʼ������ͷ
	FreeCamera* fcamera = FreeCamera::create(Vec4f(0.f, 0.f, 35.f, 1.f), Vec4f(0.f, 0.f, 0.f, 1.f), Vec4f(0.f, 1.f, 0.f));
	fcamera->QueryInterface(ICamera::IID_ICamera, (void**)&m_camera);

	//��ʼ��������
	Controller* newcontrol = new CameraController(fcamera, m_app,this);

	//�޸���ض��������ͷ
	if (surbox) {
		surbox->UpdateCamera(m_camera);
	}
	if (partisys) {
		partisys->UpdateCamera(m_camera);
	}

	//�ͷ���ʱ����
	SafeRelease(&fcamera);

	//���ӿ�����
	if (m_ctrl) {
		newcontrol->CopyKeyState(m_ctrl);
		delete m_ctrl;
	}
	m_ctrl = newcontrol;
};

//�������ٿ������������
void TestScene::UseTraceView(_In_ IObject* obj)
{
	//����ɵ�����ͷ
	SafeRelease(&m_camera);

	//��ʼ������ͷ
	TraceCamera* tcamera = TraceCamera::create(obj);
	tcamera->QueryInterface(ICamera::IID_ICamera, (void**)&m_camera);

	//��ʼ��������
	Controller* newcontrol = new TraceController(m_app, this);

	//�޸���ض��������ͷ
	if (surbox) {
		surbox->UpdateCamera(m_camera);
	}
	if (partisys) {
		partisys->UpdateCamera(m_camera);
	}

	//�ͷ���ʱ����
	SafeRelease(&tcamera);

	//���ӿ�����
	if (m_ctrl) {
		newcontrol->CopyKeyState(m_ctrl);
		delete m_ctrl;
	}
	m_ctrl = newcontrol;
};

//�����ɴ��������������
void TestScene::UseCraftView()
{
	//����ɵ�����ͷ
	SafeRelease(&m_camera);

	//��ʼ������ͷ
	CraftCamera* ccamera = CraftCamera::create(craft);
	ccamera->QueryInterface(ICamera::IID_ICamera, (void**)&m_camera);

	//��ʼ��������
	Controller* newcontrol = new CraftController(m_app, craft);

	//�޸���ض��������ͷ
	if (surbox) {
		surbox->UpdateCamera(m_camera);
	}
	if (partisys) {
		partisys->UpdateCamera(m_camera);
	}

	//�ͷ���ʱ����
	SafeRelease(&ccamera);

	//���ӿ�����
	if (m_ctrl) {
		newcontrol->CopyKeyState(m_ctrl);
		delete m_ctrl;
	}
	m_ctrl = newcontrol;
};

void TestScene::OnSelect(_In_ const Vec4f& point)
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

void TestScene::OnTrace(_In_ const Vec4f& point){
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