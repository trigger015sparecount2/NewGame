//=============================================================================
//
// �V�[������ [field.h]
// Author : �O��q��
//
//=============================================================================
#ifndef _PLANE_H_
#define _PLANE_H_

#include "main.h"
#include "scene3d.h"

//�}�N����`
#define MAX_TEXTURE_FILED (16)

class CPlane : public CScene3D
{
public:
	CPlane(PRIORITY Priority);
	~CPlane();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, bool bRotX);
	void Uninit();
	void Update();
	void Draw();

	D3DXVECTOR3 GetSize() { return m_size; }
	void SetSize(D3DXVECTOR3 size);
	LPDIRECT3DVERTEXBUFFER9 GetVtx() { return m_pVtxBuff; }
	void SetTexture(int nTex);		//�w�肳�ꂽ�ԍ��̃e�N�X�`����\���悤��
	void ChangeColor(D3DXCOLOR col);
	//�G�t�F�N�g�p�֐�
	void ColorChange(D3DXCOLOR color);	//�F�ύX
	void ChangeSize(D3DXVECTOR3 size);	//�T�C�Y�ύX
	static CPlane *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, bool bRotX);
	static void CreateTextureFiled();

	static LPDIRECT3DTEXTURE9 *GetTexture(int nTex) { return &m_pTexture[nTex]; }

	//�e�N�X�`���j��
	static void UninitTexture();

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL; //���_�o�b�t�@�ւ̃|�C���^
	D3DXVECTOR3 m_size;
	static int m_nMaxTex;					   //�g�p����ő�e�N�X�`��

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE_FILED];	//�e�N�X�`���ւ̃|�C���^
	int m_nTexType;												//�\��e�N�X�`��
};

#endif