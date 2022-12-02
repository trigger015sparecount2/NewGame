//=============================================================================
//
// フェード処理 [fade.h]
// Author : 三上航世
//
//=============================================================================
#include "fade.h"
#include "manager.h"
#include "renderer.h"
#include "scene2d.h"
#include "sound.h"

//静的メンバ変数
CManager::MODE CFade::m_mode = CManager::MODE_TITLE;
CFade::FADE CFade::m_fade = CFade::FADE_IN;

CFade::CFade(void)
{

}

CFade::~CFade(void)
{

}

HRESULT CFade::Init(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	// 頂点情報を設定
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4,
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&m_pVtxBuff,
		NULL);
	m_FadeColor = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	VERTEX_2D *pVtx;
	//頂点バッファをロックし、頂点データへのポインタを取得
	m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
	//頂点座標の設定
	pVtx[0].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pVtx[1].pos = D3DXVECTOR3(SCREEN_WIDTH, 0.0f, 0.0f);
	pVtx[2].pos = D3DXVECTOR3(0.0f, SCREEN_HEIGHT, 0.0f);
	pVtx[3].pos = D3DXVECTOR3(SCREEN_WIDTH, SCREEN_HEIGHT, 0.0f);
	//rhwの設定
	pVtx[0].rhw = 1.0f;
	pVtx[1].rhw = 1.0f;
	pVtx[2].rhw = 1.0f;
	pVtx[3].rhw = 1.0f;
	//頂点カラー
	pVtx[0].col = m_FadeColor;
	pVtx[1].col = m_FadeColor;
	pVtx[2].col = m_FadeColor;
	pVtx[3].col = m_FadeColor;
	//テクスチャ座標
	pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
	pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
	pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
	pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
	//頂点バッファをアンロックする
	m_pVtxBuff->Unlock();
	return S_OK;
}

void CFade::Uninit(void)
{
	//頂点バッファの破棄
	if (m_pVtxBuff != NULL)
	{
		m_pVtxBuff->Release();
		m_pVtxBuff = NULL;
	}
}

void CFade::Update(void)
{
	if (m_fade != FADE_NONE)
	{
		if (m_fade == FADE_IN)
		{
			m_FadeColor.a -= 0.05f;
			if (m_FadeColor.a <= 0.0f)
			{
				m_FadeColor.a = 0.0f;
				m_fade = FADE_NONE;
			}
		}
		else if (m_fade == FADE_OUT)
		{
			m_FadeColor.a += 0.05f;
			if (m_FadeColor.a >= 1.0f)
			{
				m_FadeColor.a = 1.0f;
				m_fade = FADE_IN;
				//モードの設定
				CSound::Stop();
				CManager::SetMode(m_mode);
			}
		}
		VERTEX_2D *pVtx; //頂点情報へのポインタ
		m_pVtxBuff->Lock(0, 0, (void **)&pVtx, 0);
		pVtx[0].col = m_FadeColor;
		pVtx[1].col = m_FadeColor;
		pVtx[2].col = m_FadeColor;
		pVtx[3].col = m_FadeColor;
		m_pVtxBuff->Unlock();
	}
}

void CFade::Draw(void)
{
	LPDIRECT3DDEVICE9 pDevice;
	pDevice = CManager::GetRenderer()->GetDevice();
	// 頂点フォーマットの設定
	pDevice->SetStreamSource(0, m_pVtxBuff, 0, sizeof(VERTEX_2D));
	pDevice->SetFVF(FVF_VERTEX_2D);  //頂点フォーマットの設定
	pDevice->SetTexture(0, NULL);    //テクスチャの設定

	pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP,
		0,  //開始する始点のインデックス
		2); //描画するプリミティブ数
}

void CFade::SetFade(CManager::MODE modenext)
{
	m_fade = FADE_OUT;
	m_mode = modenext;
}

CFade::FADE CFade::GetFade(void)
{
	return m_fade;
}