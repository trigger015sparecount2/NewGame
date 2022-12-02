//=============================================================================
//
// 入力処理 [input.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_
#include "main.h"

class CInput
{
public:
	CInput();
	virtual ~CInput();

	virtual HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	virtual void Uninit();
	virtual void Update() = 0;

protected:
	static LPDIRECTINPUT8 m_pInput;             //DirectInputオブジェクトのポインタ
	LPDIRECTINPUTDEVICE8 m_pDevice; //入力デバイス(キーボード)へのポインタ
};
#endif