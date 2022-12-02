//---------------------------
//Author:三上航世
//モデル(model.cpp)
//---------------------------
#include "model.h"
#include "manager.h"
#include "renderer.h"

CModel::CModel()
{

}

CModel::~CModel()
{

}

//初期化処理
HRESULT CModel::Init(const char *aModelName)
{
	LPDIRECT3DDEVICE9 pDevice; //デバイスのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();     //デバイスを取得する
	D3DXLoadMeshFromX(aModelName,
		D3DXMESH_SYSTEMMEM,
		pDevice,
		NULL,
		&m_pBuffMat,
		NULL,
		&m_nNumMat,
		&m_pMesh);
	D3DXMATERIAL *pMat;
	//マテリアル情報に対するポインタを取得
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	int nCntTex = 0;
	for (int nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			D3DXCreateTextureFromFile(pDevice,
				pMat[nCntMat].pTextureFilename,
				&m_aTextureModel[nCntTex]);
			nCntTex++;
		}
	}
	m_MinSize = SearchMinSize();
	m_MaxSize = SearchMaxSize();
	m_rotOri = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_posOri = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_pParent = NULL;
	m_nIdxModelParent = -1;
	return S_OK;
}

//終了処理
void CModel::Uninit()
{
	//テクスチャの破棄
	if (m_pMesh != NULL)
	{
		m_pMesh->Release();
		m_pMesh = NULL;
	}
	//頂点バッファの破棄
	if (m_pBuffMat != NULL)
	{
		m_pBuffMat->Release();
		m_pBuffMat = NULL;
	}
}

//更新処理
void CModel::Update()
{

}

//描画処理
void CModel::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;   //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans, mtxParent; //計算用マトリックス
	D3DMATERIAL9 matDef;		 //現在のマテリアル保存用
	D3DXMATERIAL *pMat;			 //マテリアルデータへのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();		 //デバイスを取得する
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, m_rot.y + m_rotOri.y, m_rot.x + m_rotOri.x, m_rot.z + m_rotOri.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, m_pos.x + m_posOri.x, m_pos.y + m_posOri.y, m_pos.z + m_posOri.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	if (m_pParent != NULL)
	{
		//親のマトリックス取得
		mtxParent = m_pParent->GetMatrix();
	}
	else
	{
		//直前にセットされたマトリックス取得
		pDevice->GetTransform(D3DTS_WORLD, &mtxParent);
	}
	//算出した各パーツのワールドマトリックスと親のマトリックスを掛け合わせる
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxParent);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//パーツの現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	//パーツのマテリアルデータへのポインタの反映
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	int nCntMat;
	for (nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		int nCntTex = 0;
		//プレイヤーのマテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, m_aTextureModel[nCntTex]);
			nCntTex++;
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}
		//プレイヤーのモデル（パーツ）の描画
		m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

void CModel::DrawObject(D3DXVECTOR3 pos, D3DXVECTOR3 rot)
{
	LPDIRECT3DDEVICE9 pDevice;   //デバイスのポインタ
	D3DXMATRIX mtxRot, mtxTrans; //計算用マトリックス
	D3DMATERIAL9 matDef;		 //現在のマテリアル保存用
	D3DXMATERIAL *pMat;			 //マテリアルデータへのポインタ
	pDevice = CManager::GetRenderer()->GetDevice();		//デバイスを取得する
	//ワールドマトリックスの初期化
	D3DXMatrixIdentity(&m_mtxWorld);
	//向きを反映
	D3DXMatrixRotationYawPitchRoll(&mtxRot, rot.y, rot.x, rot.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxRot);
	//位置を反映
	D3DXMatrixTranslation(&mtxTrans, pos.x, pos.y, pos.z);
	D3DXMatrixMultiply(&m_mtxWorld, &m_mtxWorld, &mtxTrans);
	//ワールドマトリックスの設定
	pDevice->SetTransform(D3DTS_WORLD, &m_mtxWorld);
	//パーツの現在のマテリアルを取得
	pDevice->GetMaterial(&matDef);
	//パーツのマテリアルデータへのポインタの反映
	pMat = (D3DXMATERIAL*)m_pBuffMat->GetBufferPointer();
	int nCntMat;
	for (nCntMat = 0; nCntMat < (int)m_nNumMat; nCntMat++)
	{
		int nCntTex = 0;
		//オブジェクトのマテリアルの設定
		pDevice->SetMaterial(&pMat[nCntMat].MatD3D);
		if (pMat[nCntMat].pTextureFilename != NULL)
		{
			//テクスチャの設定
			pDevice->SetTexture(0, m_aTextureModel[nCntTex]);
			nCntTex++;
		}
		else
		{
			pDevice->SetTexture(0, NULL);
		}
		//オブジェクトの描画
		m_pMesh->DrawSubset(nCntMat);
	}
	//保存していたマテリアルを戻す
	pDevice->SetMaterial(&matDef);
}

CModel *CModel::Create(const char *aModelName)
{
	CModel *pModel;
	pModel = new CModel;
	if (pModel != NULL)
	{
		pModel->Init(aModelName);
	}
	return pModel;
}

D3DXVECTOR3 CModel::SearchMinSize()
{
	DWORD sizeFVF; //頂点フォーマットのサイズ
	BYTE *pVtxBuff; //頂点バッファへのポインタ
	D3DXVECTOR3 vtxMinModel = D3DXVECTOR3(10000.0f, 10000.0f, 10000.0f);
	//頂点数を取得
	int nNumVtx = m_pMesh->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());
	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標の代入
		if (vtxMinModel.x > vtx.x)
		{
			vtxMinModel.x = vtx.x;
		}
		if (vtxMinModel.y > vtx.y)
		{
			vtxMinModel.y = vtx.y;
		}
		if (vtxMinModel.z > vtx.z)
		{
			vtxMinModel.z = vtx.z;
		}
		pVtxBuff += sizeFVF; //頂点フォーマットのサイズ分ポインタを進める
	}
	//頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();
	return vtxMinModel;
}

D3DXVECTOR3 CModel::SearchMaxSize()
{
	DWORD sizeFVF; //頂点フォーマットのサイズ
	BYTE *pVtxBuff; //頂点バッファへのポインタ
	D3DXVECTOR3 vtxMaxModel = D3DXVECTOR3(-10000.0f, -10000.0f, -10000.0f);
	//頂点数を取得
	int nNumVtx = m_pMesh->GetNumVertices();
	//頂点フォーマットのサイズを取得
	sizeFVF = D3DXGetFVFVertexSize(m_pMesh->GetFVF());
	//頂点バッファのロック
	m_pMesh->LockVertexBuffer(D3DLOCK_READONLY, (void**)&pVtxBuff);
	for (int nCntVtx = 0; nCntVtx < nNumVtx; nCntVtx++)
	{
		D3DXVECTOR3 vtx = *(D3DXVECTOR3*)pVtxBuff; //頂点座標の代入
		if (vtxMaxModel.x < vtx.x)
		{
			vtxMaxModel.x = vtx.x;
		}
		if (vtxMaxModel.y < vtx.y)
		{
			vtxMaxModel.y = vtx.y;
		}
		if (vtxMaxModel.z < vtx.z)
		{
			vtxMaxModel.z = vtx.z;
		}
		pVtxBuff += sizeFVF; //頂点フォーマットのサイズ分ポインタを進める
	}
	//頂点バッファをアンロック
	m_pMesh->UnlockVertexBuffer();
	return vtxMaxModel;
}

void CModel::Copy(CModel *pModel)
{
	m_pMesh = pModel->m_pMesh;
	m_pBuffMat = pModel->m_pBuffMat;
	m_nNumMat = pModel->m_nNumMat;
	m_pParent = NULL;
	m_posOri = pModel->m_posOri;
	m_rotOri = pModel->m_rotOri;
	m_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nCntTex;
	for (nCntTex = 0; nCntTex < 4; nCntTex++)
	{
		if (pModel->m_aTextureModel[nCntTex] != NULL)
		{
			m_aTextureModel[nCntTex] = pModel->m_aTextureModel[nCntTex];
		}
	}
	m_MinSize = pModel->m_MinSize;
	m_MaxSize = pModel->m_MaxSize;
}

void CModel::SetRot(D3DXVECTOR3 rot)
{
	m_rot = rot;
	if (m_rot.x < -D3DX_PI)
	{
		m_rot.x += D3DX_PI * 2.0f;
	}
	else if (m_rot.x > D3DX_PI)
	{
		m_rot.x -= D3DX_PI * 2.0f;
	}
	if (m_rot.y < -D3DX_PI)
	{
		m_rot.y += D3DX_PI * 2.0f;
	}
	else if (m_rot.y > D3DX_PI)
	{
		m_rot.y -= D3DX_PI * 2.0f;
	}
	if (m_rot.z < -D3DX_PI)
	{
		m_rot.z += D3DX_PI * 2.0f;
	}
	else if (m_rot.z > D3DX_PI)
	{
		m_rot.z -= D3DX_PI * 2.0f;
	}
}