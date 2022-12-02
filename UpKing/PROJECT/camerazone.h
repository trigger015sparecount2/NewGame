//=============================================================================
//
// ÉJÉÅÉâÉ]Å[Éìèàóù [camerazone.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _CAMERAZONE_H_
#define _CAMERAZONE_H_

#include "main.h"
#include "plane.h"

class CCameraZone : public CPlane
{
public:
	CCameraZone(PRIORITY Priority);
	~CCameraZone();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_CAMERAPANEL; }
	D3DXVECTOR3 GetVtxMax() { return m_VtxMax; }
	D3DXVECTOR3 GetVtxMin() { return m_VtxMin; }

	static CCameraZone *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos);

private:
	D3DXVECTOR3 m_VtxMax;
	D3DXVECTOR3 m_VtxMin;
};

#endif