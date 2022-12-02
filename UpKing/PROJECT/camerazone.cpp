//---------------------------
//Author:�O��q��
//�J�����]�[��(camerazone.cpp)
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

//����������
HRESULT CCameraZone::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos)
{
	CPlane::Init(size, pos, D3DXVECTOR2(0.0f, 0.0f), false);
	m_VtxMax = size;
	m_VtxMin = -size;
	return S_OK;
}

//�I������
void CCameraZone::Uninit()
{
	CPlane::Uninit();
}

//�X�V����
void CCameraZone::Update()
{
	
}

//�`�揈��
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