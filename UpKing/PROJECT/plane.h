//=============================================================================
//
// シーン処理 [field.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _PLANE_H_
#define _PLANE_H_

#include "main.h"
#include "scene3d.h"

//マクロ定義
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
	void SetTexture(int nTex);		//指定された番号のテクスチャを貼れるように
	void ChangeColor(D3DXCOLOR col);
	//エフェクト用関数
	void ColorChange(D3DXCOLOR color);	//色変更
	void ChangeSize(D3DXVECTOR3 size);	//サイズ変更
	static CPlane *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, D3DXVECTOR2 Tex, bool bRotX);
	static void CreateTextureFiled();

	static LPDIRECT3DTEXTURE9 *GetTexture(int nTex) { return &m_pTexture[nTex]; }

	//テクスチャ破棄
	static void UninitTexture();

private:
	LPDIRECT3DVERTEXBUFFER9 m_pVtxBuff = NULL; //頂点バッファへのポインタ
	D3DXVECTOR3 m_size;
	static int m_nMaxTex;					   //使用する最大テクスチャ

protected:
	static LPDIRECT3DTEXTURE9 m_pTexture[MAX_TEXTURE_FILED];	//テクスチャへのポインタ
	int m_nTexType;												//貼るテクスチャ
};

#endif