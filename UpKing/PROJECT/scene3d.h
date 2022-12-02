//=============================================================================
//
// シーン処理 [scene3d.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _SCENE3D_H_
#define _SCENE3D_H_

#include "main.h"
#include "scene.h"

struct VERTEX_3D
{
	D3DXVECTOR3 pos; //頂点座標
	D3DXVECTOR3 nor; //法線ベクトル
	D3DCOLOR col;    //頂点カラー
	D3DXVECTOR2 tex; //テクスチャ座標
};

class CScene3D : public CScene
{
public:
	CScene3D(PRIORITY Priority);
	~CScene3D();
	HRESULT Init(D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_NONE; }
	void SetPos(D3DXVECTOR3 pos) { m_pos = pos; }
	D3DXVECTOR3 GetPos() { return m_pos; }
	D3DXVECTOR3 GetMove() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	float GetRadius() { return 0.0f; }
	float GetHeight() { return 0.0f; }
	COLLISION GetCollision() { return COLLISION_SPHERE; }
	D3DXVECTOR3 GetVtxMax() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	D3DXVECTOR3 GetVtxMin() { return D3DXVECTOR3(0.0f, 0.0f, 0.0f); }
	void SetMatrix(D3DXMATRIX mtx) { m_mtxWorld = mtx; }
	D3DXVECTOR3 GetRot() { return m_rot; }
	void SetRot(D3DXVECTOR3 rot) { m_rot = rot; }
	bool GetRotX() { return false; }
	D3DXMATRIX GetMatrix(int) { return m_mtxWorld; }
	D3DXMATRIX GetMatrix() { return m_mtxWorld; }

	static CScene3D *Create(const D3DXVECTOR3& pos);

private:
	D3DXVECTOR3 m_rot; //向き
	D3DXVECTOR3 m_pos;
	D3DXMATRIX m_mtxWorld; //ワールドマトリックス
};
#endif
