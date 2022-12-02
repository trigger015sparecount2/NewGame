//---------------------------
//Author:�O��q��
//���f��(player.cpp)
//---------------------------
#include "player.h"
#include "manager.h"
#include "renderer.h"
#include "mouse.h"
#include "keyboard.h"
#include "model.h"
#include "camera.h"
#include "fade.h"
#include "shadow.h"
#include "gamepad.h"
#include "game.h"
#include "sound.h"
#include "ui.h"
#include "pauseui.h"

//=============================================================================
//�ÓI
//=============================================================================

//=============================================================================
//�}�N��
//=============================================================================
#define MAX_JUMPMOVEX (10.0f)
#define MAX_JUMPMOVEY (20.0f)
#define MAX_GRAVITY (-30.0f)
#define REFLECT (-0.6f)
#define SCREEN_HEIGHT_DIFF (30.0f)	//�E�B���h�E�̈�(�g�S��)�ƃN���C�A���g�̈�(�`�悵�Ă�̈�)�̍�(�c)
#define SCREEN_WIDTH_DIFF (8.0f)	//�E�B���h�E�̈�(�g�S��)�ƃN���C�A���g�̈�(�`�悵�Ă�̈�)�̍�(��)

CPlayer::CPlayer(PRIORITY Priority) : CScene3D::CScene3D(Priority)
{

}

CPlayer::~CPlayer()
{

}

//����������
HRESULT CPlayer::Init(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName)
{
	Load(aFileName);
	m_pBlackGauge[0] = CUI::Create(D3DXVECTOR3(640.0f, 100.0f, 0.0f), 723.0f, 42.0f, -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)); //��ʏ㕔�Q�[�W�̉��n
	m_pBlackGauge[1] = CUI::Create(D3DXVECTOR3(500.0f, 474.0f, 0.0f), 42.0f, 170.0f, -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)); //��ʍ��Q�[�W�̉��n
	m_pBlackGauge[2] = CUI::Create(D3DXVECTOR3(780.0f, 474.0f, 0.0f), 42.0f, 170.0f, -1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f)); //��ʉE�Q�[�W�̉��n
	m_pGauge = CUI::Create(D3DXVECTOR3(280.0f, 100.0f, 0.0f), 0.0f, 40.0f, -1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	int nCnt;
	for (nCnt = 0; nCnt < 20; nCnt++)
	{
		if (nCnt < 10)
		{
			m_pBlockGauge[nCnt] = CUI::Create(D3DXVECTOR3(500.0f, 550.0f - 17.0f * nCnt, 0.0f), 40.0f, 15.0f, -1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
		}
		else
		{
			m_pBlockGauge[nCnt] = CUI::Create(D3DXVECTOR3(780.0f, 550.0f - 17.0f * (nCnt - 10), 0.0f), 40.0f, 15.0f, -1, D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
		}
	}
	m_pPauseUI[0] = CPauseUI::Create(D3DXVECTOR3(640.0f, 260.0f, 0.0f), 200.0f, 50.0f, 1, D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f)); //�|�[�Y��������
	m_pPauseUI[1] = CPauseUI::Create(D3DXVECTOR3(640.0f, 360.0f, 0.0f), 180.0f, 50.0f, 2, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f)); //�|�[�Y�����g���C
	m_pPauseUI[2] = CPauseUI::Create(D3DXVECTOR3(640.0f, 460.0f, 0.0f), 140.0f, 50.0f, 3, D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f)); //�|�[�Y���I��
	SetPos(pos);
	SetRot(rot);
	m_rotDesh = rot;
	m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_RestartPos = pos;
	m_PosOld = pos;
	m_pKeyboard = CManager::GetKeyboard();
	//m_pGamePad = CManager::GetGamepad();
	m_pMouse = CManager::GetMouse();
	m_pCamera = CManager::GetRenderer()->GetCamera();
	m_pCamera->ResetRot();
	m_pCamera->SetPlayer(this);
	m_bLandObject = false;
	m_motionTypeOld = MOTIONTYPE_NEUTRAL;
	m_nRunStop = 0;
	m_nPauseSelect = 0;
	m_bGameClear = false;
	m_bEndSelect = false;
	D3DXVECTOR3 size;
	size.x = GetRadius();
	size.y = 0.0f;
	size.z = GetRadius();
	m_fShadowPosY = 1000000.0f;
	m_pShadow = CShadow::Create(size, D3DXVECTOR3(pos.x, pos.y + 0.1f, pos.z));
	MotionChange(MOTIONTYPE_RETURN);
	m_bLandField = false;
	m_bLanding = false;
	m_fChargeJump = 4.0f;
	m_ShadowRot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_bCharge = false;
	m_bGaugeFade = false;
	m_bChargePlus = true;
	m_fJumpMoveX = 0.0f;
	m_fGaugeFade = 0.0f;
	m_nCntJump = 0;
	m_nFastMotion = 0;
	return S_OK;
}

//�I������
void CPlayer::Uninit()
{
	if (m_pCamera != NULL)
	{
		m_pCamera = NULL;
	}
	/*if (m_pGamePad != NULL)
	{
		m_pGamePad = NULL;
	}*/
	if (m_pKeyboard != NULL)
	{
		m_pKeyboard = NULL;
	}
	int nCnt;
	for (nCnt = 0; nCnt < 32; nCnt++)
	{
		if (m_pModel[nCnt] != NULL)
		{
			m_pModel[nCnt]->Uninit();
			m_pModel[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 20; nCnt++)
	{
		if (m_pBlockGauge[nCnt] != NULL)
		{
			m_pBlockGauge[nCnt] = NULL;
		}
	}
	if (m_pMouse != NULL)
	{
		m_pMouse = NULL;
	}
	if (m_pShadow != NULL)
	{
		m_pShadow->SetDeath(true);
		m_pShadow = NULL;
	}
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pPauseUI[nCnt] != NULL)
		{
			m_pPauseUI[nCnt] = NULL;
		}
	}
	for (nCnt = 0; nCnt < 3; nCnt++)
	{
		if (m_pBlackGauge[nCnt] != NULL)
		{
			m_pBlackGauge[nCnt] = NULL;
		}
	}
	if (m_pGauge != NULL)
	{
		m_pGauge = NULL;
	}
	CScene3D::Uninit();
}

//�X�V����
void CPlayer::Update()
{
	D3DXVECTOR3 pos = GetPos();
	D3DXVECTOR3 rot = GetRot();
	m_PosOld = pos;
	bool bRun = false;
	m_fShadowPosY = -100000.0f;
	if (CManager::GetPause() == false)
	{
		if (m_pKeyboard != NULL && m_pMouse != NULL)
		{
			if (m_bLanding == true)
			{
				if (m_pKeyboard->GetPress(DIK_A) == true)
				{
					bRun = Move(-0.5f);
				}
				else if (m_pKeyboard->GetPress(DIK_D) == true)
				{
					bRun = Move(0.5f);
				}
				else
				{
					Stop();
				}
			}
			if (m_pKeyboard->GetKey(DIK_ESCAPE) == true)
			{
				CSound::Play(8);
				CManager::SetPause(true);
			}
			if (bRun == true && m_bLanding == true && m_motionType != MOTIONTYPE_RUN)
			{
				RunFirst();
			}
			if (bRun == false && m_motionType == MOTIONTYPE_RUN)
			{
				RunStop();
			}
			if (m_bLanding == true)
			{
				JumpCharge();
			}
			else
			{
				Gravity();
			}
			if (m_pMouse->GetPress(CMouse::DIM_L) != true && m_pMouse->GetPress(CMouse::DIM_R) == true) //�}�E�X�̍��������Ă��炸�A�E�̂݉����Ă���
			{
				if (m_pMouse->MouseX() != 0.0f || m_pMouse->MouseY() != 0.0f)
				{
					m_pCamera->AddPosPlus(D3DXVECTOR3(m_pMouse->MouseX() * 0.7f, m_pMouse->MouseY() * -0.7f, 0.0f), &m_aCamera[0]);
				}
			}
			else if(m_pMouse->GetMouseButton(CMouse::DIM_L) == true || m_pMouse->GetRelease(CMouse::DIM_R) == true)
			{
				m_pCamera->AddPosReset();
			}
		}
		pos += m_move;
		if (m_bLanding == false)
		{
			pos.x += m_fJumpMoveX;
		}
		CollisionObject(&pos, m_PosOld);
		CollisionPlane(&pos, m_PosOld);
		if (m_bLandField == true || m_bLandObject == true)
		{
			m_bLanding = true;
		}
		else if(m_bLandField == false && m_bLandObject == false)
		{
			m_bLanding = false;
		}
		if (m_bLanding == false)
		{
			m_pShadow->MoveY(D3DXVECTOR3(pos.x, m_fShadowPosY, pos.z), m_ShadowRot);
			if (m_motionType != MOTIONTYPE_FRONTJUMP && m_motionType != MOTIONTYPE_BACKJUMP)
			{
				MotionChange(MOTIONTYPE_FRONTJUMP);
			}
		}
		if (m_bGaugeFade == true)
		{
			GaugeFadeOut();
		}
		else
		{
			GaugeFadeIn();
		}
		#ifdef _DEBUG
		if (m_pKeyboard != NULL) //�`�[�g�R�}���h
		{
			if (m_pKeyboard->GetKey(DIK_RETURN) == true) //���X�^�[�g
			{
				m_move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pos = m_RestartPos;
				m_fJumpMoveX = 0.0f;
			}
			if (m_pKeyboard->GetKey(DIK_LSHIFT) == true) //���X�^�[�g�|�C���g�X�V
			{
				m_RestartPos = pos;
			}
			if (m_pKeyboard->GetKey(DIK_RSHIFT) == true) //���[�h������
			{
				CFade::SetFade(CManager::MODE_GAME);
			}
		}
		#endif
		if (rot.y >= D3DX_PI)
		{
			rot.y -= D3DX_PI * 2;
		}
		else if (rot.y < -D3DX_PI)
		{
			rot.y += D3DX_PI * 2;
		}
		if (rot.y != m_rotDesh.y)
		{
			if (m_rotDesh.y - rot.y < -D3DX_PI)
			{
				rot.y += (m_rotDesh.y - rot.y + D3DX_PI * 2) * 0.14f;
			}
			else if (m_rotDesh.y - rot.y > D3DX_PI)
			{
				rot.y += (m_rotDesh.y - rot.y - D3DX_PI * 2) * 0.14f;
			}
			else
			{
				rot.y += (m_rotDesh.y - rot.y) * 0.14f;
			}
		}
		if (pos.y > 40000.0f)
		{
			CFade::SetFade(CManager::MODE_RESULT);
		}
		SetPos(pos);
		m_pShadow->Move(pos.y);
		SetRot(rot);
		Motion();
	}
	else
	{
		if (m_pKeyboard != NULL || m_pMouse != NULL)
		{
			if (m_pKeyboard != NULL)
			{
				if (m_pKeyboard->GetKey(DIK_W) == true)
				{
					PauseChange(-1);
				}
				if (m_pKeyboard->GetKey(DIK_S) == true)
				{
					PauseChange(1);
				}
				if (m_pKeyboard->GetKey(DIK_RETURN) == true)
				{
					PauseSelect();
				}
				if (m_pKeyboard->GetKey(DIK_ESCAPE) == true)
				{
					CSound::Play(3);
					CManager::SetPause(false);
				}
			}
			if (m_pMouse != NULL)
			{
				//�}�E�X�𓮂������Ƃ��̏���
				MouseSelect();
				if (m_pMouse->GetMouseButton(CMouse::DIM_L) == true)
				{
					PauseSelect();
				}
			}
		}
	}
}

//�`�揈��
void CPlayer::Draw()
{
	LPDIRECT3DDEVICE9 pDevice;   //�f�o�C�X�̃|�C���^
	D3DXMATRIX mtxPlayer, mtxRot, mtxTrans;
	pDevice = CManager::GetRenderer()->GetDevice();		 //�f�o�C�X���擾����

	//�v���C���[�̃��[���h�}�g���b�N�X�̏�����
	D3DXMatrixIdentity(&mtxPlayer);
	//�v���C���[�̌����𔽉f
	D3DXMatrixRotationYawPitchRoll(&mtxRot, GetRot().y, GetRot().x, GetRot().z);
	D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxRot);
	//�v���C���[�̈ʒu�𔽉f
	D3DXMatrixTranslation(&mtxTrans, GetPos().x, GetPos().y, GetPos().z);
	D3DXMatrixMultiply(&mtxPlayer, &mtxPlayer, &mtxTrans);
	//�v���C���[�̃��[���h�}�g���b�N�X�̐ݒ�
	pDevice->SetTransform(D3DTS_WORLD, &mtxPlayer);
	SetMatrix(mtxPlayer);
	int nCntModel;
	for (nCntModel = 0; nCntModel < m_nNumModel; nCntModel++)
	{
		if (m_pModel[nCntModel] != NULL)
		{
			m_pModel[nCntModel]->Draw();
		}
	}
}

void CPlayer::Load(const char *aFileName)
{
	LPDIRECT3DDEVICE9 pDevice; //�f�o�C�X�̃|�C���^
	pDevice = CManager::GetRenderer()->GetDevice();     //�f�o�C�X���擾����
	D3DXVECTOR3 rot;
	D3DXVECTOR3 pos;
	int nParent;
	CModel *paModel[32];

	int nCntParts = 0;
	int nCntMotion = 0;
	int nCntKey = 0;
	int nCntCol = 0;
	char aFile[256];
	int nLoop = 0;
	int nGravity = 0;

	bool bChara = false;
	bool bParts = false;
	bool bMotion = false;
	bool bKey = false;
	bool bKeySet = false;
	int nIndex;

	FILE *pFile;
	pFile = fopen(aFileName, "r");
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanf���J��Ԃ��ăt�@�C����ǂݎ���Ă���
			if (strcmp(&aFile[0], "#") == 0) //���f���t�@�C���ǂݍ���
			{
				fscanf(pFile, "%s", &aFile[0]);
			}
			if (strcmp(&aFile[0], "MODEL_FILENAME") == 0) //���f���t�@�C���ǂݍ���
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				paModel[nCntParts] = CModel::Create(&aFile[0]);
				nCntParts++;
			}
			if (bChara == true) //CHARACTERSET��
			{
				if (strcmp(&aFile[0], "NUM_PARTS") == 0) //�p�[�c��
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_nNumModel);
				}
				if (strcmp(&aFile[0], "MOVE") == 0) //�ړ����x
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fMove);
				}
				if (strcmp(&aFile[0], "JUMP") == 0) //�W�����v��
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fJump);
				}
				if (strcmp(&aFile[0], "GRAVITY") == 0) //�d��
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fGravity);
				}
				if (strcmp(&aFile[0], "RADIUS") == 0) //���a
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fRadius);
				}
				if (strcmp(&aFile[0], "HEIGHT") == 0) //�g��
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &m_fHeight);
				}
				if (strcmp(&aFile[0], "PARTSSET") == 0)
				{
					bParts = true;
				}
				if (strcmp(&aFile[0], "END_PARTSSET") == 0)
				{
					bParts = false;
					nCntParts++;
				}
				if (bParts == true) //PARTSSET��
				{
					//if (strcmp(&aFile[0], "MOVEPARTS") == 0) //���ꂪ������g_player��pos�������i����Ȃ������j
					//{
					//	g_player.nMoveParts = nCntParts;
					//}
					if (strcmp(&aFile[0], "INDEX") == 0) //���f���t�@�C���ǂݍ��݂̎��́A���Ԗڂ̃��f�����Ăяo����
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &nIndex);
						m_pModel[nCntParts] = paModel[nIndex];
					}
					if (strcmp(&aFile[0], "PARENT") == 0) //�e���f��
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &nParent);
						if (nParent != -1)
						{
							m_pModel[nCntParts]->SetParent(m_pModel[nParent]);
						}
					}
					if (strcmp(&aFile[0], "POS") == 0) //�e�p�[�c�̏ꏊ
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
						m_pModel[nCntParts]->SetOriPos(pos);
					}
					if (strcmp(&aFile[0], "ROT") == 0) //�e�p�[�c�̊p�x
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
						m_pModel[nCntParts]->SetOriRot(rot);
					}
				}
			}
			if (bMotion == true && nCntMotion < MOTIONTYPE_RETURN) //MOTIONSET��
			{
				if (strcmp(&aFile[0], "LOOP") == 0) //���̃��[�V���������[�v���邩
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nLoop);
					if (nLoop == 0)
					{
						m_aMotionInfo[nCntMotion].bLoop = false;
					}
					else
					{
						m_aMotionInfo[nCntMotion].bLoop = true;
					}
				}
				if (strcmp(&aFile[0], "GRAVITY") == 0) //�d�͂�������
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nGravity);
					if (nGravity == 0)
					{
						m_aMotionInfo[nCntMotion].bGravity = false;
					}
					else
					{
						m_aMotionInfo[nCntMotion].bGravity = true;
					}
				}
				if (strcmp(&aFile[0], "NUM_KEY") == 0) //�����L�[�����邩
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNumKey);
				}
				if (strcmp(&aFile[0], "NON_OPE") == 0) //����s�\����
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].nNonOpe);
				}
				if (strcmp(&aFile[0], "KEYSET") == 0)
				{
					bKeySet = true;
					m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fMove = 0.0f;
					m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fHeight = 0.0f;
					m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nSound = -1;
					nCntParts = 0;
				}
				if (strcmp(&aFile[0], "END_KEYSET") == 0)
				{
					bKeySet = false;
					nCntKey++;
				}
				if (bKeySet == true) //KEYSET��
				{
					if (strcmp(&aFile[0], "FRAME") == 0) //�t���[����
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nFrame);
					}
					if (strcmp(&aFile[0], "SOUND") == 0) //��
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].nSound);
					}
					if (strcmp(&aFile[0], "MOVE") == 0) //�ړ���(��)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fMove);
					}
					if (strcmp(&aFile[0], "HEIGHT") == 0) //�ړ���(�c)
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].fHeight);
					}
					if (strcmp(&aFile[0], "KEY") == 0)
					{
						bKey = true;
					}
					if (strcmp(&aFile[0], "END_KEY") == 0)
					{
						bKey = false;
						nCntParts++;
					}
					if (bKey == true) //KEY��
					{
						if (strcmp(&aFile[0], "POS") == 0) //�ꏊ
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].pos.x,
								&m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].pos.y, &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].pos.z);
						}
						if (strcmp(&aFile[0], "ROT") == 0) //�p�x
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f %f", &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].rot.x,
								&m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].rot.y, &m_aMotionInfo[nCntMotion].aKeyInfo[nCntKey].aKey[nCntParts].rot.z);
						}
					}
				}
			}
			if (strcmp(&aFile[0], "CHARACTERSET") == 0)
			{
				bChara = true;
				nCntParts = 0;
			}
			if (strcmp(&aFile[0], "END_CHARACTERSET") == 0)
			{
				bChara = false;
			}
			if (strcmp(&aFile[0], "MOTIONSET") == 0)
			{
				bMotion = true;
				nCntKey = 0;
				nCntCol = 0;
			}
			if (strcmp(&aFile[0], "END_MOTIONSET") == 0)
			{
				bMotion = false;
				nCntMotion++;
			}
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //�I����1��
			{
				break;
			}
		}
		fclose(pFile);
	}
	//�uMOTIONTYPE_RETURN�v�́A�j���[�g�������[�V�����ɖ߂�ۂɌo�R���郂�[�V����
	m_aMotionInfo[MOTIONTYPE_RETURN].bLoop = false;
	m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].nFrame = 10;
	m_aMotionInfo[MOTIONTYPE_RETURN].nNumKey = 1;
	m_aMotionInfo[MOTIONTYPE_RETURN].bGravity = true;
	m_aMotionInfo[MOTIONTYPE_RETURN].nNonOpe = 0;
	m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].nSound = -1;
	for (nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		m_aMotionInfo[MOTIONTYPE_RETURN].aKeyInfo[0].aKey[nCntParts] = m_aMotionInfo[MOTIONTYPE_NEUTRAL].aKeyInfo[m_aMotionInfo[MOTIONTYPE_NEUTRAL].nNumKey - 1].aKey[nCntParts];
	}
}

bool CPlayer::Move(float fRotY)
{
	float fCameraRotY = m_pCamera->GetRotY();
	m_move.x += (sinf(fCameraRotY + D3DX_PI * fRotY) * m_fMove - m_move.x) * 0.1f;
	m_move.z += (cosf(fCameraRotY + D3DX_PI * fRotY) * m_fMove - m_move.z) * 0.1f;
	m_rotDesh.y = fCameraRotY + D3DX_PI * fRotY + D3DX_PI;
	if (m_rotDesh.y >= D3DX_PI)
	{
		m_rotDesh.y -= D3DX_PI * 2;
	}
	if (m_rotDesh.y < -D3DX_PI)
	{
		m_rotDesh.y += D3DX_PI * 2;
	}
	if (m_nRunStop > 0)
	{
		m_nRunStop = 0;
	}
	return true;
}

bool CPlayer::Stop()
{
	m_move.x -= m_move.x * 0.2f;
	m_move.z -= m_move.z * 0.2f;
	return false;
}

void CPlayer::Jump()
{
	m_move.y = m_fChargeJump;
	m_bLanding = false;
	m_bLandField = false;
	m_bLandObject = false;
	m_bCharge = false;
	m_fChargeJump = 2.0f;
	if ((m_fJumpMoveX <= 0.0f && GetRot().y > 0.0f) || (m_fJumpMoveX >= 0.0f && GetRot().y < 0.0f))
	{
		MotionChange(MOTIONTYPE_FRONTJUMP);
	}
	else if((m_fJumpMoveX > 0.0f && GetRot().y > 0.0f) || (m_fJumpMoveX < 0.0f && GetRot().y < 0.0f))
	{
		MotionChange(MOTIONTYPE_BACKJUMP);
	}
	//m_pGauge->SetBesideGauge(0.0f);
	m_nCntJump = 60;
}

void CPlayer::JumpCharge()
{
	if (m_pMouse->GetMouseButton(CMouse::DIM_L) == true)
	{
		JumpFirst();
	}
	if (m_bCharge == true)
	{
		if (m_pMouse->GetPress(CMouse::DIM_L) == true)
		{
			JumpCharging();
		}
		else if (m_pMouse->GetRelease(CMouse::DIM_L) == true)
		{
			Jump();
		}
	}
	else
	{
		if (m_fChargeJump > 4.0f)
		{
			m_fChargeJump = 4.0f;
		}
		if (m_nCntJump > 0)
		{
			m_nCntJump--;
			if (m_nCntJump <= 0)
			{
				m_bGaugeFade = false;
			}
		}
	}
}

void CPlayer::JumpFirst()
{
	m_bCharge = true;
	m_bGaugeFade = true;
	MotionChange(MOTIONTYPE_CHARGE);
	m_pGauge->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
	int nCnt;
	for (nCnt = 0; nCnt < 20; nCnt++)
	{
		m_pBlockGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
	}
}

void CPlayer::JumpCharging()
{
	if (m_bChargePlus == true)
	{
		m_fChargeJump += 0.4f;
		if (m_fChargeJump > MAX_JUMPMOVEY)
		{
			m_fChargeJump = MAX_JUMPMOVEY;
			m_bChargePlus = false;
		}
		m_pGauge->SetBesideGauge((m_fChargeJump - 4.0f) * 45.0f);
	}
	else
	{
		m_fChargeJump -= 0.4f;
		if (m_fChargeJump < 4.0f)
		{
			m_fChargeJump = 4.0f;
			m_bChargePlus = true;
		}
		m_pGauge->SetBesideGauge((m_fChargeJump - 4.0f) * 45.0f);
	}
	if (m_pMouse->MouseX() != 0.0f)
	{
		MouseMoveX();
	}
}

void CPlayer::MouseMoveX()
{
	int nCnt;
	m_fJumpMoveX += m_pMouse->MouseX() * 0.015f;
	if (-0.1f < m_fJumpMoveX && m_fJumpMoveX < 0.1f)
	{
		m_fJumpMoveX = 0.0f;
	}
	if (m_fJumpMoveX > MAX_JUMPMOVEX)
	{
		m_fJumpMoveX = MAX_JUMPMOVEX;
	}
	else if (m_fJumpMoveX < -MAX_JUMPMOVEX)
	{
		m_fJumpMoveX = -MAX_JUMPMOVEX;
	}
	if (m_fJumpMoveX > 0.0f) //�E���ɔ�ԂƂ�
	{
		for (nCnt = 0; nCnt < 20; nCnt++)
		{
			if (nCnt < 10)
			{
				m_pBlockGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
			}
			else
			{
				if ((nCnt - 10) < floorf(m_fJumpMoveX))
				{
					m_pBlockGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				}
				else if (floorf(m_fJumpMoveX) <= (nCnt - 10) && (nCnt - 10) < ceilf(m_fJumpMoveX))
				{
					m_pBlockGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - (ceilf(m_fJumpMoveX) - m_fJumpMoveX)));
				}
				else if (m_fJumpMoveX < (nCnt - 10))
				{
					m_pBlockGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
				}
			}
		}
	}
	else if (m_fJumpMoveX < 0.0f) //�����ɔ�ԂƂ�
	{
		for (nCnt = 0; nCnt < 20; nCnt++)
		{
			if (nCnt < 10)
			{
				if (nCnt < floorf(fabsf(m_fJumpMoveX)))
				{
					m_pBlockGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f));
				}
				else if (floorf(fabsf(m_fJumpMoveX)) <= nCnt && nCnt < ceilf(fabsf(m_fJumpMoveX)))
				{
					m_pBlockGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f - (m_fJumpMoveX + ceilf(fabsf(m_fJumpMoveX)))));
				}
				else if (fabsf(m_fJumpMoveX) < nCnt)
				{
					m_pBlockGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
				}
			}
			else
			{
				m_pBlockGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
			}
		}
	}
	else //�����W�����v
	{
		for (nCnt = 0; nCnt < 20; nCnt++)
		{
			m_pBlockGauge[nCnt]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
		}
	}
}

void CPlayer::GaugeFadeIn()
{
	if (m_fGaugeFade > 0.0f)
	{
		m_fGaugeFade -= 0.1f;
		if (m_fGaugeFade < 0.0f)
		{
			m_fGaugeFade = 0.0f;
		}
		int nCntUI;
		for (nCntUI = 0; nCntUI < 3; nCntUI++)
		{
			m_pBlackGauge[nCntUI]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fGaugeFade));
		}
		for (nCntUI = 0; nCntUI < 20; nCntUI++)
		{
			m_pBlockGauge[nCntUI]->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, 0.0f));
		}
		m_pGauge->ColorChange(D3DXCOLOR(1.0f, 1.0f, 0.0f, m_fGaugeFade));
	}
}

void CPlayer::GaugeFadeOut()
{
	if (m_fGaugeFade < 1.0f)
	{
		m_fGaugeFade += 0.1f;
		if (m_fGaugeFade > 1.0f)
		{
			m_fGaugeFade = 1.0f;
		}
		int nCntUI;
		for (nCntUI = 0; nCntUI < 3; nCntUI++)
		{
			m_pBlackGauge[nCntUI]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, m_fGaugeFade));
		}
	}
}

void CPlayer::MouseSelect()
{
	if (m_pMouse->MouseX() != 0.0f || m_pMouse->MouseY() != 0.0f)
	{
		POINT CursorPoint;
		GetCursorPos(&CursorPoint);
		RECT Winrect;
		RECT Clirect;
		HWND hWnd = GethWnd();
		GetWindowRect(hWnd, &Winrect);	//�E�B���h�E�̈�̎擾
		GetClientRect(hWnd, &Clirect);	//�N���C�A���g�̈�̎擾
		float fRatio[2];	//�����A�A�X�y�N�g��̂���
		fRatio[0] = (Clirect.right - Clirect.left) / (float)SCREEN_WIDTH;	//��
		fRatio[1] = (Clirect.bottom - Clirect.top) / (float)SCREEN_HEIGHT;	//�c
		int nCntPause;
		for (nCntPause = 0; nCntPause < 3; nCntPause++)
		{
			D3DXVECTOR3 UIPos = m_pPauseUI[nCntPause]->GetPos();
			D3DXVECTOR3 UIMinSize = m_pPauseUI[nCntPause]->GetVtxMin() * 0.5f;
			D3DXVECTOR3 UIMaxSize = m_pPauseUI[nCntPause]->GetVtxMax() * 0.5f;
			if ((Winrect.left + SCREEN_WIDTH_DIFF) + (UIPos.x + UIMinSize.x) * fRatio[0] <= CursorPoint.x && CursorPoint.x <= (Winrect.left + SCREEN_WIDTH_DIFF) + (UIPos.x + UIMaxSize.x) * fRatio[0] &&
				(Winrect.top + SCREEN_HEIGHT_DIFF) + (UIPos.y + UIMinSize.y) * fRatio[1] <= CursorPoint.y && CursorPoint.y <= (Winrect.top + SCREEN_HEIGHT_DIFF) + (UIPos.y + UIMaxSize.y) * fRatio[1])
			{
				if (m_nPauseSelect != nCntPause)
				{
					m_pPauseUI[m_nPauseSelect]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f));
					m_nPauseSelect = nCntPause;
					m_pPauseUI[m_nPauseSelect]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
					CSound::Play(9);
				}
			}
		}
	}
}

void CPlayer::Gravity()
{
	if (m_move.y > MAX_GRAVITY)
	{
		m_move.y -= m_fGravity; //�󒆂ɋ���Ƃ��̏d�͂�����
								//�������x�����ȏ�ɂȂ�ƁA���[�V�����������Ȃ�
		if (m_move.y <= MAX_GRAVITY / 3.0f * 2.0f)
		{
			m_nFastMotion = -2;
		}
		else if (m_move.y <= MAX_GRAVITY / 3.0f)
		{
			m_nFastMotion = -1;
		}
	}
}

void CPlayer::RunFirst()
{
	m_bCharge = false;
	m_fChargeJump = 4.0f;
	MotionChange(MOTIONTYPE_RUN);
	m_fJumpMoveX = 0.0f;
	m_nCntJump = 10;
}

void CPlayer::RunStop()
{
	m_nRunStop++;
	if (m_nRunStop > 10)
	{
		MotionChange(MOTIONTYPE_RETURN);
		m_nRunStop = 0;
	}
}

void CPlayer::CollisionObject(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	CScene *pScene;
	D3DXVECTOR3 NextPos = *pPos;
	pScene = GetScene(PRIORITY_OBJECT);
	bool bLand = false;
	bool bStand;
	bool bHit;
	bool bShadow;
	while (pScene)
	{
		CScene *pSceneNext;
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJECTTYPE_BLOCK)
		{
			D3DXVECTOR3 pos = pScene->GetPos();
			if (fabsf(pPos->y - pos.y) < 1000.0f)
			{
				if (pScene->GetCollision() == CScene::COLLISION_SPHERE)
				{
					float fRadius = pScene->GetRadius();
					float fLengthX = pPos->x - pos.x;
					float fLengthY = pPos->y - pos.y;
					float fLengthZ = pPos->z - pos.z;
					fLengthX = powf(fLengthX, 2.0f);
					fLengthY = powf(fLengthY, 2.0f);
					fLengthZ = powf(fLengthZ, 2.0f);
					if (fLengthX + fLengthY + fLengthZ <= powf(m_fRadius + fRadius, 2.0f))
					{
						float fRotY = atanf(pos.x - pPos->x);
						float fRotX = atan2f(pPos->y - pos.y, pos.x - pPos->x);
						pPos->x = pos.x - fabsf(sinf(fRotY)) * cosf(fRotX) * (m_fRadius + fRadius);
					}
				}
				else if (pScene->GetCollision() == CScene::COLLISION_SQUARE)
				{
					D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
					D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
					D3DXVECTOR3 rot = pScene->GetRot();
					D3DXVECTOR3 aPoint[8]; //�I�u�W�F�N�g��8���_

					D3DXVECTOR2 MinSize = { 100000.0f,100000.0f };
					D3DXVECTOR2 MaxSize = { -100000.0f,-100000.0f };

					D3DXMATRIX mtxTrans, mtxWorld, mtxParent;
					mtxParent = pScene->GetMatrix();

					int nCntPos;
					for (nCntPos = 0; nCntPos < 8; nCntPos++)
					{
						//���[���h�}�g���b�N�X�̏�����
						D3DXMatrixIdentity(&mtxWorld);
						switch (nCntPos)
						{
						case 0:
							//�ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, VtxMin.x, VtxMax.y, VtxMax.z);
							break;
						case 1:
							//�ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, VtxMax.x, VtxMax.y, VtxMax.z);
							break;
						case 2:
							//�ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, VtxMin.x, VtxMax.y, VtxMin.z);
							break;
						case 3:
							//�ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, VtxMax.x, VtxMax.y, VtxMin.z);
							break;
						case 4:
							//�ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, VtxMin.x, VtxMin.y, VtxMax.z);
							break;
						case 5:
							//�ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, VtxMax.x, VtxMin.y, VtxMax.z);
							break;
						case 6:
							//�ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, VtxMin.x, VtxMin.y, VtxMin.z);
							break;
						case 7:
							//�ʒu�𔽉f
							D3DXMatrixTranslation(&mtxTrans, VtxMax.x, VtxMin.y, VtxMin.z);
							break;
						default:
							break;
						}
						D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

						//�e�̃}�g���b�N�X�Ɗ|�����킹��
						D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxParent);

						aPoint[nCntPos] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
						if (MaxSize.x < aPoint[nCntPos].x)
						{
							MaxSize.x = aPoint[nCntPos].x;
						}
						else if (MinSize.x > aPoint[nCntPos].x)
						{
							MinSize.x = aPoint[nCntPos].x;
						}
						if (MaxSize.y < aPoint[nCntPos].y)
						{
							MaxSize.y = aPoint[nCntPos].y;
						}
						else if (MinSize.y > aPoint[nCntPos].y)
						{
							MinSize.y = aPoint[nCntPos].y;
						}
					}
					//8���_���ꂼ��̍ő�l�A�ŏ��l�Ƃ̓����蔻��B���傫��
					if (MinSize.x <= pPos->x + m_fRadius && pPos->x - m_fRadius <= MaxSize.x && MinSize.y - m_fRadius <= pPos->y + m_fHeight && pPos->y <= MaxSize.y + m_fRadius)
					{
						bHit = true;
					}
					else
					{
						bHit = false;
					}
					if (MinSize.x <= pPos->x + m_fRadius && pPos->x - m_fRadius <= MaxSize.x && MinSize.y <= pPos->y)
					{
						bShadow = true;
					}
					else
					{
						bShadow = false;
					}
					D3DXVECTOR3 aSquare[4];
					int nCnt;
					for(nCnt = 0; nCnt < 4; nCnt++)
					{
						switch (nCnt)
						{
						case 0:	//��̖�
							aSquare[0] = aPoint[0];
							aSquare[1] = aPoint[1];
							aSquare[2] = aPoint[2];
							aSquare[3] = aPoint[3];
							break;
						case 1:	//�E�̖�
							aSquare[0] = aPoint[1];
							aSquare[1] = aPoint[5];
							aSquare[2] = aPoint[3];
							aSquare[3] = aPoint[7];
							break;
						case 2:	//���̖�
							aSquare[0] = aPoint[5];
							aSquare[1] = aPoint[4];
							aSquare[2] = aPoint[7];
							aSquare[3] = aPoint[6];
							break;
						case 3:	//���̖�
							aSquare[0] = aPoint[4];
							aSquare[1] = aPoint[0];
							aSquare[2] = aPoint[6];
							aSquare[3] = aPoint[2];
							break;
						default:
							break;
						}
						if (bHit == true)
						{
							bStand = CollisionBlock(&aSquare[0], pPos, posOld, &bLand, &NextPos);
							if (bStand == false && nCnt == 3)
							{
								*pPos = NextPos;
							}
						}
						if (bShadow == true)
						{
							ShadowObject(&aSquare[0], pPos);
						}
						if (bStand == true)
						{
							break;
						}
					}
				}
				/*else if (pScene->GetCollision() == CScene::COLLISION_PALL)
				{
					D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
					D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
					float fRadius = pScene->GetRadius();
				}*/
			}
		}
		pScene = pSceneNext;
	}
	if (bLand == false)
	{
		m_bLandObject = false;
	}
	else
	{
		m_bLandObject = true;
	}
}

void CPlayer::Motion() //���[�V�����̓���
{
	int nCntParts;
	m_nTotalTime++;
	m_nCntMotion++;
	for (nCntParts = 0; nCntParts < m_nNumModel; nCntParts++)
	{
		D3DXVECTOR3 posOld;
		posOld = m_pModel[nCntParts]->GetPos();
		D3DXVECTOR3 rotOld;
		rotOld = m_pModel[nCntParts]->GetRot();
		if (m_nCntMotion == -m_nFastMotion + 1) //���������߂�(�Ή�����L�[�̊e�p�[�c��pos(rot) - ���̊e�p�[�c��pos(rot))
		{
			m_aKeyDiff[nCntParts].pos.x = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].pos.x - posOld.x;
			m_aKeyDiff[nCntParts].pos.y = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].pos.y - posOld.y;
			m_aKeyDiff[nCntParts].pos.z = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].pos.z - posOld.z;
			m_aKeyDiff[nCntParts].rot.x = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].rot.x - rotOld.x;
			if (m_aKeyDiff[nCntParts].rot.x < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.x += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].rot.x > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.x -= D3DX_PI * 2;
			}
			m_aKeyDiff[nCntParts].rot.y = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].rot.y - rotOld.y;
			if (m_aKeyDiff[nCntParts].rot.y < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.y += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].rot.y > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.y -= D3DX_PI * 2;
			}
			m_aKeyDiff[nCntParts].rot.z = m_MotionInfo.aKeyInfo[m_nKey].aKey[nCntParts].rot.z - rotOld.z;
			if (m_aKeyDiff[nCntParts].rot.z < -D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.z += D3DX_PI * 2;
			}
			else if (m_aKeyDiff[nCntParts].rot.z > D3DX_PI)
			{
				m_aKeyDiff[nCntParts].rot.z -= D3DX_PI * 2;
			}
			if (m_MotionInfo.aKeyInfo[m_nKey].nSound != -1)
			{
				CSound::Play(m_MotionInfo.aKeyInfo[m_nKey].nSound);
			}
		}
		posOld.x += m_aKeyDiff[nCntParts].pos.x / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		posOld.y += m_aKeyDiff[nCntParts].pos.y / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		posOld.z += m_aKeyDiff[nCntParts].pos.z / m_MotionInfo.aKeyInfo[m_nKey].nFrame;
		m_pModel[nCntParts]->SetPos(posOld);

		rotOld.x += m_aKeyDiff[nCntParts].rot.x / (m_MotionInfo.aKeyInfo[m_nKey].nFrame + m_nFastMotion);
		rotOld.y += m_aKeyDiff[nCntParts].rot.y / (m_MotionInfo.aKeyInfo[m_nKey].nFrame + m_nFastMotion);
		rotOld.z += m_aKeyDiff[nCntParts].rot.z / (m_MotionInfo.aKeyInfo[m_nKey].nFrame + m_nFastMotion);
		m_pModel[nCntParts]->SetRot(rotOld);
	}
	if (m_nCntMotion == m_MotionInfo.aKeyInfo[m_nKey].nFrame)
	{
		m_nCntMotion = -m_nFastMotion;
		m_nKey++;
		if (m_nKey == m_MotionInfo.nNumKey)
		{
			if (m_MotionInfo.bLoop == false)
			{
				if (m_bLanding == true)
				{
					if (m_motionType != MOTIONTYPE_RETURN)
					{
						MotionChange(MOTIONTYPE_RETURN);
					}
					else
					{
						MotionChange(MOTIONTYPE_NEUTRAL);
					}
				}
				else
				{
					if (m_motionType != MOTIONTYPE_FRONTJUMP)
					{
						MotionChange(MOTIONTYPE_FRONTJUMP);
					}
				}
			}
			m_nKey = 0;
		}
	}
}

void CPlayer::MotionChange(MOTIONTYPE motionType)
{
	m_motionTypeOld = m_motionType;
	m_motionType = motionType;
	m_MotionInfo = m_aMotionInfo[motionType];
	m_nCntMotion = 0;
	m_nTotalTime = 0;
	m_nKey = 0;
	m_nFastMotion = 0;
}

bool CPlayer::CollisionLand(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene)
{
	D3DXVECTOR3 pos = pScene->GetPos();
	if (fabsf(pPos->y - pos.y) < 1000.0f)
	{
		D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
		D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
		D3DXVECTOR3 rot = pScene->GetRot();
		D3DXVECTOR3 aPoint[4]; //�n�ʂ�4���_

		D3DXMATRIX mtxTrans, mtxRot, mtxWorld, mtxParent;

		D3DXVECTOR3 MatMax = D3DXVECTOR3(-1000000.0f, -10000000.0f, -10000000.0f);
		D3DXVECTOR3 MatMin = D3DXVECTOR3(1000000.0f, 10000000.0f, 10000000.0f);

		mtxParent = pScene->GetMatrix();

		int nCntPos;
		for (nCntPos = 0; nCntPos < 4; nCntPos++)
		{
			//���[���h�}�g���b�N�X�̏�����
			D3DXMatrixIdentity(&mtxWorld);
			switch (nCntPos)
			{
			case 0:
				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, VtxMin.x, VtxMax.y, VtxMin.z);
				break;
			case 1:
				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, VtxMax.x, VtxMax.y, VtxMin.z);
				break;
			case 2:
				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, VtxMin.x, VtxMin.y, VtxMax.z);
				break;
			case 3:
				//�ʒu�𔽉f
				D3DXMatrixTranslation(&mtxTrans, VtxMax.x, VtxMin.y, VtxMax.z);
				break;
			default:
				break;
			}
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

			//�n�ʂ̃}�g���b�N�X�Ɗ|�����킹��
			D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxParent);

			aPoint[nCntPos] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);

			if (aPoint[nCntPos].x < MatMin.x)
			{
				MatMin.x = aPoint[nCntPos].x;
			}
			else if (MatMax.x < aPoint[nCntPos].x)
			{
				MatMax.x = aPoint[nCntPos].x;
			}
			if (aPoint[nCntPos].y < MatMin.y)
			{
				MatMin.y = aPoint[nCntPos].y;
			}
			else if (MatMax.y < aPoint[nCntPos].y)
			{
				MatMax.y = aPoint[nCntPos].y;
			}
			if (aPoint[nCntPos].z < MatMin.z)
			{
				MatMin.z = aPoint[nCntPos].z;
			}
			else if (MatMax.z < aPoint[nCntPos].z)
			{
				MatMax.z = aPoint[nCntPos].z;
			}
		}
		if (MatMin.x <= pPos->x + m_fRadius && pPos->x - m_fRadius <= MatMax.x && MatMin.z <= pPos->z - m_fRadius && pPos->z + m_fRadius <= MatMax.z)
		{
			D3DXVECTOR3 SubPoint[2];
			SubPoint[0] = aPoint[2] - aPoint[1];
			SubPoint[1] = aPoint[1] - aPoint[0];
			D3DXVECTOR3 Cross[3];
			D3DXVec3Cross(&Cross[0], &SubPoint[0], &SubPoint[1]);
			float fLength = powf((Cross[0].x * Cross[0].x) + (Cross[0].y * Cross[0].y) + (Cross[0].z * Cross[0].z), 0.5f);
			Cross[0].x /= fLength;
			Cross[0].y /= fLength;
			Cross[0].z /= fLength; //�����܂Ŗ@���x�N�g��

			Cross[1] = *pPos - aPoint[0]; //�v���C���[��pos�ƒn�ʂ̔C�ӂ̓_�̃x�N�g��

			float fDistance = (Cross[0].x * Cross[1].x) + (Cross[0].y * Cross[1].y) + (Cross[0].z * Cross[1].z);	//�����ƒn�ʂƂ̋��� �v���X�̎��A�\���ɂ���
			if (MatMin.y <= pPos->y)
			{
				D3DXVECTOR3 Point = D3DXVECTOR3(pPos->x - (Cross[0].x * fDistance), pPos->y - (Cross[0].y * fDistance), pPos->z - (Cross[0].z * fDistance));
				if (m_fShadowPosY < Point.y && Point.y < pPos->y)
				{
					m_fShadowPosY = Point.y;
					m_ShadowRot = rot;
				}
			}
			if (MatMin.y + 5.0f >= pPos->y && posOld.y >= MatMin.y - 5.0f)
			{
				Cross[2] = posOld - aPoint[0]; //�v���C���[��Oldpos�ƕǂ̔C�ӂ̓_�̃x�N�g��
				float fOldDistance = (Cross[0].x * Cross[2].x) + (Cross[0].y * Cross[2].y) + (Cross[0].z * Cross[2].z);	//�ߋ��ʒu�����ƒn�ʂƂ̋��� �v���X�̎��A�\���ɂ���
				if (fabsf(fDistance) <= 2.0f || fDistance <= 2.0f && fOldDistance >= -2.0f)
				{
					D3DXVECTOR3 aWideSquare[4];
					aWideSquare[0] = D3DXVECTOR3(aPoint[0].x - m_fRadius, aPoint[0].y, aPoint[0].z + m_fRadius);
					aWideSquare[1] = D3DXVECTOR3(aPoint[1].x + m_fRadius, aPoint[1].y, aPoint[1].z + m_fRadius);
					aWideSquare[2] = D3DXVECTOR3(aPoint[2].x - m_fRadius, aPoint[2].y, aPoint[2].z - m_fRadius);
					aWideSquare[3] = D3DXVECTOR3(aPoint[3].x + m_fRadius, aPoint[3].y, aPoint[3].z - m_fRadius);
					D3DXVECTOR3 Point = D3DXVECTOR3(pPos->x - (Cross[0].x * fDistance), pPos->y - (Cross[0].y * fDistance), pPos->z - (Cross[0].z * fDistance));
					D3DXVECTOR3 OldPoint = D3DXVECTOR3(posOld.x - (Cross[0].x * fOldDistance), posOld.y - (Cross[0].y * fOldDistance), posOld.z - (Cross[0].z * fOldDistance));
					if (Collision(&aWideSquare[0], Point, OldPoint))
					{
						pPos->y -= fDistance;
						m_pShadow->MoveY(D3DXVECTOR3(pPos->x, pPos->y + 0.2f, pPos->z), rot);
						m_move.y = 0.0f;
						if (m_bLanding == false && m_bLandField == false)
						{
							m_bLandField = true;
							m_bCharge = false;
							m_fJumpMoveX = 0.0f;
							MotionChange(MOTIONTYPE_LANDING);
						}
						return true;
					}
				}
			}
			else if (MatMax.y <= pPos->y + m_fHeight && MatMax.y >= posOld.y + m_fHeight)
			{
				Cross[2] = posOld - aPoint[0]; //�v���C���[��Oldpos�ƕǂ̔C�ӂ̓_�̃x�N�g��
				if (fDistance >= -m_fHeight)
				{
					pPos->y += (fDistance - m_fHeight);
					m_move.y = 0.0f;
				}
			}
		}
	}
	return false;
}

void CPlayer::CollisionWall(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, CScene *pScene)
{
	D3DXVECTOR3 pos = pScene->GetPos();
	D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
	D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
	D3DXVECTOR3 rot = pScene->GetRot();
	D3DXVECTOR3 aPoint[4]; //�ǂ�4���_

	D3DXMATRIX mtxTrans, mtxWorld, mtxParent;
	mtxParent = pScene->GetMatrix();

	int nCntPos;
	for (nCntPos = 0; nCntPos < 4; nCntPos++)
	{
		//���[���h�}�g���b�N�X�̏�����
		D3DXMatrixIdentity(&mtxWorld);
		switch (nCntPos)
		{
		case 0:
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, VtxMin.x, VtxMax.y, VtxMin.z);
			break;
		case 1:
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, VtxMax.x, VtxMax.y, VtxMax.z);
			break;
		case 2:
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, VtxMin.x, VtxMin.y, VtxMin.z);
			break;
		case 3:
			//�ʒu�𔽉f
			D3DXMatrixTranslation(&mtxTrans, VtxMax.x, VtxMin.y, VtxMax.z);
			break;
		default:
			break;
		}
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxTrans);

		//�ǂ̃}�g���b�N�X�Ɗ|�����킹��
		D3DXMatrixMultiply(&mtxWorld, &mtxWorld, &mtxParent);

		aPoint[nCntPos] = D3DXVECTOR3(mtxWorld._41, mtxWorld._42, mtxWorld._43);
	}

	/*
	aPoint[0] = D3DXVECTOR3(pos.x + VtxMin.x * cosf(rot.y) + VtxMax.y * sinf(rot.x), pos.y + VtxMax.y, pos.z + VtxMin.x * sinf(rot.y));
	aPoint[1] = D3DXVECTOR3(pos.x + VtxMax.x * cosf(rot.y) + VtxMax.y * sinf(rot.x), pos.y + VtxMax.y, pos.z + VtxMax.x * sinf(rot.y));
	aPoint[2] = D3DXVECTOR3(pos.x + VtxMin.x * cosf(rot.y) + VtxMin.y * sinf(rot.x), pos.y + VtxMin.y, pos.z + VtxMin.x * sinf(rot.y));
	aPoint[3] = D3DXVECTOR3(pos.x + VtxMax.x * cosf(rot.y) + VtxMin.y * sinf(rot.x), pos.y + VtxMin.y, pos.z + VtxMax.x * sinf(rot.y));
	*/

	if (aPoint[2].y <= pPos->y + m_fHeight && pPos->y <= aPoint[0].y || aPoint[0].y < posOld.y + m_fHeight && posOld.y < aPoint[2].y)
	{
		//D3DXVECTOR3 Cross[5];
		//D3DXVec3Cross(&Cross[0], &(aPoint[2] - aPoint[1]), &(aPoint[1] - aPoint[0]));
		//float fLength = powf((Cross[0].x * Cross[0].x) + (Cross[0].y * Cross[0].y) + (Cross[0].z * Cross[0].z), 0.5f);
		//Cross[0].x /= fLength;
		//Cross[0].y /= fLength;
		//Cross[0].z /= fLength; //�����܂Ŗ@���x�N�g��

		//Cross[1] = *pPos - aPoint[0]; //�v���C���[��pos�ƕǖʏ�̔C�ӂ̓_�̃x�N�g��
		//Cross[2] = posOld - aPoint[0]; //�v���C���[��Oldpos�ƕǖʏ�̔C�ӂ̓_�̃x�N�g��

		//Cross[3] = D3DXVECTOR3(pPos->x - aPoint[0].x, (pPos->y + m_fHeight) - aPoint[0].y, pPos->z - aPoint[0].z);		//�v���C���[�̓��ƕǖʏ�̔C�ӂ̓_�̃x�N�g��
		//Cross[4] = D3DXVECTOR3(posOld.x - aPoint[0].x, (posOld.y + m_fHeight) - aPoint[0].y, posOld.z - aPoint[0].z);	//�v���C���[�̉ߋ��ʒu�̓��ƕǖʏ�̔C�ӂ̓_�̃x�N�g��

		//float fDistance = Cross[0].x * Cross[1].x + Cross[0].y * Cross[1].y + Cross[0].z * Cross[1].z;		//�����ƕǖʂƂ̋��� �}�C�i�X�̎��A�\���ɂ���
		//float fOldDistance = Cross[0].x * Cross[2].x + Cross[0].y * Cross[2].y + Cross[0].z * Cross[2].z;	//�ߋ��ʒu�����ƕǖʂƂ̋��� �}�C�i�X�̎��A�\���ɂ���

		//float fHeadDistance = Cross[0].x * Cross[3].x + Cross[0].y * Cross[3].y + Cross[0].z * Cross[3].z;		//���ƕǖʂƂ̋��� �}�C�i�X�̎��A�\���ɂ���
		//float fHeadOldDistance = Cross[0].x * Cross[4].x + Cross[0].y * Cross[4].y + Cross[0].z * Cross[4].z;	//�ǖʂƂ̋��� �}�C�i�X�̎��A�\���ɂ���

		//D3DXVECTOR3 Point; //�ǖʏ�̃v���C���[�̈ʒu
		//D3DXVECTOR3 OldPoint; //�ǖʏ��1�t���[���O�̃v���C���[�̈ʒu

		////Point = D3DXVECTOR3(pPos->x - (Cross[0].x * fDistance), pPos->y - (Cross[0].y * fDistance), pPos->z - (Cross[0].z * fDistance)); //�ǖʏ�̃v���C���[�̈ʒu

		////D3DXVECTOR3 PosPoint = Point - *pPos;
		////D3DXVECTOR3 HeadPoint = D3DXVECTOR3(Point.x - pPos->x, Point.y - (pPos->y + m_fHeight) , Point.z - pPos->z);
		//if ((fabsf(fDistance) <= m_fRadius || fOldDistance <= -m_fRadius && fDistance >= -m_fRadius) || (fabsf(fHeadDistance) <= m_fRadius || fHeadOldDistance <= -m_fRadius && fHeadDistance >= -m_fRadius))
		//{
		//	if (fabsf(fDistance) <= m_fRadius || fOldDistance <= -m_fRadius && fDistance >= -m_fRadius)
		//	{
		//		Point = D3DXVECTOR3(pPos->x - (Cross[0].x * fDistance), pPos->y - (Cross[0].y * fDistance), pPos->z - (Cross[0].z * fDistance)); //�ǖʏ�̃v���C���[�̈ʒu
		//		OldPoint = D3DXVECTOR3(posOld.x - (Cross[0].x * fOldDistance), posOld.y - (Cross[0].y * fOldDistance), posOld.z - (Cross[0].z * fOldDistance)); //�ǖʏ��1�t���[���O�̃v���C���[�̈ʒu
		//		if (Collision(&aPoint[0], Point, OldPoint) == true)
		//		{
		//			pPos->x += sinf(rot.y) * (fabsf(fDistance) - m_fRadius);
		//			//pPos->z += cosf(rot.y) * (fabsf(fDistance) - m_fRadius);
		//			if (m_move.y > 0.0f && rot.x < 0.0f)
		//			{
		//				m_move.x = sinf(-rot.y) * sinf(rot.x) * m_move.y * 4.0f;
		//			}
		//		}
		//	}
		//	else if (fabsf(fHeadDistance) <= m_fRadius || fHeadOldDistance <= -m_fRadius && fHeadDistance >= -m_fRadius)
		//	{
		//		Point = D3DXVECTOR3(pPos->x - (Cross[0].x * fHeadDistance), pPos->y - (Cross[0].y * fHeadDistance), pPos->z - (Cross[0].z * fHeadDistance)); //�ǖʏ�̃v���C���[�̈ʒu
		//		OldPoint = D3DXVECTOR3(posOld.x - (Cross[0].x * fHeadOldDistance), posOld.y - (Cross[0].y * fHeadOldDistance), posOld.z - (Cross[0].z * fHeadOldDistance)); //�ǖʏ��1�t���[���O�̃v���C���[�̈ʒu
		//		if (Collision(&aPoint[0], Point, OldPoint) == true)
		//		{
		//			pPos->x += sinf(rot.y) * (fabsf(fHeadDistance) - m_fRadius);
		//			//pPos->z += cosf(rot.y) * (fabsf(fHeadDistance) - m_fRadius);
		//			if (m_move.y > 0.0f)
		//			{
		//				m_move.x = sinf(-rot.y) * sinf(rot.x) * m_move.y * 4.0f;
		//			}
		//		}
		//	}
		//}

		D3DXVECTOR3 Cross[5];
		D3DXVec3Cross(&Cross[0], &(aPoint[2] - aPoint[1]), &(aPoint[1] - aPoint[0]));
		float fLength = powf((Cross[0].x * Cross[0].x) + (Cross[0].y * Cross[0].y) + (Cross[0].z * Cross[0].z), 0.5f);
		Cross[0].x /= fLength;
		Cross[0].y /= fLength;
		Cross[0].z /= fLength; //�����܂Ŗ@���x�N�g��

		float fd = aPoint[0].x * Cross[0].x + aPoint[0].y * Cross[0].y + aPoint[0].z * Cross[0].z; //����D
		D3DXVECTOR3 fDCross = Cross[0] * fd;	//���ʏ�̓_
		D3DXVECTOR3 PosD = fDCross - *pPos;
		D3DXVECTOR3 HeadD = D3DXVECTOR3(fDCross.x - pPos->x, fDCross.y - (pPos->y + m_fHeight), fDCross.x - pPos->z);
		float fDotPosD = PosD.x * Cross[0].x + PosD.y * Cross[0].y + PosD.z * Cross[0].z;
		float fDotHeadD = HeadD.x * Cross[0].x + HeadD.y * Cross[0].y + HeadD.z * Cross[0].z;
		Cross[1] = *pPos - aPoint[0]; //�v���C���[��pos�ƕǖʏ�̔C�ӂ̓_�̃x�N�g��
		Cross[2] = posOld - aPoint[0]; //�v���C���[��Oldpos�ƕǖʏ�̔C�ӂ̓_�̃x�N�g��
		Cross[3] = D3DXVECTOR3(pPos->x - aPoint[0].x, (pPos->y + m_fHeight) - aPoint[0].y, pPos->z - aPoint[0].z);		//�v���C���[�̓��ƕǖʏ�̔C�ӂ̓_�̃x�N�g��
		Cross[4] = D3DXVECTOR3(posOld.x - aPoint[0].x, (posOld.y + m_fHeight) - aPoint[0].y, posOld.z - aPoint[0].z);	//�v���C���[�̉ߋ��ʒu�̓��ƕǖʏ�̔C�ӂ̓_�̃x�N�g��

		float fDistance = Cross[0].x * Cross[1].x + Cross[0].y * Cross[1].y + Cross[0].z * Cross[1].z;		//�����ƕǖʏ�Ƃ̋��� �v���X�̎��A�\���ɂ���
		float fOldDistance = Cross[0].x * Cross[2].x + Cross[0].y * Cross[2].y + Cross[0].z * Cross[2].z;	//�ߋ��ʒu�����ƕǖʏ�Ƃ̋��� �v���X�̎��A�\���ɂ���
		float fHeadDistance = Cross[0].x * Cross[3].x + Cross[0].y * Cross[3].y + Cross[0].z * Cross[3].z;		//���ƕǖʏ�Ƃ̋��� �}�C�i�X�̎��A�\���ɂ���
		float fHeadOldDistance = Cross[0].x * Cross[4].x + Cross[0].y * Cross[4].y + Cross[0].z * Cross[4].z;	//�ߋ��ʒu�ǖʏ�Ƃ̋��� �}�C�i�X�̎��A�\���ɂ���
		if ((fDotHeadD < 0.0f && fDotPosD > 0.0f) || (fDotHeadD > 0.0f && fDotPosD < 0.0f)) //�ǂő̂��؂��ꍇ
		{
			D3DXVECTOR3 Point = D3DXVECTOR3(pPos->x - (Cross[0].x * fDistance), pPos->y - (Cross[0].y * fDistance), pPos->z - (Cross[0].z * fDistance)); //�ǖʏ�̃v���C���[�̈ʒu
			D3DXVECTOR3 OldPoint = D3DXVECTOR3(posOld.x - (Cross[0].x * fOldDistance), posOld.y - (Cross[0].y * fOldDistance), posOld.z - (Cross[0].z * fOldDistance)); //�ǖʏ��1�t���[���O�̃v���C���[�̈ʒu
			D3DXVECTOR3 LeftPoint = D3DXVECTOR3(Point.x - m_fRadius, Point.y, Point.z);					//�ǖʏ�̃v���C���[�̈ʒu
			D3DXVECTOR3 LeftOldPoint = D3DXVECTOR3(OldPoint.x - m_fRadius, OldPoint.y, OldPoint.z);		//�ǖʏ��1�t���[���O�̃v���C���[�̈ʒu
			D3DXVECTOR3 RightPoint = D3DXVECTOR3(Point.x + m_fRadius, Point.y, Point.z);				//�ǖʏ�̃v���C���[�̈ʒu
			D3DXVECTOR3 RightOldPoint = D3DXVECTOR3(OldPoint.x + m_fRadius, OldPoint.y, OldPoint.z);	//�ǖʏ��1�t���[���O�̃v���C���[�̈ʒu
			if (Collision(&aPoint[0], LeftPoint, LeftOldPoint) == true)
			{
				pPos->x = aPoint[0].x + m_fRadius + 5.0f;
			}
			else if (Collision(&aPoint[0], RightPoint, RightOldPoint) == true)
			{
				pPos->x = aPoint[2].x - m_fRadius + 10.0f;
			}
		}
		else
		{
			if (fabsf(fDistance) <= m_fRadius || fOldDistance <= -m_fRadius && fDistance >= -m_fRadius)
			{
				D3DXVECTOR3 Point = D3DXVECTOR3(pPos->x - (Cross[0].x * fDistance), pPos->y - (Cross[0].y * fDistance), pPos->z - (Cross[0].z * fDistance)); //�ǖʏ�̃v���C���[�̈ʒu
				D3DXVECTOR3 OldPoint = D3DXVECTOR3(posOld.x - (Cross[0].x * fOldDistance), posOld.y - (Cross[0].y * fOldDistance), posOld.z - (Cross[0].z * fOldDistance)); //�ǖʏ��1�t���[���O�̃v���C���[�̈ʒu
				if (Collision(&aPoint[0], Point, OldPoint) == true)
				{
					pPos->x += sinf(rot.y) * (fabsf(fDistance) - m_fRadius);
					if (m_bCharge == false)
					{
						m_fJumpMoveX *= REFLECT;
					}
					/*if (m_move.y > 0.0f && rot.x < 0.0f)
					{
						m_move.x = sinf(-rot.y) * sinf(rot.x) * m_move.y * 4.0f;
					}*/
					return;
				}
			}
			if (fabsf(fHeadDistance) <= m_fRadius || fHeadOldDistance <= -m_fRadius && fHeadDistance >= -m_fRadius)
			{
				D3DXVECTOR3 Point = D3DXVECTOR3(pPos->x - (Cross[0].x * fHeadDistance), (pPos->y + m_fHeight) - (Cross[0].y * fHeadDistance), pPos->z - (Cross[0].z * fHeadDistance)); //�ǖʏ�̃v���C���[�̈ʒu
				D3DXVECTOR3 OldPoint = D3DXVECTOR3(posOld.x - (Cross[0].x * fHeadOldDistance), (posOld.y + m_fHeight) - (Cross[0].y * fHeadOldDistance), posOld.z - (Cross[0].z * fHeadOldDistance)); //�ǖʏ��1�t���[���O�̃v���C���[�̈ʒu
				if (Collision(&aPoint[0], Point, OldPoint) == true)
				{
					pPos->x += sinf(rot.y) * (fabsf(fHeadDistance) - m_fRadius);
					if (m_bCharge == false)
					{
						m_fJumpMoveX *= REFLECT;
					}
					/*if (m_move.y > 0.0f && rot.x < 0.0f)
					{
						m_move.x = sinf(-rot.y) * sinf(rot.x) * m_move.y * 4.0f;
					}*/
					return;
				}
			}
		}
	}
}

void CPlayer::CollisionPlane(D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld)
{
	CScene *pScene;
	pScene = CScene::GetScene(CScene::PRIORITY_PLANE);
	CScene *pSceneNext;
	bool bLand = false;
	bool bCamera = false;
	while (pScene)
	{
		pSceneNext = pScene->GetNext();
		if (pScene->GetObjType() == CScene::OBJECTTYPE_WALL)
		{
			CollisionWall(pPos, posOld, pScene);
		}
		else if (pScene->GetObjType() == CScene::OBJECTTYPE_FIELD && bLand == false)
		{
			bLand = CollisionLand(pPos, posOld, pScene);
		}
		else if (pScene->GetObjType() == CScene::OBJECTTYPE_CAMERAPANEL && bCamera == false)
		{
			bCamera = CollisionCamera(pPos, pScene);
		}
		pScene = pSceneNext;
	}
	m_bLandField = bLand;
}

CPlayer *CPlayer::Create(D3DXVECTOR3 pos, D3DXVECTOR3 rot, const char *aFileName)
{
	CPlayer *pPlayer;
	pPlayer = new CPlayer(PRIORITY_CHARA);
	if (pPlayer != NULL)
	{
		pPlayer->Init(pos, rot, aFileName);
	}
	return pPlayer;
}

void CPlayer::PauseChange(int nAdd)
{
	m_pPauseUI[m_nPauseSelect]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.2f));
	m_nPauseSelect += nAdd;
	if (m_nPauseSelect < 0)
	{
		m_nPauseSelect = 2;
	}
	else if (m_nPauseSelect > 2)
	{
		m_nPauseSelect = 0;
	}
	m_pPauseUI[m_nPauseSelect]->ColorChange(D3DXCOLOR(0.0f, 0.0f, 0.0f, 1.0f));
	CSound::Play(9);
}

void CPlayer::PauseSelect()
{
	switch (m_nPauseSelect)
	{
	case 0:
		CManager::SetPause(false);
		break;
	case 1:
		CFade::SetFade(CManager::MODE_GAME);
		break;
	case 2:
		CFade::SetFade(CManager::MODE_TITLE);
		break;
	default:
		break;
	}
	CSound::Play(7);
}

void CPlayer::EndChange(int nAdd)
{
	m_nPauseSelect += nAdd;
	if (m_nPauseSelect < 0)
	{
		m_nPauseSelect = 1;
	}
	else if (m_nPauseSelect > 1)
	{
		m_nPauseSelect = 0;
	}
	//CSound::Play(CSound::SOUND_LABEL_SELECT);
}

void CPlayer::EndSelect()
{
	switch (m_nPauseSelect)
	{
	case 0:
		CFade::SetFade(CManager::MODE_GAME);
		break;
	case 1:
		CFade::SetFade(CManager::MODE_TITLE);
		break;
	default:
		break;
	}
	//CSound::Play(CSound::SOUND_LABEL_OK);
}

bool CPlayer::Collision(D3DXVECTOR3 aPoint[4], D3DXVECTOR3 Point, D3DXVECTOR3 PointOld)
{
	D3DXVECTOR3 SubVector[12];
	SubVector[0] = D3DXVECTOR3(aPoint[1].x - aPoint[0].x, aPoint[1].y - aPoint[0].y, aPoint[1].z - aPoint[0].z);
	SubVector[1] = D3DXVECTOR3(aPoint[0].x - Point.x, aPoint[0].y - Point.y, aPoint[0].z - Point.z);
	SubVector[2] = D3DXVECTOR3(aPoint[0].x - PointOld.x, aPoint[0].y - PointOld.y, aPoint[0].z - PointOld.z);
	SubVector[3] = D3DXVECTOR3(aPoint[3].x - aPoint[1].x, aPoint[3].y - aPoint[1].y, aPoint[3].z - aPoint[1].z);
	SubVector[4] = D3DXVECTOR3(aPoint[1].x - Point.x, aPoint[1].y - Point.y, aPoint[1].z - Point.z);
	SubVector[5] = D3DXVECTOR3(aPoint[1].x - PointOld.x, aPoint[1].y - PointOld.y, aPoint[1].z - PointOld.z);
	SubVector[6] = D3DXVECTOR3(aPoint[2].x - aPoint[3].x, aPoint[2].y - aPoint[3].y, aPoint[2].z - aPoint[3].z);
	SubVector[7] = D3DXVECTOR3(aPoint[3].x - Point.x, aPoint[3].y - Point.y, aPoint[3].z - Point.z);
	SubVector[8] = D3DXVECTOR3(aPoint[3].x - PointOld.x, aPoint[3].y - PointOld.y, aPoint[3].z - PointOld.z);
	SubVector[9] = D3DXVECTOR3(aPoint[0].x - aPoint[2].x, aPoint[0].y - aPoint[2].y, aPoint[0].z - aPoint[2].z);
	SubVector[10] = D3DXVECTOR3(aPoint[2].x - Point.x, aPoint[2].y - Point.y, aPoint[2].z - Point.z);
	SubVector[11] = D3DXVECTOR3(aPoint[2].x - PointOld.x, aPoint[2].y - PointOld.y, aPoint[2].z - PointOld.z);

	D3DXVECTOR3 CrossVector[8];
	CrossVector[0] = D3DXVECTOR3(SubVector[0].y * SubVector[1].z - SubVector[1].y * SubVector[0].z, SubVector[0].z * SubVector[1].x - SubVector[1].z * SubVector[0].x, SubVector[0].x * SubVector[1].y - SubVector[1].x * SubVector[0].y);
	CrossVector[1] = D3DXVECTOR3(SubVector[0].y * SubVector[2].z - SubVector[2].y * SubVector[0].z, SubVector[0].z * SubVector[2].x - SubVector[2].z * SubVector[0].x, SubVector[0].x * SubVector[2].y - SubVector[2].x * SubVector[0].y);
	CrossVector[2] = D3DXVECTOR3(SubVector[3].y * SubVector[4].z - SubVector[4].y * SubVector[3].z, SubVector[3].z * SubVector[4].x - SubVector[4].z * SubVector[3].x, SubVector[3].x * SubVector[4].y - SubVector[4].x * SubVector[3].y);
	CrossVector[3] = D3DXVECTOR3(SubVector[3].y * SubVector[5].z - SubVector[5].y * SubVector[3].z, SubVector[3].z * SubVector[5].x - SubVector[5].z * SubVector[3].x, SubVector[3].x * SubVector[5].y - SubVector[5].x * SubVector[3].y);
	CrossVector[4] = D3DXVECTOR3(SubVector[6].y * SubVector[7].z - SubVector[7].y * SubVector[6].z, SubVector[6].z * SubVector[7].x - SubVector[7].z * SubVector[6].x, SubVector[6].x * SubVector[7].y - SubVector[7].x * SubVector[6].y);
	CrossVector[5] = D3DXVECTOR3(SubVector[6].y * SubVector[8].z - SubVector[8].y * SubVector[6].z, SubVector[6].z * SubVector[8].x - SubVector[8].z * SubVector[6].x, SubVector[6].x * SubVector[8].y - SubVector[8].x * SubVector[6].y);
	CrossVector[6] = D3DXVECTOR3(SubVector[9].y * SubVector[10].z - SubVector[10].y * SubVector[9].z, SubVector[9].z * SubVector[10].x - SubVector[10].z * SubVector[9].x, SubVector[9].x * SubVector[10].y - SubVector[10].x * SubVector[9].y);
	CrossVector[7] = D3DXVECTOR3(SubVector[9].y * SubVector[11].z - SubVector[11].y * SubVector[9].z, SubVector[9].z * SubVector[11].x - SubVector[11].z * SubVector[9].x, SubVector[9].x * SubVector[11].y - SubVector[11].x * SubVector[9].y);

	float fDot[8];
	//���݈ʒu�̕�
	fDot[0] = CrossVector[0].x * CrossVector[2].x + CrossVector[0].y * CrossVector[2].y + CrossVector[0].z * CrossVector[2].z;
	fDot[1] = CrossVector[2].x * CrossVector[4].x + CrossVector[2].y * CrossVector[4].y + CrossVector[2].z * CrossVector[4].z;
	fDot[2] = CrossVector[4].x * CrossVector[6].x + CrossVector[4].y * CrossVector[6].y + CrossVector[4].z * CrossVector[6].z;
	fDot[3] = CrossVector[6].x * CrossVector[0].x + CrossVector[6].y * CrossVector[0].y + CrossVector[6].z * CrossVector[0].z;

	//�ߋ��ʒu�̕�
	fDot[4] = CrossVector[1].x * CrossVector[2].x + CrossVector[1].y * CrossVector[2].y + CrossVector[1].z * CrossVector[2].z;
	fDot[5] = CrossVector[3].x * CrossVector[4].x + CrossVector[3].y * CrossVector[4].y + CrossVector[3].z * CrossVector[4].z;
	fDot[6] = CrossVector[5].x * CrossVector[6].x + CrossVector[5].y * CrossVector[6].y + CrossVector[5].z * CrossVector[6].z;
	fDot[7] = CrossVector[7].x * CrossVector[0].x + CrossVector[7].y * CrossVector[0].y + CrossVector[7].z * CrossVector[0].z;
	if ((fDot[0] > 0.0f && fDot[1] > 0.0f && fDot[2] > 0.0f && fDot[3] > 0.0f) && (fDot[4] > 0.0f && fDot[5] > 0.0f && fDot[6] > 0.0f && fDot[7] > 0.0f))
	{
		return true;
	}
	return false;
}

bool CPlayer::CollisionBlock(D3DXVECTOR3 aSquare[4], D3DXVECTOR3 *pPos, D3DXVECTOR3 posOld, bool *bLand, D3DXVECTOR3 *Nextpos)
{
	D3DXVECTOR3 SubPoint[2];
	D3DXVECTOR3 Cross[5];
	float fLength;
	float fDistance;
	float fOldDistance;
	float fHeadDistance;
	float fHeadOldDistance;

	SubPoint[0] = aSquare[2] - aSquare[1];
	SubPoint[1] = aSquare[1] - aSquare[0];
	D3DXVec3Cross(&Cross[0], &SubPoint[0], &SubPoint[1]);
	if (fabsf(Cross[0].x) < 0.01f)
	{
		Cross[0].x = 0.0f;
	}
	if (fabsf(Cross[0].y) < 0.01f)
	{
		Cross[0].y = 0.0f;
	}
	if (fabsf(Cross[0].z) < 0.01f)
	{
		Cross[0].z = 0.0f;
	}
	fLength = powf((Cross[0].x * Cross[0].x) + (Cross[0].y * Cross[0].y) + (Cross[0].z * Cross[0].z), 0.5f);
	Cross[0].x /= fLength;
	Cross[0].y /= fLength;
	Cross[0].z /= fLength; //�����܂Ŗ@���x�N�g�� �l���傫�������������Ă��� X�͍��Ńv���X�E�Ń}�C�i�X Y�͉��Ń}�C�i�X��Ńv���X

	Cross[1] = *pPos - aSquare[0]; //�v���C���[��pos�ƕǂ̔C�ӂ̓_�̃x�N�g��
	fDistance = (Cross[0].x * Cross[1].x) + (Cross[0].y * Cross[1].y) + (Cross[0].z * Cross[1].z);	//�����ƒn�ʂƂ̋��� �v���X�̎��A�\���ɂ���
	Cross[2] = posOld - aSquare[0]; //�v���C���[��Oldpos�ƕǂ̔C�ӂ̓_�̃x�N�g��
	fOldDistance = (Cross[0].x * Cross[2].x) + (Cross[0].y * Cross[2].y) + (Cross[0].z * Cross[2].z);	//�ߋ��ʒu�����ƒn�ʂƂ̋��� �v���X�̎��A�\���ɂ���

	if (Cross[0].y < 0.0f)	//���������\�������Ă���
	{
		if ((fabsf(fDistance) <= 3.0f || fOldDistance <= 2.0f && fDistance >= -2.0f) && m_move.y <= 0.0f)
		{
			D3DXVECTOR3 aWideSquare[4];
			aWideSquare[0] = D3DXVECTOR3(aSquare[0].x - m_fRadius, aSquare[0].y, aSquare[0].z + m_fRadius);
			aWideSquare[1] = D3DXVECTOR3(aSquare[1].x + m_fRadius, aSquare[1].y, aSquare[1].z + m_fRadius);
			aWideSquare[2] = D3DXVECTOR3(aSquare[2].x - m_fRadius, aSquare[2].y, aSquare[2].z - m_fRadius);
			aWideSquare[3] = D3DXVECTOR3(aSquare[3].x + m_fRadius, aSquare[3].y, aSquare[3].z - m_fRadius);
			D3DXVECTOR3 SquarePos;
			SquarePos.x = pPos->x - Cross[0].x * fDistance;
			SquarePos.y = pPos->y - Cross[0].y * fDistance;
			SquarePos.z = pPos->z - Cross[0].z * fDistance;
			D3DXVECTOR3 SquareOldPos;
			SquareOldPos.x = posOld.x - Cross[0].x * fOldDistance;
			SquareOldPos.y = posOld.y - Cross[0].y * fOldDistance;
			SquareOldPos.z = posOld.z - Cross[0].z * fOldDistance;
			if (Collision(&aWideSquare[0], SquarePos, SquareOldPos) == true)
			{
				if (Cross[0].y < -0.7f) //��̌X��45�x�܂�
				{
					D3DXVECTOR3 ShadowRot = D3DXVECTOR3(Cross[0].z, 0.0f, Cross[0].x * 1.11f);
					m_move.y = 0.0f;
					pPos->y += fDistance;
					m_pShadow->MoveY(D3DXVECTOR3(pPos->x, pPos->y + 3.0f, pPos->z), ShadowRot);
					if (m_bLanding == false && m_bLandObject == false)
					{
						MotionChange(MOTIONTYPE_LANDING);
						m_bLandObject = true;
						m_bCharge = false;
						m_fJumpMoveX = 0.0f;
					}
					*bLand = true;
					return true;
				}
				else
				{
					pPos->x -= fDistance;
					if (m_bCharge == false)
					{
						m_fJumpMoveX *= REFLECT;
					}
					return false;
				}
			}
		}
	}
	else if(Cross[0].y > 0.0f) //�ʂ����������Ă���
	{
		Cross[3] = *pPos - aSquare[0]; //�v���C���[��pos�ƕǂ̔C�ӂ̓_�̃x�N�g��
		Cross[3].y += m_fHeight;
		fHeadDistance = (Cross[0].x * Cross[3].x) + (Cross[0].y * Cross[3].y) + (Cross[0].z * Cross[3].z);	//���ƒn�ʂƂ̋��� �v���X�̎��A�\���ɂ���
		Cross[4] = posOld - aSquare[0]; //�v���C���[��Oldpos�ƕǂ̔C�ӂ̓_�̃x�N�g��
		Cross[4].y += m_fHeight;
		fHeadOldDistance = (Cross[0].x * Cross[4].x) + (Cross[0].y * Cross[4].y) + (Cross[0].z * Cross[4].z);	//���ƒn�ʂƂ̋��� �v���X�̎��A�\���ɂ���
		if (fabsf(fHeadDistance) < 2.0f || fHeadOldDistance <= 2.0f && fHeadDistance >= -2.0f)
		{
			D3DXVECTOR3 aWideSquare[4];
			aWideSquare[0] = D3DXVECTOR3(aSquare[0].x + m_fRadius * 0.6f, aSquare[0].y, aSquare[0].z + m_fRadius * 0.6f);
			aWideSquare[1] = D3DXVECTOR3(aSquare[1].x - m_fRadius * 0.6f, aSquare[1].y, aSquare[1].z + m_fRadius * 0.6f);
			aWideSquare[2] = D3DXVECTOR3(aSquare[2].x + m_fRadius * 0.6f, aSquare[2].y, aSquare[2].z - m_fRadius * 0.6f);
			aWideSquare[3] = D3DXVECTOR3(aSquare[3].x - m_fRadius * 0.6f, aSquare[3].y, aSquare[3].z - m_fRadius * 0.6f);
			D3DXVECTOR3 HeadPos = D3DXVECTOR3(pPos->x - Cross[0].x * fHeadDistance, (pPos->y + m_fHeight) - Cross[0].y * fHeadDistance, pPos->z - Cross[0].z * fHeadDistance);
			D3DXVECTOR3 HeadOldPos = D3DXVECTOR3(posOld.x - Cross[0].x * fHeadOldDistance, (posOld.y + m_fHeight) - Cross[0].y * fHeadOldDistance, posOld.z - Cross[0].z * fHeadOldDistance);
			if (Collision(&aWideSquare[0], HeadPos, HeadOldPos) == true)
			{
				Nextpos->y = pPos->y - fabsf(fHeadDistance) - 2.2f;
				m_move.y *= -0.4f;
				return false;
			}
		}
	}

	D3DXVECTOR3 VtxMax = D3DXVECTOR3(-1000000.0f, -1000000.0f, -1000000.0f);
	D3DXVECTOR3 VtxMin = D3DXVECTOR3(1000000.0f, 1000000.0f, 1000000.0f);
	int nCntPoint;
	for (nCntPoint = 0; nCntPoint < 4; nCntPoint++)
	{
		if (VtxMax.x < aSquare[nCntPoint].x)
		{
			VtxMax.x = aSquare[nCntPoint].x; //4���_�̂����A��ԉE�̓_
		}
		else if (VtxMin.x > aSquare[nCntPoint].x)
		{
			VtxMin.x = aSquare[nCntPoint].x; //4���_�̂����A��ԍ��̓_
		}
		if (VtxMax.y < aSquare[nCntPoint].y)
		{
			VtxMax.y = aSquare[nCntPoint].y; //4���_�̂����A��ԏ�̓_
		}
		else if (VtxMin.y > aSquare[nCntPoint].y)
		{
			VtxMin.y = aSquare[nCntPoint].y; //4���_�̂����A��ԉ��̓_
		}
		if (VtxMax.z < aSquare[nCntPoint].z)
		{
			VtxMax.z = aSquare[nCntPoint].z; //4���_�̂����A��ԉ��̓_
		}
		else if (VtxMin.z > aSquare[nCntPoint].z)
		{
			VtxMin.z = aSquare[nCntPoint].z; //4���_�̂����A��Ԏ�O�̓_
		}
	}

	//��G�c�ɓ������Ă���
	if (VtxMin.y < pPos->y + m_fHeight && pPos->y < VtxMax.y && VtxMin.x < pPos->x + m_fRadius && pPos->x - m_fRadius < VtxMax.x/* && (posOld.x + m_fRadius <= VtxMin.x || VtxMax.x <= posOld.x - m_fRadius)*/)
	{
		//���Ȃ��ʂ�������
		if (fabsf(Cross[0].y) < 0.7f)
		{
			float fD = aSquare[0].x * Cross[0].x + aSquare[0].y * Cross[0].y + aSquare[0].z * Cross[0].z; //���ʕ�������D
			D3DXVECTOR3 fDCross = Cross[0] * fD;	//���ʂ̓_P�̈ʒu
			D3DXVECTOR3 LeftPosD = D3DXVECTOR3(fDCross.x - (pPos->x - m_fRadius), fDCross.y - pPos->y				, fDCross.z - pPos->z);
			D3DXVECTOR3 LeftHeadD = D3DXVECTOR3(fDCross.x - (pPos->x - m_fRadius), fDCross.y - (pPos->y + m_fHeight), fDCross.z - pPos->z);
			D3DXVECTOR3 RightPosD = D3DXVECTOR3(fDCross.x - (pPos->x + m_fRadius), fDCross.y - pPos->y				, fDCross.z - pPos->z);
			D3DXVECTOR3 RightHeadD = D3DXVECTOR3(fDCross.x - (pPos->x + m_fRadius), fDCross.y - (pPos->y + m_fHeight), fDCross.z - pPos->z);
			float fDotLeftPosD = LeftPosD.x * Cross[0].x + LeftPosD.y * Cross[0].y + LeftPosD.z * Cross[0].z;
			float fDotLeftHeadD = LeftHeadD.x * Cross[0].x + LeftHeadD.y * Cross[0].y + LeftHeadD.z * Cross[0].z;
			float fDotRightPosD = RightPosD.x * Cross[0].x + RightPosD.y * Cross[0].y + RightPosD.z * Cross[0].z;
			float fDotRightHeadD = RightHeadD.x * Cross[0].x + RightHeadD.y * Cross[0].y + RightHeadD.z * Cross[0].z;
			//if (fDotLeftPosD > 0.0f && fDotLeftHeadD < 0.0f || fDotLeftPosD < 0.0f && fDotLeftHeadD > 0.0f) //���ʂƒ���(�����������瓪)�Ƃ̓����蔻��
			//{
			//	D3DXVECTOR3 PlayerVector = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);	//����pos - pos
			//	float fRatio = fabsf(fDotLeftPosD) / (fabsf(fDotLeftHeadD) + fabsf(fDotLeftPosD)); //���ʂ�2�_�̌�_����̒����̔䗦
			//	D3DXVECTOR3 PlanePos = D3DXVECTOR3((pPos->x - m_fRadius) + PlayerVector.x * fRatio, pPos->y + PlayerVector.y * fRatio, pPos->z + PlayerVector.z * fRatio);			//���ʂƒ����̌�_
			//	D3DXVECTOR3 PlanePosOld = D3DXVECTOR3((posOld.x - m_fRadius) + PlayerVector.x * fRatio, posOld.y + PlayerVector.y * fRatio, posOld.z + PlayerVector.z * fRatio);	//���ʂƒ����̌�_
			//	if (Collision(&aSquare[0], PlanePos, PlanePosOld) == true)
			//	{
			//		Nextpos->x =  VtxMax.x + m_fRadius; //�E���ɉ����o��
			//		m_fJumpMoveX *= -0.8f;
			//		return false;
			//	}
			//}
			//else if (fDotRightPosD > 0.0f && fDotRightHeadD < 0.0f || fDotRightPosD < 0.0f && fDotRightHeadD > 0.0f) //���ʂƒ���(�E���������瓪)�Ƃ̓����蔻��
			//{
			//	D3DXVECTOR3 PlayerVector = D3DXVECTOR3(0.0f, m_fHeight, 0.0f);	//����pos - pos
			//	float fRatio = fabsf(fDotRightPosD) / (fabsf(fDotRightHeadD) + fabsf(fDotRightPosD)); //���ʂ�2�_�̌�_����̒����̔䗦
			//	D3DXVECTOR3 PlanePos = D3DXVECTOR3((pPos->x + m_fRadius) + PlayerVector.x * fRatio, pPos->y + PlayerVector.y * fRatio, pPos->z + PlayerVector.z * fRatio);			//���ʂƒ����̌�_
			//	D3DXVECTOR3 PlanePosOld = D3DXVECTOR3((posOld.x + m_fRadius) + PlayerVector.x * fRatio, posOld.y + PlayerVector.y * fRatio, posOld.z + PlayerVector.z * fRatio);	//���ʂƒ����̌�_
			//	if (Collision(&aSquare[0], PlanePos, PlanePosOld) == true)
			//	{
			//		Nextpos->x = VtxMin.x - m_fRadius; //�����ɉ����o��
			//		m_fJumpMoveX *= -0.8f;
			//		return false;
			//	}
			//}
			if (fDotLeftPosD > 0.0f && fDotRightPosD < 0.0f || fDotLeftPosD < 0.0f && fDotRightPosD > 0.0f) //���ʂƒ���(����������E)�Ƃ̓����蔻��
			{
				D3DXVECTOR3 PlayerVector = D3DXVECTOR3(-m_fRadius * 2.0f, 0.0f, 0.0f);	//pos�� - pos�E
				float fRatio = fabsf(fDotLeftPosD) / (fabsf(fDotRightPosD) + fabsf(fDotLeftPosD)); //���ʂ�2�_�̌�_����̒����̔䗦
				D3DXVECTOR3 PlanePos = D3DXVECTOR3(pPos->x + PlayerVector.x * fRatio, pPos->y + PlayerVector.y * fRatio, pPos->z + PlayerVector.z * fRatio);		//���ʂƒ����̌�_
				D3DXVECTOR3 PlanePosOld = D3DXVECTOR3(posOld.x + PlayerVector.x * fRatio, posOld.y + PlayerVector.y * fRatio, posOld.z + PlayerVector.z * fRatio);	//���ʂƒ����̌�_
				if (Collision(&aSquare[0], PlanePos, PlanePosOld) == true)
				{
					if (posOld.x < VtxMin.x)
					{
						Nextpos->x = VtxMin.x - m_fRadius; //�����ɉ����o��
						if (m_fJumpMoveX > 0.0f)
						{
							m_fJumpMoveX *= REFLECT;
						}
						return false;
					}
					if (VtxMax.x < posOld.x)
					{
						Nextpos->x = VtxMax.x + m_fRadius; //�E���ɉ����o��
						if (m_fJumpMoveX < 0.0f)
						{
							m_fJumpMoveX *= REFLECT;
						}
						return false;
					}
				}
			}
			else if (fDotLeftHeadD > 0.0f && fDotRightHeadD < 0.0f || fDotLeftHeadD < 0.0f && fDotRightHeadD > 0.0f) //���ʂƒ���(��������E)�Ƃ̓����蔻��
			{
				D3DXVECTOR3 PlayerVector = D3DXVECTOR3(-m_fRadius * 2.0f, 0.0f, 0.0f);	//pos�� - pos�E
				float fRatio = fabsf(fDotLeftHeadD) / (fabsf(fDotRightHeadD) + fabsf(fDotLeftHeadD)); //���ʂ�2�_�̌�_����̒����̔䗦
				D3DXVECTOR3 PlanePos = D3DXVECTOR3(pPos->x + PlayerVector.x * fRatio, pPos->y + PlayerVector.y * fRatio, pPos->z + PlayerVector.z * fRatio);			//���ʂƒ����̌�_
				D3DXVECTOR3 PlanePosOld = D3DXVECTOR3(posOld.x + PlayerVector.x * fRatio, posOld.y + PlayerVector.y * fRatio, posOld.z + PlayerVector.z * fRatio);	//���ʂƒ����̌�_
				if (Collision(&aSquare[0], PlanePos, PlanePosOld) == true)
				{
					if (posOld.x < VtxMin.x)
					{
						Nextpos->x = VtxMin.x - m_fRadius; //�����ɉ����o��
						if (m_fJumpMoveX > 0.0f)
						{
							m_fJumpMoveX *= REFLECT;
						}
						return false;
					}
					if (VtxMax.x < posOld.x)
					{
						Nextpos->x = VtxMax.x + m_fRadius; //�E���ɉ����o��
						if (m_fJumpMoveX < 0.0f)
						{
							m_fJumpMoveX *= REFLECT;
						}
						return false;
					}
				}
			}

		}
		else //����ʂ̂Ƃ�
		{
			if (posOld.x < VtxMin.x)		//�ߋ��ʒu�����ʂ̍�����荶�ɂ������Ƃ�
			{
				Nextpos->x = VtxMin.x - m_fRadius; //�����ɉ����o��
				if (m_fJumpMoveX > 0.0f)
				{
					m_fJumpMoveX *= REFLECT;
				}
				return false;
			}
			else if (VtxMax.x < posOld.x)	//�ߋ��ʒu�����ʂ̉E�����E�ɂ������Ƃ�
			{
				Nextpos->x = VtxMax.x + m_fRadius; //�E���ɉ����o��
				if (m_fJumpMoveX < 0.0f)
				{
					m_fJumpMoveX *= REFLECT;
				}
				return false;
			}
		}
	}
	return false;
}

bool CPlayer::CollisionCamera(D3DXVECTOR3 *pPos, CScene *pScene)
{
	D3DXVECTOR3 pos = pScene->GetPos();
	D3DXVECTOR3 VtxMax = pScene->GetVtxMax();
	D3DXVECTOR3 VtxMin = pScene->GetVtxMin();
	D3DXVECTOR2 aPoint[4]; //�X�y�[�X��4���_

	int nCntPos;
	for (nCntPos = 0; nCntPos < 4; nCntPos++)
	{
		switch (nCntPos)
		{
		case 0:
			//�ʒu�𔽉f
			aPoint[nCntPos] = D3DXVECTOR2(pos.x + VtxMin.x, pos.y + VtxMax.y);
			break;
		case 1:
			//�ʒu�𔽉f
			aPoint[nCntPos] = D3DXVECTOR2(pos.x + VtxMax.x, pos.y + VtxMax.y);
			break;
		case 2:
			//�ʒu�𔽉f
			aPoint[nCntPos] = D3DXVECTOR2(pos.x + VtxMin.x, pos.y + VtxMin.y);
			break;
		case 3:
			//�ʒu�𔽉f
			aPoint[nCntPos] = D3DXVECTOR2(pos.x + VtxMax.x, pos.y + VtxMin.y);
			break;
		default:
			break;
		}
	}
	if (aPoint[0].x <= pPos->x && pPos->x <= aPoint[1].x && aPoint[2].y <= pPos->y && pPos->y <= aPoint[0].y)
	{
		m_aCamera[0] = aPoint[2];
		m_aCamera[1] = aPoint[1];
		return true;
	}
	return false;
}

void CPlayer::ShadowObject(D3DXVECTOR3 aSquare[4], D3DXVECTOR3 *pPos)
{
	D3DXVECTOR3 PlayerVector = D3DXVECTOR3(0.0f, m_fShadowPosY - pPos->y, 0.0f); //�v���C���[���炸���Ɖ��ւ̃x�N�g��
	D3DXVECTOR3 BlockVector = aSquare[1] - aSquare[0]; //�l�p�`��1�ӂ̃x�N�g��
	D3DXVECTOR3 Cross[2];
	float fLength;
	fLength = powf((PlayerVector.x * PlayerVector.x) + (PlayerVector.y * PlayerVector.y) + (PlayerVector.z * PlayerVector.z), 0.5f);
	Cross[0] = PlayerVector / fLength; //�v���C���[�̃x�N�g���̒P�ʃx�N�g��
	fLength = powf((BlockVector.x * BlockVector.x) + (BlockVector.y * BlockVector.y) + (BlockVector.z * BlockVector.z), 0.5f);
	Cross[1] = BlockVector / fLength; //�l�p�`1�ӂ̃x�N�g���̒P�ʃx�N�g��
	float fWork1 = Cross[0].x * Cross[1].x + Cross[0].y * Cross[1].y + Cross[0].z * Cross[1].z; //��2�̒P�ʃx�N�g���̓���
	float fWork2 = 1.0f - fWork1 * fWork1;
	if (fWork2 == 0.0f) //0���ƕ��s
	{
		return;
	}
	D3DXVECTOR3 PlayerBlock = aSquare[0] - *pPos; //�v���C���[�Ǝl�p�`��1�_�Ƃ̃x�N�g��
	float fD1 = ((PlayerBlock.x * Cross[0].x + PlayerBlock.y * Cross[0].y + PlayerBlock.z * Cross[0].z) - fWork1 * (PlayerBlock.x * Cross[1].x + PlayerBlock.y * Cross[1].y + PlayerBlock.z * Cross[1].z)) / fWork2;
	float fD2 = (fWork1 * (PlayerBlock.x * Cross[0].x + PlayerBlock.y * Cross[0].y + PlayerBlock.z * Cross[0].z) - (PlayerBlock.x * Cross[1].x + PlayerBlock.y * Cross[1].y + PlayerBlock.z * Cross[1].z)) / fWork2;

	D3DXVECTOR3 MostVertex[2];
	//�v���C���[�x�N�g����̍ŋߓ_
	MostVertex[0].x = pPos->x + fD1 * Cross[0].x;
	MostVertex[0].y = pPos->y + fD1 * Cross[0].y;
	MostVertex[0].z = pPos->z + fD1 * Cross[0].z;

	//1�ӂ̍ŋߓ_
	MostVertex[1].x = aSquare[0].x + fD2 * Cross[1].x;
	MostVertex[1].y = aSquare[0].y + fD2 * Cross[1].y;
	MostVertex[1].z = 0.0f;

	//�������Ă��邩�̔���
	if (powf(MostVertex[1].x - MostVertex[0].x, 2.0f) + powf(MostVertex[1].y - MostVertex[0].y, 2.0f) + powf(MostVertex[1].z - MostVertex[0].z, 2.0f) < 0.001f)
	{
		//���X�̉e�̏ꏊ��荂���A�v���C���[���Ⴂ
		if (m_fShadowPosY < MostVertex[0].y && MostVertex[0].y < pPos->y)
		{
			D3DXVECTOR3 aWideSquare[4];
			aWideSquare[0] = D3DXVECTOR3(aSquare[0].x - m_fRadius, aSquare[0].y, aSquare[0].z + m_fRadius);
			aWideSquare[1] = D3DXVECTOR3(aSquare[1].x + m_fRadius, aSquare[1].y, aSquare[1].z + m_fRadius);
			aWideSquare[2] = D3DXVECTOR3(aSquare[2].x - m_fRadius, aSquare[2].y, aSquare[2].z - m_fRadius);
			aWideSquare[3] = D3DXVECTOR3(aSquare[3].x + m_fRadius, aSquare[3].y, aSquare[3].z - m_fRadius);
			//�l�p�`��1�_�̓����蔻��
			if (Collision(&aWideSquare[0], MostVertex[0], MostVertex[1]) == true)
			{
				D3DXVECTOR3 SubPoint[2];
				SubPoint[0] = aSquare[2] - aSquare[1];
				SubPoint[1] = aSquare[1] - aSquare[0];
				D3DXVec3Cross(&Cross[0], &SubPoint[0], &SubPoint[1]);
				fLength = powf((Cross[0].x * Cross[0].x) + (Cross[0].y * Cross[0].y) + (Cross[0].z * Cross[0].z), 0.5f); //�P�ʃx�N�g��
				Cross[0].x /= fLength;
				Cross[0].y /= fLength;
				Cross[0].z /= fLength; //�����܂Ŗ@���x�N�g�� �l���傫�������������Ă���
				m_fShadowPosY = MostVertex[0].y + 1.0f;
				m_ShadowRot.x = Cross[0].z;
				m_ShadowRot.y = Cross[0].y;
				m_ShadowRot.z = Cross[0].x * 1.11f;
			}
		}
	}
}