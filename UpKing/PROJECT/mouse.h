//=============================================================================
//
// 入力処理 [gamepad.h]
// Author : 三上航世
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
		DIM_L = 0, //左クリック
		DIM_R,     //右クリック
		DIM_W,      //ホイールボタン
		DIM_MAX
	}MouseButton;
	//プロトタイプ宣言
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
	DIMOUSESTATE m_MouseState;              //マウスの入力情報(プレス情報)
	DIMOUSESTATE m_MouseStateTrigger;
	DIMOUSESTATE m_MouseStateRelease;
	bool m_bMouse;
};
#endif