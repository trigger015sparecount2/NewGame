//=============================================================================
//
// 入力処理 [keyboard.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _KEYBOARD_H_
#define _KEYBOARD_H_
#include "main.h"
#include "input.h"

//マクロ定義
#define NUM_KEY_MAX (256) //キーの最大数

class CKeyboard : CInput
{
public:
	CKeyboard();
	~CKeyboard();
	HRESULT Init(HINSTANCE hInstance, HWND hWnd);
	void Uninit();
	void Update();
	bool GetPress(int nKey);
	bool GetAnyKey();
	bool GetTrigger(int nKey);
	bool GetRelease(int nKey);
	bool GetKey(int nKey);

private:
	BYTE m_aKeyState[NUM_KEY_MAX];        //キーボードの入力情報(プレス情報)
	BYTE m_aKeyStateTrigger[NUM_KEY_MAX];
	BYTE m_aKeyStateRelease[NUM_KEY_MAX];
};
#endif