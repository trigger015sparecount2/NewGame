//=============================================================================
//
// �V�[������ [scene.h]
// Author : �O��q��
//
//=============================================================================
#ifndef _SCENE_H_
#define _SCENE_H_
#include "main.h"

//�}�N����`
//#define POLYGON_SIZE (70.0f)
//#define PRIORITY (7)
//#define CIRCLE (float(rand() % 324) / 100.0f - float(rand() % 324) / 100.0f) //�~

class CScene
{
public:

	//�I�u�W�F�N�g�̎��
	typedef enum
	{
		OBJECTTYPE_NONE = 0,
		OBJECTTYPE_UI,
		OBJECTTYPE_PLAYER,
		OBJECTTYPE_ENEMY,
		OBJECTTYPE_FIELD,
		OBJECTTYPE_WALL,
		OBJECTTYPE_CAMERAPANEL,
		OBJECTTYPE_TUTORIAL,
		OBJECTTYPE_BLOCK,
		OBJECTTYPE_MAX
	} OBJTYPE;

	typedef enum
	{
		COLLISION_SPHERE = 0,
		COLLISION_PALL,
		COLLISION_SQUARE,
		COLLISION_MAX
	} COLLISION;

	typedef enum
	{
		PRIORITY_PLANE = 0,
		PRIORITY_OBJECT,
		PRIORITY_CHARA,
		PRIORITY_EFFECT,
		PRIORITY_ORBIT,
		PRIORITY_UIEFFECT,
		PRIORITY_UI,
		PRIORITY_PAUSEUI,
		PRIORITY_MAX
	} PRIORITY;

	CScene(PRIORITY Priority);
	virtual ~CScene();
	virtual HRESULT Init(D3DXVECTOR3 pos) = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;
	virtual OBJTYPE GetObjType() = 0;
	virtual D3DXVECTOR3 GetPos() = 0;
	virtual D3DXVECTOR3 GetRot() = 0;
	virtual D3DXVECTOR3 GetMove() = 0;
	virtual float GetRadius() = 0;
	virtual float GetHeight() = 0;
	virtual COLLISION GetCollision() = 0;
	virtual D3DXVECTOR3 GetVtxMax() = 0;
	virtual D3DXVECTOR3 GetVtxMin() = 0;
	virtual bool GetRotX() = 0;
	virtual D3DXMATRIX GetMatrix() = 0;
	virtual CScene *GetNext() { return m_pNext; }

	static CScene *GetScene(PRIORITY Priority) { return m_pTop[Priority]; }
	static void ReleaseAll();
	static void UpdateAll();
	static void DrawAll();

	void SetDeath(bool bDeath) { m_bDeath = bDeath; }
	bool GetDeath() { return m_bDeath; }

private:
	static CScene *m_pTop[PRIORITY_MAX]; //�擪�̃I�u�W�F�N�g�̃|�C���^
	static CScene *m_pCur[PRIORITY_MAX]; //����(��Ԍ��)�̃I�u�W�F�N�g�̃|�C���^
	CScene *m_pPrev;       //�O�̃I�u�W�F�N�g�̃|�C���^
	CScene *m_pNext;       //���̃I�u�W�F�N�g�̃|�C���^

	static int m_nNumAll;
	bool m_bDeath;

	PRIORITY m_Priority;//�D�揇��

protected:
	void Release();
};
#endif