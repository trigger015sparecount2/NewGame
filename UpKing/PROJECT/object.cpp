//---------------------------
//Author:�O��q��
//�I�u�W�F�N�g(object.cpp)
//---------------------------
#include "object.h"
#include "manager.h"
#include "renderer.h"
#include "model.h"

//�ÓI�����o�ϐ�
CModel *CObject::m_paModel[8] = {};

CObject::CObject(PRIORITY Priority) : CScene3D(Priority)
{

}

CObject::~CObject()
{

}

//����������
HRESULT CObject::Init(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot)
{
	D3DXVECTOR3 VtxMax, VtxMin;
	m_pModel = new CModel;
	m_pModel->Copy(m_paModel[nType]);
	m_Collision = collision;
	VtxMax = m_pModel->GetMaxSize();
	VtxMin = m_pModel->GetMinSize();
	float fRadius = (VtxMax.x - VtxMin.x) / 2;
	if (fRadius < (VtxMax.y - VtxMin.y) / 2)
	{
		fRadius = (VtxMax.y - VtxMin.y) / 2;
	}
	if (fRadius < (VtxMax.z - VtxMin.z) / 2)
	{
		fRadius = (VtxMax.z - VtxMin.z) / 2;
	}

	SetRot(rot);
	m_pos = pos;
	return S_OK;
}

//�I������
void CObject::Uninit()
{
	if (m_pModel != NULL)
	{
		m_pModel = NULL;
	}
	CScene3D::Uninit();
}

//�X�V����
void CObject::Update()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 posOld = pos;
	D3DXVECTOR3 rot = GetRot();
	LPDIRECT3DDEVICE9 pDevice;   //�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxCamera;
	pDevice = CManager::GetRenderer()->GetDevice();		 //�f�o�C�X���擾����
	pDevice->GetTransform(D3DTS_VIEW, &mtxCamera);
	if (fabsf(mtxCamera._42) - fabsf(pos.y) < 2000.0f)
	{
		if (m_bDraw == false)
		{
			m_bDraw = true;
		}
	}
	else if(m_bDraw == true)
	{
		m_bDraw = false;
	}
}

//�`�揈��
void CObject::Draw()
{
	if (m_bDraw == true)
	{
		m_pModel->DrawObject(GetPos(), GetRot());
	}
}

CObject *CObject::Create(D3DXVECTOR3 pos, int nType, COLLISION collision, D3DXVECTOR3 rot)
{
	CObject *pObject;
	pObject = new CObject(PRIORITY_OBJECT);
	if (pObject != NULL)
	{
		pObject->Init(pos, nType, collision, rot);
	}
	return pObject;
}

void CObject::Load(int nCnt, const char *aModelName)
{
	m_paModel[nCnt] = CModel::Create(aModelName);
}

void CObject::UnLoad()
{
	int nCntModel;
	for (nCntModel = 0; nCntModel < 8; nCntModel++)
	{
		if (m_paModel[nCntModel] != NULL)
		{
			m_paModel[nCntModel]->Uninit();
			m_paModel[nCntModel] = NULL;
		}
	}
}