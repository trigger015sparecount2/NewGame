//=============================================================================
//
// オブジェクト処理 [object.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _OBJECT_H_
#define _OBJECT_H_

#include "main.h"
#include "model.h"
#include "scene3d.h"

class CObject : public CScene3D
{
public:
	CObject(PRIORITY Priority);
	~CObject();
	HRESULT Init(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_BLOCK; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	//D3DXVECTOR3 GetMove() { return m_move; }
	float GetRadius() { return m_fRadius; }
	COLLISION GetCollision() { return m_Collision; }
	D3DXVECTOR3 GetVtxMax() { return m_pModel->GetMaxSize(); }
	D3DXVECTOR3 GetVtxMin() { return m_pModel->GetMinSize(); }
	D3DXMATRIX GetMatrix() { return m_pModel->GetMatrix(); }

	static CObject *Create(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot);
	static void Load(int nCnt, const char *aModelName);
	static void UnLoad();

private:
	CModel *m_pModel;
	D3DXVECTOR3 m_pos;
	CScene::COLLISION m_Collision;
	float m_fRadius;
	bool m_bDraw;

	static CModel *m_paModel[8];
};

#endif