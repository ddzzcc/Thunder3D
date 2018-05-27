#ifndef _CONTROLLER_H
#define _CONTROLLER_H

#include "common.h"
#include "camera.h"
#include "craft.h"

class App;

class Controller{
protected:
	//������״̬
	enum{
		KEY_PRESS,
		KEY_RELEASE,
		KEY_DOWN,
		KEY_UP
	} keys[256], lbutton, rbutton;

	//����λ�ã�����ʱ�޶���
	int mouseX, mouseY;

	//��������ָ��
	App* app;

	Controller(_In_ App* app);
public:
	//�������µ��¼��ص�
	void OnKeyDown(_In_ UINT key);

	//�����ɿ����¼��ص�
	void OnKeyUp(_In_ UINT key);

	//��갴�µ��¼��ص�
	void OnMouseDown(_In_ bool isLeft, _In_ int x, _In_ int y);

	//����ɿ����¼��ص�
	void OnMouseUp(_In_ bool isLeft, _In_ int x, _In_ int y);

	//ת��������굽���е�
	Vec4f GetHitPoint();

	//�����������
	//����ʹ�õķ���
	void UpdateMousePos();

	//ǰ��һ֡
	virtual void Frame(_In_ float interval);

	//������������
	virtual ~Controller(){};

	//���ư���״̬
	void CopyKeyState(_In_ Controller* src);
};

//////////////////////////////////////////////////////////////////////////

class CameraController : public Controller{
protected:
	//���Ƶ����������
	FreeCamera* camera;

	//ƽ���ٶ�
	static const float TRANS_SPEED;

	//��ת�ٶ�
	static const float ROTATE_SPEED;

public:
	CameraController(_In_ FreeCamera* camera, _In_ App* app);
	~CameraController();

	void Frame(_In_ float interval);
};

//////////////////////////////////////////////////////////////////////////

class TraceController : public Controller{
public:
	TraceController(_In_ App* app);

	void Frame(_In_ float interval);
};

//////////////////////////////////////////////////////////////////////////

class CraftController : public Controller{
private:
	//�ɴ�
	Craft* craft;

	//ƽ���ٶ�
	static const float TRANS_SPEED;

	//��ת�ٶ�
	static const float ROTATE_SPEED;

public:
	CraftController(_In_ App* app, _In_ Craft* craft);

	~CraftController();

	void Frame(_In_ float interval);
};

#endif
