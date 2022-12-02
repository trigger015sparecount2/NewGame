//=============================================================================
//
// 入力処理 [input.h]
// Author : 三上航世
//
//=============================================================================
#include "input.h"
//静的メンバ変数
LPDIRECTINPUT8 CInput::m_pInput = NULL;

CInput::CInput()
{
	m_pDevice = NULL;
}

CInput::~CInput()
{

}

HRESULT CInput::Init(HINSTANCE hInstance, HWND hWnd)
{
	HRESULT hr = S_FALSE;
	if (m_pInput == NULL)
	{
		//DirectInitオブジェクトの生成
		hr = DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pInput, NULL);
	}
	return hr;
}

void CInput::Uninit()
{
	//入力デバイスの開放
	if (m_pDevice != NULL)
	{
		m_pDevice->Unacquire();
		m_pDevice->Release();
		m_pDevice = NULL;
	}
	//DirectInputオブジェクトの開放
	if (m_pInput != NULL)
	{
		m_pInput->Release();
		m_pInput = NULL;
	}
}