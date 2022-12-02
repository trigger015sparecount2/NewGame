//=============================================================================
//
// �V�[������ [player.h]
// Author : �O��q��
//
//=============================================================================
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "main.h"
#include "scene3d.h"
#include "model.h"

class CKeyboard;
//class CGamePad;
class CMouse;
class CCamera;
class CShadow;
class COrbit;
class CRockon;
class CUI;
class CPauseUI;

class CPlayer : public CScene3D
{
	//���[�V�����̎��
	typedef enum
	{
		MOTIONTYPE_NEUTRAL = 0,
		//MOTIONTYPE_AIRNUETRAL,
		MOTIONTYPE_RUN,
		MOTIONTYPE_CHARGE,
		MOTIONTYPE_FRONTJUMP,
		MOTIONTYPE_BACKJUMP,
		MOTIONTYPE_LANDING,
		MOTIONTYPE_RETURN, //�j���[�g�������[�V�����ɖ߂�ۂ̃��[�V�����Btxt�t�@�C���ɏ����K�v�͂Ȃ�
		MAX_MOTION
	} MOTIONTYPE;

	//�L�[�̍\����
	typedef struct
	{
		D3DXVECTOR3 pos;
		D3DXVECTOR3 rot;
	} KEY;

	//�L�[���̍\����
	typedef struct
	{
		int nFrame; //�t���[��
		int nSound;	//���̉����邩
		float fMove; //���̃L�[�̐����ړ���
		float fHeight; //���̃L�[�̐����ړ���
		KEY aKey[32]; //�e���f���̃L�[�v�f []���̓p�[�c�̍ő吔
	} KEY_INFO;

	//���[�V�������̍\����
	typedef struct
	{
		bool bLoop;	//���[�v���邩�ǂ���
		bool bGravity; //�d�͂�������
		int nNumKey;	//�L�[�̑���
		int nNonOpe; //����s�\����
		KEY_INFO aKeyInfo[16];	//�L�[��� []���̓L�[�̍ő吔
	} MOTION_INFO;

public:
	CPlayer(PRIORITY Priority);
	~CPlayer();
	HRESULT Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_PLAYER; }
	float GetRadius() { return m_fRadius; }
	float GetHeight() { return m_fHeight; }
	D3DXMATRIX GetMatrix(int nPart) { return m_pModel[nPart]->GetMatrix(); }
	static CPlayer *Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName);

private:
	void Load(const char *aFileName);
	bool Move(float fRotY);
	bool Stop();
	void Jump();
	void GaugeFadeIn();
	void GaugeFadeOut();
	void MouseSelect();
	void JumpCharge();
	void JumpFirst();
	void JumpCharging();
	void MouseMoveX();
	void Gravity();
	void RunFirst();
	void RunStop();
	void CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);
	void CollisionPlane(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld);
	bool CollisionLand(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene);
	void CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene);
	bool Collision(D3DXVECTOR3 aPoint[4], D3DXVECTOR3 Point, D3DXVECTOR3 PointOld);
	bool CollisionBlock(D3DXVECTOR3 aPoint[4], D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, bool *bLand, D3DXVECTOR3 *Nextpos);
	bool CollisionCamera(D3DXVECTOR3 *pPos, CScene *pScene);
	void ShadowObject(D3DXVECTOR3 aPoint[4], D3DXVECTOR3 *pPos);
	void Motion();
	void MotionChange(MOTIONTYPE motionType);
	void PauseChange(int nAdd);
	void PauseSelect();
	void EndChange(int nAdd);
	void EndSelect();

	D3DXVECTOR3 m_PosOld;
	D3DXVECTOR3 m_move;
	D3DXVECTOR3 m_rotDesh;
	D3DXVECTOR3 m_RestartPos;
	float m_fMove;
	float m_fChargeJump;
	float m_fGravity;
	float m_fJump;
	float m_fHeight;
	float m_fRadius;
	float m_fJumpMoveX;
	float m_fGaugeFade;
	bool m_bCharge;
	bool m_bLanding;
	bool m_bLandObject;	//�I�u�W�F�N�g�ɒ��n
	bool m_bLandField;	//�n�ʂɒ��n
	bool m_bChargePlus;
	bool m_bGaugeFade;
	int m_nFastMotion;
	int m_nNumModel;
	int m_nTotalTime;
	MOTIONTYPE m_motionType;
	MOTIONTYPE m_motionTypeOld;
	int m_nCntMotion;
	MOTION_INFO m_aMotionInfo[32];
	MOTION_INFO m_MotionInfo;
	int m_nNeutralTime;
	KEY m_aKeyDiff[32];
	int m_nKey;
	CModel *m_pModel[32]; //���f���i�p�[�c�j
	CKeyboard *m_pKeyboard;
	//CGamePad *m_pGamePad;
	CMouse *m_pMouse;
	CCamera *m_pCamera;
	CShadow *m_pShadow;
	int m_nRunStop;
	D3DXVECTOR3 m_MotionMove;
	bool m_bGameClear;
	bool m_bEndSelect;
	int m_nPauseSelect;
	float m_fShadowPosY;
	int m_nCntJump;
	D3DXVECTOR3 m_ShadowRot;
	CPauseUI *m_pPauseUI[3];
	CUI *m_pBlackGauge[3];
	CUI *m_pGauge;
	CUI *m_pBlockGauge[20];
	D3DXVECTOR2 m_aCamera[2];
};
#endif