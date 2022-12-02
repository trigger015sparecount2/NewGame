//=============================================================================
//
// ÉçÅ[Éhèàóù [load.h]
// Author : éOè„çqê¢
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