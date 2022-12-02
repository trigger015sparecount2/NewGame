//---------------------------
//Author:三上航世
//ロード処理(load.cpp)
//---------------------------
#include "load.h"
#include "manager.h"
#include "renderer.h"
#include "player.h"
#include "field.h"
#include "light.h"
#include "fade.h"
#include "object.h"
#include "camera.h"
#include "scene.h"
#include "wall.h"
#include "player.h"
#include "bg.h"
#include "camerazone.h"
#include "tutorial.h"
#include "sound.h"

CLoad::CLoad()
{

}

CLoad::~CLoad(void)
{

}

void CLoad::Load(const char *aFileName)
{
	FILE *pFile;
	pFile = fopen(aFileName, "r");
	char aFile[256];
	char aPlayerFile[256];
	int nNumModel;
	int nCntModel = 0;
	bool bField = false;
	bool bWall = false;
	bool bCamera = false;
	bool bModel = false;
	bool bLight = false;
	bool bPlayer = false;
	bool bFieldTilt = false;
	bool bTutorial = false;
	bool bPattern = false;
	bool bMove = false;
	D3DXVECTOR3 pos;
	D3DXVECTOR3 rot;
	int nTex;
	int nBlock[2];
	D3DXVECTOR3 size;
	D3DXVECTOR3 move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	int nMoveTime = 0;
	int nType;
	CScene::COLLISION collision;
	int nCollision;
	int nCntLight = 0;
	int nPattern;
	int nCntPattern;
	int nMaxPattern;
	int nLoop;
	int nSound;
	D3DXCOLOR col;
	D3DXVECTOR3 vec;
	D3DXVECTOR2 Tex;
	D3DXVECTOR3 posV;
	D3DXVECTOR3 posR;
	bool bSky = false;
	float fScroll;
	CTutorial::Pattern aPattern[6];
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
			if (strcmp(&aFile[0], "NUM_MODEL") == 0) //モデル数
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &nNumModel);
			}
			if (strcmp(&aFile[0], "SOUND") == 0) //流す曲（soundファイルと連動してる）
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &nSound);
				if (nSound != -1)
				{
					CSound::Play(nSound);
				}
			}
			if (strcmp(&aFile[0], "MODEL_FILENAME") == 0) //モデル名
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%s", &aFile[0]);
				CObject::Load(nCntModel, aFile);
				nCntModel++;
			}
			if (strcmp(&aFile[0], "FIELDSET") == 0) //地面
			{
				bField = true;
			}
			if (strcmp(&aFile[0], "END_FIELDSET") == 0) //地面
			{
				size.x = Tex.x * nBlock[0];
				size.z = Tex.y * nBlock[1];
				//size.y = -sinf(rot.x) * size.z;
				size.y = 0.0f;
				CField::Create(size, pos, rot, nTex, Tex, bFieldTilt);
				bField = false;
			}
			if (strcmp(&aFile[0], "WALLSET") == 0) //壁
			{
				bWall = true;
			}
			if (strcmp(&aFile[0], "END_WALLSET") == 0) //壁
			{
				size.x = Tex.x * nBlock[0];
				size.y = Tex.y * nBlock[1];
				size.z = 0.0f;
				CWall::Create(size, pos, nTex, Tex, rot);
				bWall = false;
			}
			if (strcmp(&aFile[0], "CAMERAZONESET") == 0) //カメラゾーン
			{
				size.z = 0.0f;
				bCamera = true;
			}
			if (strcmp(&aFile[0], "END_CAMERAZONESET") == 0) //カメラゾーン
			{
				CCameraZone::Create(size, pos);
				bCamera = false;
			}
			if (strcmp(&aFile[0], "TUTORIALSET") == 0) //チュートリアル
			{
				size.z = 0.0f;
				nMaxPattern = 0;
				bTutorial = true;
			}
			if (strcmp(&aFile[0], "END_TUTORIALSET") == 0) //チュートリアル
			{
				CTutorial::Create(size, pos, &aPattern[0], nLoop, nMaxPattern, nTex);
				bTutorial = false;
			}
			if (strcmp(&aFile[0], "MODELSET") == 0) //オブジェクト
			{
				bModel = true;
			}
			if (strcmp(&aFile[0], "END_MODELSET") == 0) //オブジェクト
			{
				CObject::Create(pos, nType, collision, rot);
				nMoveTime = 0;
				move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				bModel = false;
			}
			if (strcmp(&aFile[0], "LIGHTSET") == 0) //ライト
			{
				bLight = true;
			}
			if (strcmp(&aFile[0], "END_LIGHTSET") == 0) //ライト
			{
				CLight::Create(col, vec, nCntLight);
				nCntLight++;
				bLight = false;
			}
			if (strcmp(&aFile[0], "PLAYERSET") == 0) //プレイヤー
			{
				bPlayer = true;
			}
			if (strcmp(&aFile[0], "END_PLAYERSET") == 0) //プレイヤー
			{
				CPlayer::Create(pos, rot, aPlayerFile);
				bPlayer = false;
			}
			if (strcmp(&aFile[0], "SKYSET") == 0) //空
			{
				bSky = true;
			}
			if (strcmp(&aFile[0], "END_SKYSET") == 0) //空
			{
				CBg::Create(D3DXVECTOR3(0.0f, 0.0f, 0.0f), 0.00002f, CPlane::GetTexture(2));
				bSky = false;
			}
			if (bField == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //角度
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = rot.x / 180.0f * D3DX_PI;
					rot.y = rot.y / 180.0f * D3DX_PI;
					rot.z = rot.z / 180.0f * D3DX_PI;
				}
				if (strcmp(&aFile[0], "BLOCK") == 0) //テクスチャの細かさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &Tex.x, &Tex.y);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d %d", &nBlock[0], &nBlock[1]);
				}
				if (strcmp(&aFile[0], "TEXTYPE") == 0) //テクスチャ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nTex);
				}
			}
			if (bWall == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //回転
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = rot.x / 180.0f * D3DX_PI;
					rot.y = rot.y / 180.0f * D3DX_PI;
					rot.z = rot.z / 180.0f * D3DX_PI;
				}
				if (strcmp(&aFile[0], "BLOCK") == 0) //テクスチャの細かさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &Tex.x, &Tex.y);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d %d", &nBlock[0], &nBlock[1]);
				}
				if (strcmp(&aFile[0], "TEXTYPE") == 0) //テクスチャ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nTex);
				}
			}
			if (bCamera == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &size.x, &size.y);
				}
			}
			if (bTutorial == true)
			{
				if (strcmp(&aFile[0], "POS") == 0) //場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "SIZE") == 0) //大きさ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f", &size.x, &size.y);
				}
				if (strcmp(&aFile[0], "PATTERN") == 0) //動きのパターン
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nPattern);
				}
				if (strcmp(&aFile[0], "PATTERNSET") == 0)
				{
					bPattern = true;
					nCntPattern = 0;
				}
				if (strcmp(&aFile[0], "END_PATTERNSET") == 0)
				{
					bPattern = false;
					nMaxPattern = nCntPattern;
				}
				if (strcmp(&aFile[0], "TEX") == 0) //テクスチャ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nTex);
				}
				if (bPattern == true)
				{
					if (strcmp(&aFile[0], "LOOP") == 0) //テクスチャ
					{
						fscanf(pFile, "%s", &aFile[0]);
						fscanf(pFile, "%d", &nLoop);
					}
					if (strcmp(&aFile[0], "MOVESET") == 0)
					{
						bMove = true;
					}
					if (strcmp(&aFile[0], "END_MOVESET") == 0)
					{
						bMove = false;
						nCntPattern++;
					}
					if (bMove == true)
					{
						if (strcmp(&aFile[0], "MOVE") == 0) //場所
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%f %f", &aPattern[nCntPattern].move.x, &aPattern[nCntPattern].move.y);
						}
						if (strcmp(&aFile[0], "TIME") == 0) //大きさ
						{
							fscanf(pFile, "%s", &aFile[0]);
							fscanf(pFile, "%d", &aPattern[nCntPattern].nMaxTime);
						}
					}
				}
			}
			if (bModel == true)
			{
				if (strcmp(&aFile[0], "TYPE") == 0) //モデルの種類
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nType);
				}
				if (strcmp(&aFile[0], "POS") == 0) //モデルの場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //モデルの方向
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = rot.x / 180.0f * D3DX_PI;
					rot.y = rot.y / 180.0f * D3DX_PI;
					rot.z = rot.z / 180.0f * D3DX_PI;
				}
				if (strcmp(&aFile[0], "COL") == 0) //モデルの当たり判定
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nCollision);
					switch (nCollision)
					{
					case 0:
						collision = CScene::COLLISION_SPHERE;
						break;
					case 1:
						collision = CScene::COLLISION_SQUARE;
						break;
					default:
						break;
					}
				}
			}
			if (bLight == true)
			{
				if (strcmp(&aFile[0], "DIRECTION") == 0) //光の方向
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &vec.x, &vec.y, &vec.z);
				}
				if (strcmp(&aFile[0], "DIFFUSE") == 0) //光の色
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &col.r, &col.g, &col.b);
					col.a = 1.0f;
				}
			}
			if (bSky == true)
			{
				if (strcmp(&aFile[0], "TEXTYPE") == 0) //テクスチャ
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nTex);
				}
				if (strcmp(&aFile[0], "MOVE") == 0) //動き
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f", &fScroll);
				}
			}
			if (bPlayer == true)
			{
				if (strcmp(&aFile[0], "MOTION_FILENAME") == 0) //モデルの場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%s", &aPlayerFile[0]);
				}
				if (strcmp(&aFile[0], "POS") == 0) //モデルの場所
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &pos.x, &pos.y, &pos.z);
				}
				if (strcmp(&aFile[0], "ROT") == 0) //モデルの方向
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%f %f %f", &rot.x, &rot.y, &rot.z);
					rot.x = rot.x / 180.0f * D3DX_PI;
					rot.y = rot.y / 180.0f * D3DX_PI;
					rot.z = rot.z / 180.0f * D3DX_PI;
				}
			}
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //終わり
			{
				break;
			}
		}
		fclose(pFile);
	}
}

void CLoad::SoundLoad(const char *aFileName)
{
	FILE *pFile;
	pFile = fopen(aFileName, "r");
	char aFile[256];
	char aSoundName[64];
	int nLoop;
	int nCntSound = 0;
	int nSound = 0;
	bool bSet = false;
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
			if (strcmp(&aFile[0], "NUM_SOUND") == 0) //いくつ音があるか
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &nSound);
				CSound::SetSoundNum(nSound);
			}
			if (strcmp(&aFile[0], "SOUND_SET") == 0)
			{
				bSet = true;
			}
			if (strcmp(&aFile[0], "END_SOUND_SET") == 0)
			{
				CSound::SetParamData(&aSoundName[0], nLoop, nCntSound);
				bSet = false;
				nCntSound++;
			}
			if (bSet == true)
			{
				if (strcmp(&aFile[0], "NAME") == 0) //名前
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%s", &aSoundName[0]);
				}
				if (strcmp(&aFile[0], "LOOP") == 0) //ループするかしないか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nLoop);
				}
			}
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //終わり
			{
				break;
			}
		}
		fclose(pFile);
	}
}

void CLoad::RankLoad(const char *aFileName)
{
	FILE *pFile;
	pFile = fopen(aFileName, "r");
	char aFile[256];
	char aSoundName[64];
	int nLoop;
	int nCntSound = 0;
	int nSound = 0;
	bool bSet = false;
	if (pFile != NULL)
	{
		while (true)
		{
			fscanf(pFile, "%s", &aFile[0]); //fscanfを繰り返してファイルを読み取っていく
			if (strcmp(&aFile[0], "NUM_SOUND") == 0) //いくつ音があるか
			{
				fscanf(pFile, "%s", &aFile[0]);
				fscanf(pFile, "%d", &nSound);
				CSound::SetSoundNum(nSound);
			}
			if (strcmp(&aFile[0], "SOUND_SET") == 0)
			{
				bSet = true;
			}
			if (strcmp(&aFile[0], "END_SOUND_SET") == 0)
			{
				CSound::SetParamData(&aSoundName[0], nLoop, nCntSound);
				bSet = false;
				nCntSound++;
			}
			if (bSet == true)
			{
				if (strcmp(&aFile[0], "NAME") == 0) //名前
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%s", &aSoundName[0]);
				}
				if (strcmp(&aFile[0], "LOOP") == 0) //ループするかしないか
				{
					fscanf(pFile, "%s", &aFile[0]);
					fscanf(pFile, "%d", &nLoop);
				}
			}
			if (strcmp(&aFile[0], "END_SCRIPT") == 0) //終わり
			{
				break;
			}
		}
		fclose(pFile);
	}
}