//---------------------------
//Author:ŽOãq¢
//•Ç(wall.cpp)
//---------------------------
#include "wall.h"
#include "manager.h"
#include "renderer.h"

CWall::CWall(PRIORITY Priority) : CPlane::CPlane(Priority)
{

}

CWall::~CWall()
{

}

//‰Šú‰»ˆ—
HRESULT CWall::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, D3DXVECTOR3 rot)
{
	CPlane::Init(size, pos, Tex, false);
	//SetPos(pos);
	SetRot(rot);
	m_VtxMax = size;
	m_VtxMin = -size;
	return S_OK;
}

//I—¹ˆ—
void CWall::Uninit()
{
	CPlane::Uninit();
}

//XVˆ—
void CWall::Update()
{

}

//•`‰æˆ—
void CWall::Draw()
{
	CPlane::Draw();
}

CWall *CWall::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, int nTex, D3DXVECTOR2 Tex, D3DXVECTOR3 rot)
{
	CWall *pWall;
	pWall = new CWall(PRIORITY_PLANE);
	if (pWall != NULL)
	{
		pWall->Init(size, pos, Tex, rot);
		pWall->SetTexture(nTex);
	}
	return pWall;
}
