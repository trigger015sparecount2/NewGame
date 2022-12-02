//=============================================================================
// シーン2D処理 [Scene2D.cpp]
// Author : 三上航世
//=============================================================================
//=============================================================================
// インクルード
//=============================================================================
#include "Scene2D.h"
#include "manager.h"
#include "Renderer.h"
#include "keyboard.h"

//=============================================================================
// マクロ
//=============================================================================
#define FILENAMETEXT "data/FILES/TexNameRead.txt"	//読み込むスクリプトファイル名

//=============================================================================
// 静的メンバ変数宣言
//=============================================================================
LPDIRECT3DTEXTURE9 CScene2D::m_Texture[MAX_TEXTURE] = {};
int CScene2D::m_nMaxTex = 0;


//=============================================================================
// コンストラクタ
//=============================================================================
CScene2D::CScene2D(CScene::PRIORITY Priority) :CScene(Priority)
{
	m_Texture[MAX_TEXTURE] = {};
	m_pVtxBuff = NULL;

	m_Texpos = 0;
	m_TexSize = 0;
}

//=============================================================================
// デストラクタ
//=============================================================================
CScene2D::~CScene2D()
{

}

//=============================================================================
// ポリゴンの初期化
//=============================================================================
HRESULT CScene2D::Init(D3DXVECTOR3 pos)
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスの取得

																	 //頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4, D3DUSAGE_WRITEONLY, FVF_VERTEX_2D, D3DPOOL_MANAGED, &m_pVtxBuff, NULL);
	VERTEX_2D *pVtx;//頂点情報へのポインタ

					//メンバ変数に数値をセット
	m_pos = pos;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_Size.x / 2), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;

	//頂点の色
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);

	//テクスチャのUV座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();


	return S_OK;
}

//=============================================================================
// ポリゴンの終了
//=============================================================================
void CScene2D::Uninit()
{

	//頂点バッファも破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}

	//オブジェクト破棄
	Release();
}


//=============================================================================
// ポリゴンの更新
//=============================================================================
void CScene2D::Update()
{



}

//=============================================================================
// ポリゴンの描画
//=============================================================================
void CScene2D::Draw()
{
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスのポインタ

																	 //アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0);

	//頂点バッファをデバイスのデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));

	// 頂点フォーマットの設定
	pDevice->SetFVF(FVF_VERTEX_2D);//頂点フォーマットの設定

	if (m_nType != -1)
	{
		pDevice->SetTexture(0, m_Texture[m_nType]);//テクスチャの設定
	}
	else
	{
		pDevice->SetTexture(0, NULL);//テクスチャの設定
	}

	// ポリゴンの描画
	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, 0, 2);

	//アルファテスト関係
	pDevice->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	pDevice->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_ALWAYS);
	pDevice->SetRenderState(D3DRS_ALPHAREF, 0x00);

}

//=============================================================================
// テクスチャ破棄
//=============================================================================
void CScene2D::UninitTexture()
{
	//テクスチャ破棄
	for (int nCnt = 0; nCnt < m_nMaxTex; nCnt++)
	{
		if (m_Texture[nCnt] != NULL)
		{
			m_Texture[nCnt]->Release();
			m_Texture[nCnt] = NULL;
		}
	}

}
//=============================================================================
// テクスチャ情報セット(テクスチャパターンの更新)
//=============================================================================
void CScene2D::SetTex(int Texpos, float TexSize)
{
	m_Texpos = Texpos;
	m_TexSize = TexSize;

	//一枚絵と分けるためここで更新
	VERTEX_2D*pVtx;//頂点情報へのポインタ
				   //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャのUV座標
	pVtx[0].tex = D3DXVECTOR2(m_Texpos * m_TexSize, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(m_Texpos * m_TexSize + m_TexSize, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(m_Texpos * m_TexSize, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_Texpos * m_TexSize + m_TexSize, 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//=============================================================================
// テクスチャ情報セット(背景スクロール)
//=============================================================================
void CScene2D::TexMove(float TexMoveU, float TexMoveV)
{
	m_TexMoveU = TexMoveU;
	m_TexMoveV = TexMoveV;
	m_PosTexV += m_TexMoveV;

	if (m_PosTexV >= 1.0f)
	{
		m_PosTexV -= 1.0f;
	}
	else if (m_PosTexV < 0.0f)
	{
		m_PosTexV += 1.0f;
	}

	if (m_TexMoveU >= 1.0f)
	{
		m_TexMoveU -= 1.0f;
	}
	else if (m_TexMoveU < 0.0f)
	{
		m_TexMoveU += 1.0f;
	}

	//一枚絵と分けるためここで更新
	VERTEX_2D*pVtx;//頂点情報へのポインタ
				   //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//テクスチャのUV座標
	pVtx[0].tex = D3DXVECTOR2(m_TexMoveU, m_PosTexV);
	pVtx[1].tex = D3DXVECTOR2(m_TexMoveU + 1.0f, m_PosTexV);
	pVtx[2].tex = D3DXVECTOR2(m_TexMoveU, m_PosTexV + 1.0f);
	pVtx[3].tex = D3DXVECTOR2(m_TexMoveU + 1.0f, m_PosTexV + 1.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 横ゲージ型のサイズ変更
//=============================================================================
void CScene2D::SetBesideGauge(float fScale)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

	m_Size.x = fScale;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x				, m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Size.x	, m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x				, m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Size.x	, m_pos.y + (m_Size.y / 2), 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// 縦ゲージ型のサイズ変更
//=============================================================================
void CScene2D::SetVerticalGauge(float fScale)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

	m_Size.y = fScale;

	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - m_Size.x / 2, m_pos.y			, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + m_Size.x / 2, m_pos.y			, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - m_Size.x / 2, m_pos.y + m_Size.y, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + m_Size.x / 2, m_pos.y + m_Size.y, 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ポリゴン情報セット(カラー)
//=============================================================================
void CScene2D::ColorChange(D3DXCOLOR color)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の色
	pVtx[0].col = color;
	pVtx[1].col = color;
	pVtx[2].col = color;
	pVtx[3].col = color;

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ポリゴン情報セット(カラー)
//=============================================================================
void CScene2D::FadeColorChange(int Alpha)
{
	VERTEX_2D *pVtx;//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の色
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, Alpha);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, Alpha);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();

}

//セット・ゲット
#if 1
//=============================================================================
// ポリゴン情報セット
//=============================================================================
void CScene2D::SetSize(D3DXVECTOR2 Size)
{
	m_Size = Size;
	VERTEX_2D *pVtx;//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_Size.x / 2), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}

//=============================================================================
// ポリゴンテクスチャセット
//=============================================================================
void CScene2D::SetTexture(int nTex)
{
	m_nType = nTex;
}
#endif

//=============================================================================
// テクスチャ生成
//=============================================================================
void CScene2D::CreateTexture()
{
	// 頂点情報を設定
	LPDIRECT3DDEVICE9 pDevice = CManager::GetRenderer()->GetDevice();//デバイスの取得

																	 //ファイル読み込み
	char aFile[256];
	FILE *pFile = fopen(FILENAMETEXT, "r");

	int nCntTex = 0;

	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]);

			if (strcmp(&aFile[0], "NUM_TEXTURE") == 0)	//NUM_TEXTUREの文字列
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &m_nMaxTex);//使用するテクスチャ数を読み込む
			}

			if (strcmp(&aFile[0], "TEXTURE_FILENAME") == 0) //TEXTURE_FILENAMEの文字列
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				D3DXCreateTextureFromFile(pDevice, &aFile[0], &m_Texture[nCntTex]);
				nCntTex++;
			}

			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //END_SCRIPTの文字列を見つけたら
			{
				break;
			}

		}
		fclose(pFile);
	}
}

void CScene2D::SetPos(D3DXVECTOR3 pos)
{
	m_pos = pos;
	VERTEX_2D *pVtx;//頂点情報へのポインタ

					//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void**)&pVtx, 0);

	//頂点の座標
	pVtx[0].pos = D3DXVECTOR3(m_pos.x - (m_Size.x / 2), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[1].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y - (m_Size.y / 2), 0.0f);
	pVtx[2].pos = D3DXVECTOR3(m_pos.x - (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);
	pVtx[3].pos = D3DXVECTOR3(m_pos.x + (m_Size.x / 2), m_pos.y + (m_Size.y / 2), 0.0f);

	//頂点バッファをアンロック
	m_pVtxBuff->Unlock();
}