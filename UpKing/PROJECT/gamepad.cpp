//=============================================================================
// 入力処理 [gamepad.h]
// Author : 三上航世
//=============================================================================
#include "gamepad.h"

bool CGamePad::m_bGamePad = false;

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef);

CGamePad::CGamePad()
{
	m_pDevice = NULL;
	m_bGamePad = false;
}

CGamePad::~CGamePad()
{

}

HRESULT CGamePad::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInitオブジェクトの生成
	CInput::Init(hInstance, hWnd);
	//接続されてるゲームパッドの列挙
	if (FAILED(m_pInput->EnumDevices(DI8DEVCLASS_GAMECTRL, DeviceFindCallBack, NULL, DIEDFL_ATTACHEDONLY)))
	{
		return E_FAIL;
	}
	//入力デバイスの生成
	if (FAILED(m_pInput->CreateDevice(GUID_Joystick, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	// データフォーマットを設定
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIJoystick)))
	{
		return E_FAIL;
	}
	// 軸モードを絶対値モードとして設定
	DIPROPDWORD diprop;
	ZeroMemory(&diprop, sizeof(diprop));
	diprop.diph.dwSize = sizeof(diprop);
	diprop.diph.dwHeaderSize = sizeof(diprop.diph);
	diprop.diph.dwHow = DIPH_DEVICE;
	diprop.diph.dwObj = 0;
	diprop.dwData = DIPROPAXISMODE_ABS;

	// 軸モードを変更
	if (FAILED(m_pDevice->SetProperty(DIPROP_AXISMODE, &diprop.diph)))
	{
		return E_FAIL;
	}

	// X軸の値の範囲設定
	DIPROPRANGE diprg;
	ZeroMemory(&diprg, sizeof(diprg));
	diprg.diph.dwSize = sizeof(diprg);
	diprg.diph.dwHeaderSize = sizeof(diprg.diph);
	diprg.diph.dwHow = DIPH_BYOFFSET;
	diprg.diph.dwObj = DIJOFS_X;
	diprg.lMin = -1000;
	diprg.lMax = 1000;
	if (FAILED(m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return E_FAIL;
	}

	// Y軸の値の範囲設定
	diprg.diph.dwObj = DIJOFS_Y;
	if (FAILED(m_pDevice->SetProperty(DIPROP_RANGE, &diprg.diph)))
	{
		return E_FAIL;
	}

	// 協調モードを設定
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}
	// ゲームパッドへのアクセス権を獲得(入力制御開始)
	m_pDevice->Acquire();
	//処理を同期させる
	m_pDevice->Poll();
	m_bGamePad = true;
	return S_OK;
}

void CGamePad::Uninit()
{
	CInput::Uninit();
}

void CGamePad::Update()
{
	DIJOYSTATE aPadState;
	int nCntPad;
	//入力デバイスからデータを取得する
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aPadState), &aPadState)))
	{
		for (nCntPad = 0; nCntPad < DIP_MAX; nCntPad++)
		{
			m_aPadStateTrigger.rgbButtons[nCntPad] = m_aPadState.rgbButtons[nCntPad] ^ aPadState.rgbButtons[nCntPad] & aPadState.rgbButtons[nCntPad];
			m_aPadStateRelease.rgbButtons[nCntPad] = (m_aPadState.rgbButtons[nCntPad] ^ aPadState.rgbButtons[nCntPad]) & m_aPadState.rgbButtons[nCntPad];
			m_aPadState.rgbButtons[nCntPad] = aPadState.rgbButtons[nCntPad]; //キーボードの入力情報保存
		}
		m_aPadState.lX = aPadState.lX;
		m_aPadState.lY = aPadState.lY;
		m_aPadState.lZ = aPadState.lZ;
		m_aPadState.lRz = aPadState.lRz;
		m_aPadStateTrigger.rgdwPOV[0] = m_aPadState.rgdwPOV[0];
		m_aPadState.rgdwPOV[0] = aPadState.rgdwPOV[0];
	}
	else
	{
		m_pDevice->Acquire();
	}
}

BOOL CALLBACK DeviceFindCallBack(LPCDIDEVICEINSTANCE ipddi, LPVOID pvRef)
{
	return DIENUM_CONTINUE;
}

bool CGamePad::GetAnyButton()
{
	int nCntPad;
	for (nCntPad = 0; nCntPad < DIP_MAX; nCntPad++)
	{
		if ((m_aPadStateTrigger.rgbButtons[nCntPad] & 0x80) != 0 && (m_aPadStateRelease.rgbButtons[nCntPad] & 0x80) == 0)
		{
			return true;
		}
	}
	return false;
}

bool CGamePad::GetButton(CGamePad::PadButton Button)
{
	if ((m_aPadStateTrigger.rgbButtons[Button] & 0x80) != 0 && (m_aPadStateRelease.rgbButtons[Button] & 0x80) == 0)
	{
		return true;
	}
	return false;
}

bool CGamePad::GetGamePad()
{
	return m_bGamePad;
}

bool CGamePad::GetPress(CGamePad::PadButton Button)
{
	return (m_aPadState.rgbButtons[Button] & 0x80) ? true : false;
}

bool CGamePad::GetRelease(CGamePad::PadButton Button)
{
	return (m_aPadStateRelease.rgbButtons[Button] & 0x80) ? true : false;
}

bool CGamePad::GetTrigger(CGamePad::PadButton Button)
{
	return (m_aPadStateTrigger.rgbButtons[Button] & 0x80) ? true : false;
}

float CGamePad::TriggerCrossKey()
{
	//十字キーが入力されてるか
	if (m_aPadState.rgdwPOV[0] != 0xFFFFFFFF)
	{
		//直前に押された方向と違うか（同じだったら無視する）
		if (m_aPadStateTrigger.rgdwPOV[0] != m_aPadState.rgdwPOV[0])
		{
			return m_aPadState.rgdwPOV[0];
		}
	}
	return 10.0f; //何も押されてなかったとき用
}

float CGamePad::PressCrossKey()
{
	//十字キーが入力されてるか
	if (m_aPadState.rgdwPOV[0] != 0xFFFFFFFF)
	{
		return m_aPadState.rgdwPOV[0];
	}
	return 10.0f; //何も押されてなかったとき用
}

float CGamePad::LeftStickX()
{
	float length = 1000.0f;
	float LeftX = 0.0f;
	if (m_aPadState.lX != 0)
	{
		LeftX = (float)m_aPadState.lX / length; //左はマイナス、右はプラス
	}
	return LeftX;
}

float CGamePad::LeftStickY()
{
	float length = 1000.0f;
	float LeftY = 0.0f;
	if (m_aPadState.lY != 0)
	{
		LeftY = (float)m_aPadState.lY / length; //上はマイナス、下はプラス
	}
	return LeftY;
}

float CGamePad::RightStickX()
{
	float length = 32767.0f;
	float RightX = 0.0f;
	if (m_aPadState.lZ != 32767)
	{
		RightX = (float)(m_aPadState.lZ - length) / length; //左はマイナス、右はプラス
	}
	return RightX;
}

float CGamePad::RightStickY()
{
	float length = 32767.0f;
	float RightY = 0.0f;
	if (m_aPadState.lRz != 32767)
	{
		RightY = (float)(m_aPadState.lRz - length) / length; //上はマイナス、下はプラス
	}
	return RightY;
}