//---------------------------
//Author:�O��q��
//�t�B�[���h(field.cpp)
//---------------------------
#include "field.h"
#include "manager.h"
#include "renderer.h"

CField::CField(PRIORITY Priority) : CPlane::CPlane(Priority)
{

}

CField::~CField()
{

}

//����������
HRESULT CField::Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR3 rot, D3DXVECTOR2 Tex, bool bRotX)
{
	CPlane::Init(size, pos, Tex, bRotX);
	SetRot(rot);
	m_VtxMax = size;
	m_VtxMin = -size;
	m_bRotX = bRotX;
	return S_OK;
}

//�I������
void CField::Uninit()
{
	CPlane::Uninit();
}

//�X�V����
void CField::Update()
{
	D3DXVECTOR3 pos = GetPos();
	LPDIRECT3DDEVICE9 pDevice;   //�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxCamera;
	pDevice = CManager::GetRenderer()->GetDevice();		 //�f�o�C�X���擾����
	pDevice->GetTransform(D3DTS_VIEW, &mtxCamera);
	if (fabsf(mtxCamera._42) - fabsf(pos.y) < 3000.0f)
	{
		if (m_bDraw == false)
		{
			m_bDraw = true;
		}
	}
	else
	{
		if (m_bDraw == true)
		{
			m_bDraw = false;
		}
	}
}

//�`�揈��
void CField::Draw()
{
	if (m_bDraw == true)
	{
		CPlane::Draw();
	}
}

CField *CField::Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR3 rot, int nTex, D3DXVECTOR2 Tex, bool bRotX)
{
	CField *pField;
	pField = new CField(PRIORITY_PLANE);
	if (pField != NULL)
	{
		pField->Init(size, pos, rot, Tex, bRotX);
		pField->SetTexture(nTex);
	}
	return pField;
}