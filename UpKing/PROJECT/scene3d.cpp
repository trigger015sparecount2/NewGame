//---------------------------
//Author:ŽOãq¢
//3DŠÖŒW(scene3d.cpp)
//---------------------------
#include "scene3d.h"
#include "manager.h"
#include "renderer.h"

CScene3D::CScene3D(PRIORITY Priority) : CScene::CScene(Priority)
{

}

CScene3D::~CScene3D()
{

}


//‰Šú‰»ˆ—
HRESULT CScene3D::Init(D3DXVECTOR3 /*pos*/)
{
	return S_OK;
}

//I—¹ˆ—
void CScene3D::Uninit()
{
	Release();
}

//XVˆ—
void CScene3D::Update()
{

}

//•`‰æˆ—
void CScene3D::Draw()
{

}

CScene3D *CScene3D::Create(const D3DXVECTOR3& pos)
{
	CScene3D *pScene3D;
	pScene3D = new CScene3D(PRIORITY_OBJECT);
	if (pScene3D != NULL)
	{
		pScene3D->Init(pos);
	}
	return pScene3D;
}