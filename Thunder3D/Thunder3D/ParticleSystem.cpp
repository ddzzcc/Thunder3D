#include "ParticleSystem.h"
#include <random>
#include <gl\GL.h>
using namespace std;

//��ʱ��ʩ���ṩ���������Դ
static default_random_engine re(GetTickCount());
static const float PI = 3.1415926535f;
const float ParticleSystem::TIME_THRES = 0.005f;

void ParticleSystem::Particle::DoWrap(_In_ const Vec4f& force, _In_ float turbul, _In_ float resist){
	float theta = uniform_real_distribution<float>(0, PI)(re);
	float phi = uniform_real_distribution<float>(0, 2 * PI)(re);
	float v = uniform_real_distribution<float>(0, turbul)(re);
	Vec4f t(v * sin(theta) * cos(phi), v * sin(theta) * sin(phi), v * cos(theta));
	speed = speed * (1 - resist) + t + force;
}

void ParticleSystem::Particle::Frame(_In_ float interval){
	//�����ٶ�
	Vec4f f = force;
	f.normal();
	f = f * 0.3f;
	DoWrap(f, 0.6f, 0.15f);
	DoWrap(Vec4f(), 0.25f, 0.01f);

	//����������λ��
	life -= interval * fade;
	pos = pos + speed * interval * 0.1f;
}

ParticleSystem::Particle::Particle(_In_ const Vec4f& pos, _In_ const Vec4f& speed, _In_ const Vec4f& force, _In_ const float fade) :
life(1.0f),
pos(pos),
speed(speed),
force(force),
fade(fade)
{
}

void ParticleSystem::Particle::Render(_In_ ICamera* camera){
	float r, g, b;
	float rnd = 1.0f - life;
	//������ɫ
	if(rnd < 0.5f){
		r = 0.92f * (1.0f - 2.0f * rnd) + 2.0f * rnd;
		g = 0.92f * (1.0f - 2.0f * rnd);
		b = 0;
	}else{
		r = 2.0f - 2.0f * rnd;
		g = 0;
		b = 0;
	}

	glPushMatrix();
	glTranslatef(pos.x, pos.y, pos.z);
	glScalef(0.3f, 0.3f, 0.3f);

	camera->MakeBillBoard();
	glBegin(GL_QUADS);
	glColor4f(r, g, b, life);
	glTexCoord2f(0.f, 0.f);
	glVertex3f(-1.f, 1.f, 0.0f);
	glTexCoord2f(1.f, 0.f);
	glVertex3f(1.f, 1.f, 0.0f);
	glTexCoord2f(1.f, 1.f);
	glVertex3f(1.f, -1.f, 0.0f);
	glTexCoord2f(0.f, 1.f);
	glVertex3f(-1.f, -1.f, 0.0f);
	glEnd();
	glPopMatrix();
}

void ParticleSystem::AddParticle(){
	float fade = uniform_real_distribution<float>(0.8f, 1.2f)(re);
	float dx = uniform_real_distribution<float>(-0.2f, 0.2f)(re);
	float dy = uniform_real_distribution<float>(-0.08f, 0.08f)(re) + 0.07f;
	Vec4f npos = pos + up * dy + nVec * dx;

	plist.push_back(
		Particle(npos, speed, - speed, fade)
		);
}

ParticleSystem::ParticleSystem(_In_ Craft* craft, _In_ ICamera* camera):
craft(craft),
camera(camera),
tim(0)
{
	craft->AddRef();
	camera->AddRef();

	pos = craft->GetPos();
	speed = craft->GetSpeed();
}

ParticleSystem::~ParticleSystem()
{
	SafeRelease(&craft);
	SafeRelease(&camera);
}

void ParticleSystem::Frame(_In_ float interval)
{
	//����λ��
	speed = craft->GetSpeed();
	pos = speed;
	pos.normal();
	up = craft->GetUp();
	up.normal();
	nVec = pos.cross(up);
	pos = craft->GetPos() - pos * 1.6f;

	//��������
	tim += interval;
	while(tim > TIME_THRES){
		tim -= TIME_THRES;
		AddParticle();
	}

	//��������
	for(list<Particle>::iterator it = plist.begin(); it != plist.end(); ){
		it->Frame(interval);
		if(it->life <= 0.0f){
			it = plist.erase(it);
		}else{
			++ it;
		}
	}
}

void ParticleSystem::Render(){
	glDisable(GL_LIGHTING);
	glDepthMask(GL_FALSE);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_TRUE);
	glBlendFunc(GL_ONE, GL_ONE);

	glBindTexture(GL_TEXTURE_2D, texture);
	for(list<Particle>::iterator it = plist.begin(); it != plist.end(); ++ it){
		it->Render(camera);
	}

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glLightModeli(GL_LIGHT_MODEL_TWO_SIDE, GL_FALSE);
	glEnable(GL_LIGHTING);
	glDepthMask(GL_TRUE);
}

bool ParticleSystem::Initialize(_In_ char* filename)
{
	return Model::LoadGLTexture(filename, &texture);
}

void ParticleSystem::UpdateCamera(_In_ ICamera* newcamera)
{
	SafeRelease(&camera);
	camera = newcamera;
	camera->AddRef();
}

ParticleSystem* ParticleSystem::create(_In_ Craft* craft, _In_ ICamera* camera)
{
	ParticleSystem* ret = new ParticleSystem(craft, camera);
	ret->AddRef();
	return ret;
}