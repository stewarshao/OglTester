#include "sceneManager.h"
#include <stdlib.h>

typedef struct
{
	const char* name;
	const char* descirbe;
	FnTest funcptr;
}TEST_TABLE;

static int Translate_Test(char* pReason);
static int Rotate_Test(char* pReason);
static int SwitchToSubScene(char* pSubScene);

static TEST_TABLE g_TestFunTable[] = 
{
/*
	{"Transformation","",NULL},
	{"Translate","",Translate_Test},
	{"Rotate","",Rotate_Test},
*/
	{"Sub Scene","",NULL},
	{"basic","1.0",SwitchToSubScene},
	{"basic_layout","1.1",SwitchToSubScene},
	{"basic_uniform","1.2",SwitchToSubScene},
	{"basic_uniformblock","1.3",SwitchToSubScene},
	{"scene_diffuse","2.4",SwitchToSubScene},
	{"scene_ads","2.5",SwitchToSubScene},
	{"scene_twoside","2.6",SwitchToSubScene},
	{"scene_flat","2.7",SwitchToSubScene},
	{"scene_discard","2.8",SwitchToSubScene},
	{"scene_toon","3.9",SwitchToSubScene},
	{"scene_directional","3.10",SwitchToSubScene},
	{"scene_fog","3.11",SwitchToSubScene},
	{"scene_multilight","3.12",SwitchToSubScene},
	{"scene_perfragment","3.13",SwitchToSubScene},
	{"scene_spot","3.14",SwitchToSubScene},
};

CSceneManager theScene;

const char* Title()
{
	return theScene.Title();
}

const char* Author()
{
	return theScene.Author();
}

const char* Version()
{
	return theScene.Version();
}

const char* Describe()
{
	return theScene.Describe();
}

void GetGlContextVersion(unsigned int *pMajor,unsigned int *pMinor,unsigned int *pContextProfile)
{
	*pMajor = 4;
	*pMinor = 0;
	*pContextProfile = CompatibilityProfile;
}

void Init()
{
	return theScene.Init();
}

void Paint()
{
	return theScene.Paint();
}

void resize(int width, int height)
{
	return theScene.Resize(width,height);
}

void timerFunc()
{
	return theScene.timerFunc();
}

FnNotify pNotify = NULL;
void *pOglWinP = NULL;
void SetNotifyFn(FnNotify pFn,void *pParam)
{
	pNotify = pFn;
	pOglWinP = pParam;
}

void setXRotation(int angle)
{
	return theScene.setXRotation(angle);
}

void setYRotation(int angle)
{
	return theScene.setYRotation(angle);
}

void setZRotation(int angle)
{
	return theScene.setZRotation(angle);
}

int xRotation()
{
	return theScene.xRotation();
}

int yRotation()
{
	return theScene.yRotation();
}

int zRotation()
{
	return theScene.zRotation();
}

int GetTestInstances(TEST_INFO*& pTestInfo)
{
	// pTestInfo的内存由DSO分配，之后由调用端调用DestroyTestInstance进行删除
	int nTest = sizeof(g_TestFunTable)/sizeof(TEST_TABLE);
	if (0 == nTest)
		return 0;

	pTestInfo = new TEST_INFO[nTest];

	if (NULL == pTestInfo)
		return 0;

	int nActualTest = 0;
	TEST_INFO* pMajor;
	for (int i = 0 ; i < nTest; ++i)
	{
		pTestInfo[i].Name = g_TestFunTable[i].name;
		pTestInfo[i].Describe = g_TestFunTable[i].descirbe;
		if (NULL == g_TestFunTable[i].funcptr)
		{
			// Major Test
			pMajor = &pTestInfo[i];
			pTestInfo[i].TestPtr = NULL;
			++nActualTest;
		}
		else
		{
			// Minor Test
			pMajor->SubTest.push_back(&pTestInfo[i]);
			pTestInfo[i].TestPtr = g_TestFunTable[i].funcptr;
		}
	}

	return nActualTest;
}

void DestroyTestInstances(TEST_INFO* pTestInfo)
{
	if (NULL != pTestInfo)
		delete[] pTestInfo;
}

int Translate_Test(char* pReason)
{
	Notify(OT_MSG_TEST_BEGIN,NULL);
	int ret = theScene.Translate_Test(pReason);
	return ret;
}

int Rotate_Test(char* pReason)
{
	Notify(OT_MSG_TEST_BEGIN,NULL);
	int ret = theScene.Rotate_Test(pReason);
	Notify(OT_MSG_TEST_FINISH,NULL);
	return ret;
}

void Notify(int nMsgID,void *pParam)
{
	if (nMsgID <= OT_MSG_START || nMsgID >= OT_MSG_END || NULL == pNotify)
		return;

	pNotify(nMsgID,pOglWinP,pParam);
}

int SwitchToSubScene(char* pSubScene)
{
	int nSubScene = atoi(pSubScene);
	return theScene.SwitchToScene(nSubScene);
}