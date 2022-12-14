//=============================================================================
//ゲーム管理[Game.cpp]
// Author : 三上航世
//=============================================================================
//*****************************************************************************
//インクルードファイル
//*****************************************************************************
#if 1
#include "game.h"
#include "pauseui.h"
#include "player.h"
#include "ui.h"
#include "keyboard.h"
#include "gamepad.h"
#include "bg.h"
#include "manager.h"
#include "renderer.h"
#include "fade.h"

#include "field.h"
#include "wall.h"
#include "light.h"
#include "object.h"
#include "load.h"

#include "sound.h"

#endif
//*****************************************************************************
//静的
//*****************************************************************************
bool CGame::m_bCountFlag = false;
int CGame::m_SelectNum = 1;

//*****************************************************************************
//マクロ
//*****************************************************************************
#define GAME_FILE "data/FILES/stage.txt"

#if 1
//*****************************************************************************
//コンストラクタ
//*****************************************************************************
CGame::CGame(PRIORITY Priority) : CScene::CScene(Priority)
{

}

//*****************************************************************************
// デストラクタ
//*****************************************************************************
CGame::~CGame()
{

}
#endif

//*****************************************************************************
// 初期化
//***************************************************************************** 
HRESULT CGame::Init(D3DXVECTOR3 /*pos*/)
{
	//UI作成
	//CUI::Create(D3DXVECTOR3(1200.0f, 620.0f, 0.0), 25.0f, 60.0f, 1, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));	//HP
	//CUI::Create(D3DXVECTOR3(1037.5f, 620.0f, 0.0), 300.0f, 60.0f, -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));	//HPゲージ下地
	//if (CManager::GetGamepad() != NULL)
	//{
	//	CUI::Create(D3DXVECTOR3(120.0f, 550.0f, 0.0), 200.0f, 300.0f, 4, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	//操作方法ゲームパッド
	//}
	//else
	//{
	//	CUI::Create(D3DXVECTOR3(120.0f, 550.0f, 0.0), 200.0f, 300.0f, 3, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));	//操作方法キーボード
	//}
	CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), SCREEN_WIDTH, SCREEN_HEIGHT, -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.7f));
	CPauseUI::Create(D3DXVECTOR3(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 0.0f), 280.0f, 400.0f, 0, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));

	CLoad::Load(GAME_FILE);
	//CSound::Play(1);
	CManager::SetCountdown(true);
	CManager::SetGameClear(false);
	CManager::SetGameEnd(false);
	CManager::SetEnd(false);
	return S_OK;
}

//*****************************************************************************
// 終了
//*****************************************************************************
void CGame::Uninit()
{
	Release();
}

//*****************************************************************************
// 更新
//***************************************************************************** 
void CGame::Update()
{
	
}

//*****************************************************************************
//描画
//***************************************************************************** 
void CGame::Draw()
{

}

//*****************************************************************************
//作成
//***************************************************************************** 
CGame *CGame::Create()
{
	CGame *pGame = NULL;
	pGame = new CGame(PRIORITY_ORBIT);		//メモリ確保
											//NULLチェック
	if (pGame != NULL)
	{
		pGame->Init(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	}

	return pGame;
}