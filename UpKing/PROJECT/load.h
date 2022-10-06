//=============================================================================
//
// ロード処理 [load.h]
// Author : 三上航世
//
//=============================================================================
#ifndef _LOAD_H_
#define _LOAD_H_

#include "main.h"

class CLoad
{
public:
	CLoad();
	~CLoad();
	static void Load(const char *aFileName);
	static void SoundLoad(const char *aFileName);
	static void RankLoad(const char *aFileName);
private:
};

#endif