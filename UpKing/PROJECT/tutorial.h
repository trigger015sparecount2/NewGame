//=============================================================================
//
// チュートリアル処理 [tutorial.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _TUTORIAL_H_
#define _TUTORIAL_H_

#include "main.h"
#include "plane.h"

class CTutorial : public CPlane
{
public:
	//動き方の構造体
	typedef struct
	{
		D3DXVECTOR2 move;
		int nMaxTime;
	} Pattern;
	CTutorial(PRIORITY Priority);
	~CTutorial();
	HRESULT Init(D3DXVECTOR3 size, D3DXVECTOR3 pos, CTutorial::Pattern Pattern[6], int nLoop, int nMaxPattern);
	void Uninit();
	void Update();
	void Draw();
	OBJTYPE GetObjType() { return OBJECTTYPE_TUTORIAL; }

	static CTutorial *Create(D3DXVECTOR3 size, D3DXVECTOR3 pos, CTutorial::Pattern Pattern[6], int nLoop, int nMaxPattern, int nTex);

private:
	D3DXVECTOR3 m_Move;
	int m_nLoop;
	int m_nTime;
	int m_nStopTime;
	int m_nCntPattern;
	int m_nMaxPattern;
	Pattern m_Pattern[6];
};

#endif