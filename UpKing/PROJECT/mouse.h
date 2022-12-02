//=============================================================================
//
// ���͏��� [gamepad.h]
// Author : �O��q��
//
//=============================================================================
#ifndef _MOUSE_H_
#define _MOUSE_H_
#include "main.h"
#include "input.h"

class CMouse : CInput
{
public:
	typedef enum
	{
		DIM_L = 0, //���N���b�N
		DIM_R,     //�E�N���b�N
		DIM_W,      //�z�C�[���{�^��
		DIM_MAX
	}MouseButton;
	//�v���g�^�C�v�錾
	CMouse();
	~CMouse();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(MouseButton Button) const;
	bool GetTrigger(MouseButton Button) const;
	bool GetRelease(MouseButton Button) const;
	bool Mouse() const;
	float MouseX() const;
	float MouseY() const;
	float MouseZ() const;
	bool GetMouseButton(MouseButton Button) const;

private:
	DIMOUSESTATE m_MouseState;              //�}�E�X�̓��͏��(�v���X���)
	DIMOUSESTATE m_MouseStateTrigger;
	DIMOUSESTATE m_MouseStateRelease;
	bool m_bMouse;
};
#endif