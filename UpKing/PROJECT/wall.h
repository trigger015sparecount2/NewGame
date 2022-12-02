//=============================================================================
//
// ï«èàóù [wall.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _WALL_H_
#define _WALL_H_

#include "main.h"
#include "plane.h"

class CWall : public CPlane
{
public:
	CWall(PRIORITY Priority);
	~CWall();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, D3DXVECTOR3 rot);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_WALL; }
	D3DXVECTOR3 GetVtxMax() { return m_VtxMax; }
	D3DXVECTOR3 GetVtxMin() { return m_VtxMin; }

	static CWall *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, int nTex, D3DXVECTOR2 Tex, D3DXVECTOR3 rot);

private:
	D3DXVECTOR3 m_VtxMax;
	D3DXVECTOR3 m_VtxMin;
};

#endif