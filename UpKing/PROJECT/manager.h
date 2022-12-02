//=============================================================================
//
// �}�l�[�W�� [Manager.h]
// Author : �O��q��
//
//=============================================================================
#ifndef _MANAGER_H_
#define _MANAGER_H_
#include "main.h"
//=============================================================================
//�}�N��
//=============================================================================

//�O���錾
class CRenderer;
class CKeyboard;
class CMouse;
class CGamePad;

class CTitle;
class CGame;
class CResult;
//class CTutorial;
//class CGameOver;

class CManager
{
public:

	typedef enum
	{
		MODE_TITLE,
		MODE_GAME,
		MODE_RESULT,
		//MODE_TUTORIAL,
		//MODE_GAMEOVER,
		MODE_MAX,
	}MODE;

	CManager();
	~CManager();

	HRESULT Init(HINSTANCE hInstance, HWND hWnd, bool bWindow);
	void Uninit();
	void Update();
	void Draw();
	static CRenderer *GetRenderer();
	static CKeyboard *GetKeyboard();
	static CMouse *GetMouse();
	static CGamePad *GetGamepad();

	//���[�h�i�[�E�擾
	static void SetMode(MODE mode);
	static MODE GetMode() { return m_aMode; }

	//�Q�[���I���̃t���O�i�[�E�擾
	static void SetGameEnd(bool GameEnd) { m_bGameEnd = GameEnd; }
	static bool GetGameEnd() { return m_bGameEnd; }


	//���ꂼ��̃��[�h�n�����
	static CTitle *GetTitle() { return m_pTitle; }
	static CGame *GetGame() { return m_pGame; }
	static CResult *GetResult() { return m_pResult; }
	/*static CTutorial *GetTutorial() { return m_pTutorial; }
	static CGameOver *GetGameOver() { return m_pGameOver; }*/

	//�Q�[���N���A�֌W
	static bool GetGameClear() { return m_bGameClear; }
	static void SetGameClear(bool Set) { m_bGameClear = Set; }

	//Pause�����ǂ������Ƃ�����
	static void SetPause(bool bFlag) { m_bPause = bFlag; }
	static bool GetPause() { return m_bPause; };

	//�J�E���g�_�E���֘A
	static void SetCountdown(bool bCountdown) { m_bCountdown = bCountdown; }
	static bool GetCountdown() { return m_bCountdown; }

	//�N���A��̃X�g�b�v
	static void SetEnd(bool bFlag) { m_bEnd = bFlag; }
	static bool GetEnd() { return m_bEnd; };

private:
	static CRenderer *m_pRenderer;
	static CKeyboard *m_pKeyboard;
	static CMouse *m_pMouse;
	static CGamePad *m_pGamepad;

	static int m_nTime;		//�Q�[���I����̃X�R�A���ꎞ�I�Ɋi�[
	static bool m_bGameEnd;		//�Q�[���I��������

	static MODE m_aMode;		//���݂̃��[�h

								//�n�����
	static CTitle *m_pTitle;
	static CGame *m_pGame;
	static CResult *m_pResult;
	/*static CTutorial *m_pTutorial;
	static CGameOver *m_pGameOver;*/

	static bool m_bGameClear;	//�Q�[���N���A�������ǂ���
	static bool m_bPause;		//Pause�����ǂ���
	static bool m_bEnd;
	static bool m_bCountdown;

};

#endif