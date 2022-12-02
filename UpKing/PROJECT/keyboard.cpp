//=============================================================================
//
// 入力処理 [keyboard.h]
// Author : 三上航世
//
//=============================================================================
#include "keyboard.h"

CKeyboard::CKeyboard()
{
	m_pDevice = NULL;
	//キー情報のクリア
	memset(&m_aKeyState[0], 0, sizeof(m_aKeyState));
	memset(&m_aKeyStateRelease[0], 0, sizeof(m_aKeyStateRelease));
	memset(&m_aKeyStateTrigger[0], 0, sizeof(m_aKeyStateTrigger));
}

CKeyboard::~CKeyboard()
{

}

//初期化処理
HRESULT CKeyboard::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInitオブジェクトの生成
	CInput::Init(hInstance, hWnd);
	//入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_SysKeyboard, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIKeyboard)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();
	return S_OK;
}

//終了処理
void CKeyboard::Uninit()
{
	CInput::Uninit();
}

//更新処理
void CKeyboard::Update()
{
	BYTE aKeyState[NUM_KEY_MAX];
	int nCntKey;
	//入力デバイスからデータを取得する
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aKeyState), &aKeyState[0])))
	{
		for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
		{
			m_aKeyStateTrigger[nCntKey] = m_aKeyState[nCntKey] ^ aKeyState[nCntKey] & aKeyState[nCntKey];
			m_aKeyStateRelease[nCntKey] = (m_aKeyState[nCntKey] ^ aKeyState[nCntKey]) & m_aKeyState[nCntKey];
			m_aKeyState[nCntKey] = aKeyState[nCntKey]; //キーボードの入力情報保存
		}
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//キーボードの入力情報を取得
bool CKeyboard::GetPress(int nKey)
{
	return (m_aKeyState[nKey] & 0x80) ? true : false;
}

bool CKeyboard::GetTrigger(int nKey)
{
	return (m_aKeyStateTrigger[nKey] & 0x80) ? true : false;

}

bool CKeyboard::GetRelease(int nKey)
{
	return (m_aKeyStateRelease[nKey] & 0x80) ? true : false;
}

//何かキーが押されたか
bool CKeyboard::GetAnyKey()
{
	int nCntKey;
	for (nCntKey = 0; nCntKey < NUM_KEY_MAX; nCntKey++)
	{
		if ((m_aKeyStateTrigger[nCntKey] & 0x80) != 0 && (m_aKeyStateRelease[nCntKey] & 0x80) == 0)
		{
			return true;
		}
	}
	return false;
}

//特定のキーが押されたか
bool CKeyboard::GetKey(int nKey)
{
	if ((m_aKeyStateTrigger[nKey] & 0x80) != 0 && (m_aKeyStateRelease[nKey] & 0x80) == 0)
	{
		return true;
	}
	return false;
}