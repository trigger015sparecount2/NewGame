//---------------------------
// Author:三上航世
// 空(sky.cpp)
//---------------------------
#include "bg.h"

#include "plane.h"
#include "manager.h"
#include "renderer.h"

//静的メンバ変数
LPDIRECT3DTEXTURE9 CBg::m_pTexture = NULL;

CBg::CBg(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CBg::~CBg(void)
{

}

//初期化処理
HRESULT CBg::Init(D3DXVECTOR3 pos)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	//頂点バッファの生成
	pDevice->CreateVertexBuffer(sizeof(VERTEX_3D) * 15,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_3D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = pos;
	VERTEX_3D *pVtx; //頂点情報へのポインタ
					 //頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 50000.0f, 0.0f); //てっぺん
	pVtx[1].pos = D3DXVECTOR3(-12000.0f, 46000.0f, 8000.0f);
	pVtx[2].pos = D3DXVECTOR3(-12000.0f, 46000.0f, -8000.0f);
	pVtx[3].pos = D3DXVECTOR3(0.0f, 46000.0f, -14000.0f);
	pVtx[4].pos = D3DXVECTOR3(12000.0f, 46000.0f, -8000.0f);
	pVtx[5].pos = D3DXVECTOR3(12000.0f, 46000.0f, 8000.0f);
	pVtx[6].pos = D3DXVECTOR3(0.0f, 46000.0f, 14000.0f);
	pVtx[7].pos = D3DXVECTOR3(-12000.0f, 46000.0f, 8000.0f);//上
	pVtx[8].pos = D3DXVECTOR3(-12000.0f, -5000.0f, 8000.0f);
	pVtx[9].pos = D3DXVECTOR3(-12000.0f, -5000.0f, -8000.0f);
	pVtx[10].pos = D3DXVECTOR3(0.0f, -5000.0f, -14000.0f);
	pVtx[11].pos = D3DXVECTOR3(12000.0f, -5000.0f, -8000.0f);
	pVtx[12].pos = D3DXVECTOR3(12000.0f, -5000.0f, 8000.0f);
	pVtx[13].pos = D3DXVECTOR3(0.0f, -5000.0f, 14000.0f);
	pVtx[14].pos = D3DXVECTOR3(-12000.0f, -5000.0f, 8000.0f);//真ん中
															 //法線ベクトルの設定
	pVtx[0].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[1].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[2].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[3].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[4].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[5].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[6].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[7].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[8].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[9].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[10].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[11].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[12].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[13].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	pVtx[14].nor = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	//頂点カラー
	pVtx[0].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[1].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[2].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[3].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[4].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[5].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[6].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[7].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[8].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[9].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[10].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[11].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[12].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[13].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	pVtx[14].col = D3DCOLOR_RGBA(255, 255, 255, 255);
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(3.0f / 6.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(0.0f / 6.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(1.0f / 6.0f, 0.0f);
	pVtx[3].tex = D3DXVECTOR2(2.0f / 6.0f, 0.0f);
	pVtx[4].tex = D3DXVECTOR2(3.0f / 6.0f, 0.0f);
	pVtx[5].tex = D3DXVECTOR2(4.0f / 6.0f, 0.0f);
	pVtx[6].tex = D3DXVECTOR2(5.0f / 6.0f, 0.0f);
	pVtx[7].tex = D3DXVECTOR2(6.0f / 6.0f, 0.0f);
	pVtx[8].tex = D3DXVECTOR2(0.0f / 6.0f, 1.0f);
	pVtx[9].tex = D3DXVECTOR2(1.0f / 6.0f, 1.0f);
	pVtx[10].tex = D3DXVECTOR2(2.0f / 6.0f, 1.0f);
	pVtx[11].tex = D3DXVECTOR2(3.0f / 6.0f, 1.0f);
	pVtx[12].tex = D3DXVECTOR2(4.0f / 6.0f, 1.0f);
	pVtx[13].tex = D3DXVECTOR2(5.0f / 6.0f, 1.0f);
	pVtx[14].tex = D3DXVECTOR2(6.0f / 6.0f, 1.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	//インデックスバッファの生成
	pDevice->CreateIndexBuffer(sizeof(WORD) * 38,
		D3DUSAGE_WRITEONLY,
		D3DFMT_INDEX16,
		D3DPOOL_MANAGED,
		&m_pIdxBuff,
		NULL);
	WORD *pIdx; //インデックス情報へのポインタ
				//インデックスバッファをロックし、番号データへのポインタを取得
	m_pIdxBuff->Lock(0, 0, (void **)&pIdx, 0);
	pIdx[0] = 0;
	pIdx[1] = 1;
	pIdx[2] = 2;
	pIdx[3] = 0;
	pIdx[4] = 0;
	pIdx[5] = 2;
	pIdx[6] = 3;
	pIdx[7] = 0;
	pIdx[8] = 0;
	pIdx[9] = 3;
	pIdx[10] = 4;
	pIdx[11] = 0;
	pIdx[12] = 0;
	pIdx[13] = 4;
	pIdx[14] = 5;
	pIdx[15] = 0;
	pIdx[16] = 0;
	pIdx[17] = 5;
	pIdx[18] = 6;
	pIdx[19] = 0;
	pIdx[20] = 0;
	pIdx[21] = 6;
	pIdx[22] = 7;//上
	pIdx[23] = 1;
	pIdx[24] = 1;
	pIdx[25] = 8;
	pIdx[26] = 2;
	pIdx[27] = 9;
	pIdx[28] = 3;
	pIdx[29] = 10;
	pIdx[30] = 4;
	pIdx[31] = 11;
	pIdx[32] = 5;
	pIdx[33] = 12;
	pIdx[34] = 6;
	pIdx[35] = 13;
	pIdx[36] = 7;
	pIdx[37] = 14;//下
				  //インデックスバッファをアンロックする
	m_pIdxBuff->Unlock();
	return S_OK;
}

//終了処理
void CBg::Uninit(void)
{
	if (m_pTexture != NULL)
	{
		m_pTexture = NULL;
	}
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
	//頂点バッファの破棄
	if (m_pIdxBuff != NULL)
	{
		m_pIdxBuff->Release();
		m_pIdxBuff = NULL;
	}
	Release();
}

//更新処理
void CBg::Update(void)
{
	if (CManager::GetPause() == false && CManager::GetEnd() == false)
	{
		int nCntTex;
		VERTEX_3D *pVtx; //頂点情報へのポインタ
						 //頂点バッファをロックし、頂点データへのポインタを取得
		m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
		for (nCntTex = 0; nCntTex < 15; nCntTex++)
		{
			pVtx[nCntTex].tex.x += m_fScroll;
		}
		//頂点バッファをアンロックする
		m_pVtxBuff->Unlock();
	}
}

//描画処理
void CBg::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	//ラインティングを無視する
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y, m_rot.x, m_rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x, m_pos.y, m_pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//頂点バッファをデータストリームに設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_3D));
	//インデックスバッファをデータストリームに設定
	pDevice->SetIndices(m_pIdxBuff);
	//頂点フォーマット
	pDevice->SetFVF(FVF_VERTEX_3D);
	if (m_pTexture == NULL)
	{
		m_pTexture = *CPlane::GetTexture(2);
	}
	pDevice->SetTexture(0, m_pTexture);    //テクスチャの設定
	pDevice->DrawIndexedPrimitive(D3DPT_TRIANGLESTRIP,
		0,
		0,
		15, //頂点の数
		0,
		36); //描画するプリミティブ数
	//ラインティングを有効にする
	pDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
}

CBg *CBg::Create(D3DXVECTOR3 pos, float fScroll, LPDIRECT3DTEXTURE9 *Texture)
{
	CBg *pBg;
	pBg = new CBg(PRIORITY_PLANE);
	if (pBg != NULL)
	{
		pBg->Init(pos);
		pBg->SetScroll(fScroll);
		pBg->SetTexture(Texture);
	}
	return pBg;
}

void CBg::SetScroll(float fScroll)
{
	m_fScroll = fScroll;
}

void CBg::SetTexture(LPDIRECT3DTEXTURE9 *Texture)
{
	m_pTexture = *Texture;
}