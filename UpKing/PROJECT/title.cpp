//=============================================================================
// タイトル管理[Title.cpp]
// Author : 三上航世
//=============================================================================
//*****************************************************************************
//インクルード
//*****************************************************************************
#if 1
#include "Title.h"
#include "keyboard.h"
#include "UI.h"

#include "game.h"
#include "gamepad.h"
#include "mouse.h"
#include "fade.h"

//#include "player.h"
//#include "light.h"

#include "sound.h"
#endif

//*****************************************************************************
//静的
//*****************************************************************************
//int CTitle::m_SerectNam = 1;

//*****************************************************************************
//マクロ
//*****************************************************************************
//#define MAX_TITLESERECT (4)

#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CTitle::CTitle(PRIORITY Priority) : CScene(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CTitle::~CTitle()
{
}
#endif

//*****************************************************************************
// 初期化
//***************************************************************************** 
HRESULT CTitle::Init(D3DXVECTOR3 /*pos*/)
{
	m_pKeyboard = CManager::GetKeyboard();
	//m_pGamePad = CManager::GetGamepad();
	//m_pMouse = CManager::GetMouse();

	//m_SerectNam = 1;
	//SetSerectNum(m_SerectNam);
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, 7, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 130.0f, 0.0f), 540.0f, 130.0f, 6, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
	m_pUI = CUI::Create(D3DXVECTOR3(SCREEN_WIDTH * 0.5f, 560.0f, 0.0f), 660.0f, 70.0f, 5, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	m_fClear = 1.0f;
	m_bClear = true;
	m_bButton = false;
	CManager::SetPause(false);
	CSound::Play(0);
	m_size = D3DXVECTOR2(660.0f, 70.0f);
	return S_OK;
}

//*****************************************************************************
// 終了
//***************************************************************************** 
void CTitle::Uninit()
{
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard = NULL;
	}
	/*if (m_pMouse != NULL)
	{
		m_pMouse = NULL;
	}*/
	CScene::Release();
}

//*****************************************************************************
// kousinn
//***************************************************************************** 
void CTitle::Update()
{
	if (m_bButton == false)
	{
		/*if (m_pGamePad != NULL)
		{
			if (m_pGamePad->GetAnyButton() == true)
			{
				FadeIn();
			}
		}
		else */if (m_pKeyboard != NULL/* && m_pMouse != NULL*/)
		{
			if (m_pKeyboard->GetAnyKey() == true)
			{
				FadeIn();
			}
			/*if (m_pMouse->GetMouseButton(CMouse::DIM_L))
			{
				FadeIn();
			}*/
		}
	}
	else
	{
		m_size.x += 12.0f;
		m_size.y -= 5.0f;
		m_pUI->SetSize(m_size);
	}
	if (m_bClear == true)
	{
		m_fClear -= 0.05f;
		if (m_fClear <= 0.0f)
		{
			m_fClear = 0.0f;
			m_bClear = false;
		}
	}
	else
	{
		m_fClear += 0.05f;
		if (m_fClear >= 1.0f)
		{
			m_fClear = 1.0f;
			m_bClear = true;
		}
	}
	m_pUI->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fClear));
}

#if 1
//*****************************************************************************
//描画
//***************************************************************************** 
void CTitle::Draw()
{

}

//*****************************************************************************
//描画
//***************************************************************************** 
CTitle *CTitle::Create()
{
	CTitle *pTitle = NULL;
	pTitle = new CTitle(PRIORITY_EFFECT);		//メモリ確保
												//NULLチェック
	if (pTitle != NULL)
	{
		pTitle->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return pTitle;
}
#endif

void CTitle::FadeIn()
{
	CFade::SetFade(CManager::MODE_GAME);
	CSound::Play(7);
	m_bButton = true;
}

//
//#if 1
////*****************************************************************************
////選択番号取得
////***************************************************************************** 
//int CTitle::GetSerectNum()
//{
//	return m_SerectNam;
//}
//
////*****************************************************************************
////選択番号セット
////***************************************************************************** 
//void CTitle::SetSerectNum(int nNumSerect)
//{
//	m_SerectNam = nNumSerect;
//}
//#endif