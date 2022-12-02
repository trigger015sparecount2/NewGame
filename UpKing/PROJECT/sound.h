//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
// ƒTƒEƒ“ƒhˆ— [sound.h]
// Author :Oãq¢
//{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{{
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"

#define MAX_SOUND (64)

class CSound
{
public:
	CSound();
	~CSound();

	static HRESULT Init(HWND hWnd);
	static void Uninit();
	static HRESULT Play(int nlabel);
	static void Stop(int nlabel);
	static void Stop();
	static HRESULT SetParamData(char aSoundName[64], int nLoop, int nLabel);
	static void SetSoundNum(int nSound);

	//‰¹—Ê’²®
	static void ControlVoice(int nlabel, float fVolume)
	{
		m_apSourceVoice[nlabel]->SetVolume(fVolume);
	}

private:
	typedef struct
	{
		char filename[64];		// ƒtƒ@ƒCƒ‹–¼
		int nCntLoop;		// ƒ‹[ƒvƒJƒEƒ“ƒg 0‚Åƒ‹[ƒv‚µ‚È‚¢@-1‚Å–³ŒÀƒ‹[ƒv
	} PARAM;
	static HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD *pChunkSize, DWORD *pChunkDataPosition);
	static HRESULT ReadChunkData(HANDLE hFile, void *pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
	//static HRESULT LoadSoundFile(const char *aFileName);
	static IXAudio2 *m_pXAudio2;									// XAudio2ƒIƒuƒWƒFƒNƒg‚Ö‚ÌƒCƒ“ƒ^[ƒtƒFƒCƒX
	static IXAudio2MasteringVoice *m_pMasteringVoice;				// ƒ}ƒXƒ^[ƒ{ƒCƒX
	static IXAudio2SourceVoice *m_apSourceVoice[MAX_SOUND];	// ƒ\[ƒXƒ{ƒCƒX
	static BYTE *m_apDataAudio[MAX_SOUND];					// ƒI[ƒfƒBƒIƒf[ƒ^
	static DWORD m_aSizeAudio[MAX_SOUND];						// ƒI[ƒfƒBƒIƒf[ƒ^ƒTƒCƒY
	static int m_nNumSound;

	// Še‰¹‘fŞ‚Ìƒpƒ‰ƒ[ƒ^
	static PARAM m_aParam[MAX_SOUND];
};
#endif
