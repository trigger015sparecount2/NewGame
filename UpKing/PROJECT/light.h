//=============================================================================
//
// ÉVÅ[Éìèàóù [light.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "main.h"

class CLight
{
public:
	CLight();
	~CLight();
	HRESULT Init(D3DXCOLOR col, D3DXVECTOR3 vec, int nCntLight);
	void Uninit();
	void Update();

	static CLight *Create(D3DXCOLOR col, D3DXVECTOR3 vec, int nCntLight);

private:
	D3DLIGHT9 m_light[8];
};

#endif