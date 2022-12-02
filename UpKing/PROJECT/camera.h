//=============================================================================
//
// �V�[������ [camera.h]
// Author : �O��q��
//
//=============================================================================
#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

class CKeyboard;
class CMouse;
class CScene;

class CCamera
{
public:
	//�J�����̍\����
	typedef struct
	{
		D3DXVECTOR3 posV;		//���݂̎��_
		D3DXVECTOR3 posVDest;	//�ړI�̎��_
		D3DXVECTOR3 posR;		//���݂̒����_
		D3DXVECTOR3 posRDest;	//�ړI�̒����_
		D3DXVECTOR3 posPlus;
		D3DXVECTOR3 vecU;		//������x�N�g��
		D3DXVECTOR3 rot;		//�J�����̌���
		D3DXVECTOR3 rotDesh;    //�J�����̖ړI����
		float fLength;
		D3DXMATRIX mtxProjection; //�v���W�F�N�V�����}�g���b�N�X
		D3DXMATRIX mtxView; //�r���[�}�g���b�N�X
		bool bRot[2];			//X��Y
		bool bAdd[2];			//X��Y
	} Camera;
	CCamera();
	~CCamera();
	HRESULT Init(D3DXVECTOR3 ref, float fDistance, D3DXVECTOR3 pos);
	void Uninit();
	void Update();

	float GetRotY() { return m_camera.rot.y; }
	float GetRotX() { return m_camera.rot.x; }
	void SetPosV(D3DXVECTOR3 pos);
	void SetPosR(D3DXVECTOR3 pos);
	void SetRot(D3DXVECTOR3 rot) { m_camera.rot = rot; }
	void SetLength(float fLength) { m_camera.fLength = fLength; }
	D3DXVECTOR3 GetPosR() { return m_camera.posR; }
	D3DXVECTOR3 GetPosV() { return m_camera.posV; }
	void AddRotY(float fRotY);
	void AddRotX(float fRotX);
	void MoveX(float fMove);
	void MoveZ(float fMove);
	void ResetRot();
	void SetPlayer(CScene *pPlayer) { m_pPlayer = pPlayer; }
	void SetRotDeshX(float fRotX);
	void SetRotDeshY(float fRotY);
	void SetAdd(bool bAdd);
	void AddLength(float fLength) { m_camera.fLength += fLength; }
	void AddPosReset(void);
	void AddPosPlus(D3DXVECTOR3 pos, D3DXVECTOR2 aCamera[2]);

	static CCamera *Create(D3DXVECTOR3 ref, float fDistance, D3DXVECTOR3 pos);

private:
	Camera m_camera;
	CScene *m_pPlayer;
};

#endif