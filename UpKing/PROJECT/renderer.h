//=============================================================================
//
// �����_�����O���� [renderer.h]
// Author : �O��q��
//
//=============================================================================
#ifndef _RENDERER_H_
#define _RENDERER_H_
#include "main.h"

class CCamera;
class CFade;

class CRenderer
{
public:
	CRenderer();
	~CRenderer();

	HRESULT Init(HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();
	LPDIRECT3DDEVICE9 GetDevice() { return m_pD3DDevice; }
	CCamera *GetCamera() { return m_pCamera; }

#ifdef _DEBUG
	void DrawFPS();
#endif

private:
	LPDIRECT3D9				m_pD3D = NULL;			// Direct3D�I�u�W�F�N�g
	LPDIRECT3DDEVICE9		m_pD3DDevice = NULL;	// Device�I�u�W�F�N�g(�`��ɕK�v)
	LPD3DXFONT				m_pFont = NULL;			// �t�H���g�ւ̃|�C���^
	static CCamera *m_pCamera;
	static CFade *m_pFade;
};

#endif