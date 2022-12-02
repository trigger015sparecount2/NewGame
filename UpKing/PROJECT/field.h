//=============================================================================
//
// ínñ èàóù [field.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _FIELD_H_
#define _FIELD_H_

#include "main.h"
#include "plane.h"

class CField : public CPlane
{
public:
	CField(PRIORITY Priority);
	~CField();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 Tex, bool bRotX);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_FIELD; }
	D3DXVECTOR3 GetVtxMax() { return m_VtxMax; }
	D3DXVECTOR3 GetVtxMin() { return m_VtxMin; }
	bool GetRotX() { return m_bRotX; }

	static CField *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTex, D3DXVECTOR2 Tex, bool bRotX);

private:
	D3DXVECTOR3 m_VtxMax;
	D3DXVECTOR3 m_VtxMin;
	bool m_bRotX;
	bool m_bDraw;
};

#endif