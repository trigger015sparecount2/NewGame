//=============================================================================
//
// マネージャ [Manager.h]
// Author : 三上航世
//
//=============================================================================
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"

#include "keyboard.h"
#include "mouse.h"
//#include "gamepad.h"
#include "plane.h"

#include "title.h"
#include "game.h"
#include "result.h"

//静的メンバ変数
CRenderer *CManager::m_pRenderer = NULL;
CKeyboard *CManager::m_pKeyboard = NULL;
CMouse *CManager::m_pMouse = NULL;
CGamePad *CManager::m_pGamepad = NULL;

CManager::MODE CManager::m_aMode = CManager::MODE_TITLE;

CTitle *CManager::m_pTitle = NULL;
CGame *CManager::m_pGame = NULL;
CResult *CManager::m_pResult = NULL;
//CTutorial *CManager::m_pTutorial = NULL;
//CGameOver *CManager::m_pGameOver = NULL;

int CManager::m_nTime = 0;
bool CManager::m_bGameEnd = false;
bool CManager::m_bGameClear = false;
bool CManager::m_bPause = false;
bool CManager::m_bCountdown = false;
bool CManager::m_bEnd = false;

//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CManager::CManager()
{

}

//*****************************************************************************
//デストラクタ
//*****************************************************************************
CManager::~CManager()
{

}

//*****************************************************************************
//初期化
//*****************************************************************************
HRESULT CManager::Init(HINSTANCE hInstance, HWND hWnd, bool bWindow)
{
	m_pKeyboard = new CKeyboard;
	m_pKeyboard->Init(hInstance, hWnd);
	m_pMouse = new CMouse;
	m_pMouse->Init(hInstance, hWnd);
	/*m_pGamepad = new CGamePad;
	m_pGamepad->Init(hInstance, hWnd);*/
	m_pRenderer = new CRenderer;
	m_pRenderer->Init(hWnd, bWindow);

	//テクスチャ読み込み	
	CScene2D::CreateTexture();
	CPlane::CreateTextureFiled();

	SetMode(m_aMode);

	return S_OK;
}

//*****************************************************************************
//終了
//*****************************************************************************
void CManager::Uninit()
{
	CScene2D::UninitTexture();
	CPlane::UninitTexture();

	CScene::ReleaseAll();
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Uninit();
		delete m_pKeyboard;
		m_pKeyboard = NULL;
	}
	if (m_pMouse != NULL)
	{
		m_pMouse->Uninit();
		delete m_pMouse;
		m_pMouse = NULL;
	}
	/*if (m_pGamepad != NULL)
	{
		m_pGamepad->Uninit();
		delete m_pGamepad;
		m_pGamepad = NULL;
	}*/
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Uninit();
		delete m_pRenderer;
		m_pRenderer = NULL;
	}
	if (m_pTitle != NULL)
	{
		m_pTitle = NULL;
	}
	/*if (m_pTutorial != NULL)
	{
	m_pTutorial = NULL;
	}*/
	if (m_pGame != NULL)
	{
		m_pGame = NULL;
	}
	/*if (m_pGameOver != NULL)
	{
	m_pGameOver = NULL;
	}*/
	if (m_pResult != NULL)
	{
		m_pResult = NULL;
	}
}

//*****************************************************************************
//更新
//*****************************************************************************
void CManager::Update()
{
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard->Update();
	}
	if (m_pMouse != NULL)
	{
		m_pMouse->Update();
	}
	/*if (m_pGamepad != NULL && CGamePad::GetGamePad() == true)
	{
		m_pGamepad->Update();
	}*/
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Update();
	}
}

//*****************************************************************************
//描画
//*****************************************************************************
void CManager::Draw()
{
	if (m_pRenderer != NULL)
	{
		m_pRenderer->Draw();
	}
}

//=============================================================================
//	レンダラー取得
//=============================================================================
CRenderer *CManager::GetRenderer()
{
	return m_pRenderer;
}

//=============================================================================
//	キーボード取得
//=============================================================================
CKeyboard *CManager::GetKeyboard()
{
	return m_pKeyboard;
}

//=============================================================================
//	ゲームパッド取得
//=============================================================================
CGamePad *CManager::GetGamepad()
{
	/*bool bGamepad = CGamePad::GetGamePad();

	if (bGamepad == true)
	{
		return m_pGamepad;
	}
	else
	{
		return NULL;
	}*/
	return NULL;
}

//=============================================================================
//	マウス取得
//=============================================================================
CMouse *CManager::GetMouse()
{
	return m_pMouse;
}

//=============================================================================
//モードセット
//=============================================================================
void CManager::SetMode(CManager::MODE mode)
{
	CScene::ReleaseAll();
	switch (m_aMode)
	{
	case MODE_TITLE:
		if (m_pTitle != NULL)
		{
			m_pTitle = NULL;
		}
		break;

	case MODE_GAME:
		if (m_pGame != NULL)
		{
			m_pGame = NULL;
		}
		break;

	case MODE_RESULT:
		if (m_pResult != NULL)
		{
			m_pResult = NULL;
		}
		break;

	default:
		break;
	}
	switch (mode)
	{
	case MODE_TITLE:
		m_pTitle = CTitle::Create();
		break;

	case MODE_GAME:
		m_pGame = CGame::Create();
		break;

	case MODE_RESULT:
		m_pResult = CResult::Create();
		break;

	default:
		break;
	}
	SetPause(false);
	m_aMode = mode;
}