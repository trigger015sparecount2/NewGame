//---------------------------
//Author:ŽOãq¢
//ƒJƒƒ‰ƒ][ƒ“(camerazone.cpp)
//---------------------------
#include "camerazone.h"
#include "manager.h"
#include "renderer.h"

CCameraZone::CCameraZone(PRIORITY Priority) : CPlane::CPlane(Priority)
{

}

CCameraZone::~CCameraZone()
{

}

//‰Šú‰»ˆ—
HRESULT CCameraZone::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CPlane::Init(size, pos, D3DXVECTOR2(0.0f, 0.0f), false);
	m_VtxMax = size;
	m_VtxMin = -size;
	return S_OK;
}

//I—¹ˆ—
void CCameraZone::Uninit()
{
	CPlane::Uninit();
}

//XVˆ—
void CCameraZone::Update()
{
	
}

//•`‰æˆ—
void CCameraZone::Draw()
{
	
}

CCameraZone *CCameraZone::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CCameraZone *pCameraZone;
	pCameraZone = new CCameraZone(PRIORITY_PLANE);
	if (pCameraZone != NULL)
	{
		pCameraZone->Init(size, pos);
	}
	return pCameraZone;
}