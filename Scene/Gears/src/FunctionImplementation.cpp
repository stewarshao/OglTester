#include "gears.h"

typedef struct
{
	const char* name;
	const char* descirbe;
	FnTest funcptr;
}TEST_TABLE;

static int Translate_Test(char* pReason);
static int Rotate_Test(char* pReason);

static TEST_TABLE g_TestFunTable[] = 
{
	{"Transformation","",NULL},
	{"Translate","",Translate_Test},
//	{"Rotate","",Rotate_Test},
};

CSceneGears theScene;

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