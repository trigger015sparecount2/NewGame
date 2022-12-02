//=============================================================================
//
// âeèàóù [shadow.h]
// Author : éOè„çqê¢
//
//=============================================================================
#ifndef _SHADOW_H_
#define _SHADOW_H_

#include "main.h"
#include "plane.h"

class CShadow : public CPlane
{
public:
	CShadow(PRIORITY Priority);
	~CShadow();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos);
	void Uninit();
	void Update();
	void Draw();
	void Move(float fHeight);
	void MoveY(D3DXVECTOR3 pos, D3DXVECTOR3 rot);

	static CShadow *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos);

private:
	float m_fPosY;
	float m_fLength;
	D3DXVECTOR3 m_size;
};

#endif