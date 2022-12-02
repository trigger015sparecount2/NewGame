//--------------------------------------
//Author:�O��q��
//�}�E�X�̓��͏��(mouse.cpp)
//---------------------------------------
#include "mouse.h"

CMouse::CMouse()
{
	m_pDevice = NULL;
}

CMouse::~CMouse()
{

}


//����������
HRESULT CMouse::Init(HINSTANCE hInstance, HWND hWnd)
{
	//DirectInit�I�u�W�F�N�g�̐���
	if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (void **)&m_pInput, NULL)))
	{
		return E_FAIL;
	}
	//���̓f�o�C�X�̐���
	if (FAILED(m_pInput->CreateDevice(GUID_SysMouse, &m_pDevice, NULL)))
	{
		return E_FAIL;
	}
	// �f�[�^�t�H�[�}�b�g��ݒ�
	if (FAILED(m_pDevice->SetDataFormat(&c_dfDIMouse)))
	{
		return E_FAIL;
	}

	// �������[�h��ݒ�
	if (FAILED(m_pDevice->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		return E_FAIL;
	}

	//�����𓯊�������
	m_pDevice->Poll();
	// �}�E�X�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDevice->Acquire();
	m_bMouse = true;
	return S_OK;
}

//�I������
void CMouse::Uninit()
{
	CInput::Uninit();
}

//�X�V����
void CMouse::Update()
{
	DIMOUSESTATE aMouseState;
	int nCntBut;
	//���̓f�o�C�X����f�[�^���擾����
	if (SUCCEEDED(m_pDevice->GetDeviceState(sizeof(aMouseState), &aMouseState)))
	{
		for (nCntBut = 0; nCntBut < DIM_MAX; nCntBut++)
		{
			m_MouseStateTrigger.rgbButtons[nCntBut] = m_MouseState.rgbButtons[nCntBut] ^ aMouseState.rgbButtons[nCntBut] & aMouseState.rgbButtons[nCntBut];
			m_MouseStateRelease.rgbButtons[nCntBut] = (m_MouseState.rgbButtons[nCntBut] ^ aMouseState.rgbButtons[nCntBut]) & m_MouseState.rgbButtons[nCntBut];
			m_MouseState.rgbButtons[nCntBut] = aMouseState.rgbButtons[nCntBut]; //�}�E�X�̓��͏��ۑ�
		}
		m_MouseState.lX = aMouseState.lX;
		m_MouseState.lY = aMouseState.lY;
		m_MouseState.lZ = aMouseState.lZ;
	}
	else
	{
		m_pDevice->Acquire();
	}
}

//�}�E�X�̓��͏����擾
bool CMouse::GetPress(MouseButton Button) const
{
	return (m_MouseState.rgbButtons[Button] & 0x80) ? true : false;
}

bool CMouse::GetTrigger(MouseButton Button) const
{
	return (m_MouseStateTrigger.rgbButtons[Button] & 0x80) ? true : false;

}

bool CMouse::GetRelease(MouseButton Button) const
{
	return (m_MouseStateRelease.rgbButtons[Button] & 0x80) ? true : false;
}

float CMouse::MouseX() const //�}�E�X��(�E���ƃv���X�A�����ƃ}�C�i�X)
{
	return m_MouseState.lX;
}

float CMouse::MouseY() const //�}�E�X�c(�ゾ�ƃ}�C�i�X�A�����ƃv���X)
{
	return m_MouseState.lY;
}

float CMouse::MouseZ() const //�}�E�X�̃z�C�[��(��ɓ]�����ƃv���X�A�����ƃ}�C�i�X)
{
	return m_MouseState.lZ;
}

bool CMouse::Mouse() const
{
	return m_bMouse;
}

//����̃{�^���������ꂽ��
bool CMouse::GetMouseButton(MouseButton Button) const
{
	if ((m_MouseStateTrigger.rgbButtons[Button] & 0x80) != 0 && (m_MouseStateRelease.rgbButtons[Button] & 0x80) == 0)
	{
		return true;
	}
	return false;
}
