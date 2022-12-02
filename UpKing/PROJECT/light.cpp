//---------------------------
//Author:三上航世
//ライト(light.cpp)
//---------------------------
#include "light.h"
#include "manager.h"
#include "renderer.h"
#include "camera.h"

CLight::CLight()
{

}

CLight::~CLight()
{

}

//初期化処理
HRESULT CLight::Init(D3DXCOLOR col, D3DXVECTOR3 vec, int nCntLight)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	D3DXVECTOR3 vecDir; //設定用方向ベクトル
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
														//ライトをクリアする
	ZeroMemory(&m_light[nCntLight], sizeof(D3DLIGHT9));
	//ライトの種類を設定
	m_light[nCntLight].Type = D3DLIGHT_DIRECTIONAL; //偏向光源
													//ライトの拡散先（色）を設定
	m_light[nCntLight].Diffuse = col;
	//ライトの方向を設定
	vecDir = vec;
	D3DXVec3Normalize(&vecDir, &vecDir); //正規化する
	m_light[nCntLight].Direction = vecDir;
	//ライトを設定する
	pDevice->SetLight(nCntLight, &m_light[nCntLight]);
	//ライトを有効にする
	pDevice->LightEnable(nCntLight, TRUE);
	return S_OK;
}

void CLight::Uninit()
{

}

void CLight::Update()
{

}

CLight *CLight::Create(D3DXCOLOR col, D3DXVECTOR3 vec, int nCntLight)
{
	CLight *pLight;
	pLight = new CLight;
	if (pLight != NULL)
	{
		pLight->Init(col, vec, nCntLight);
	}
	return pLight;
}