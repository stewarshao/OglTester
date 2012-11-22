#include "glScene.h"
#include "DsoGadget.h"
#include "glwidgetManager.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

CScene::CScene(const char* pDsoFullPath)
{
	// 自动加载解析dso
	m_pDso = new CDsoGadget(pDsoFullPath);

	if (NULL != m_pDso)
	{
		// 加载函数
		fun(title) = (fun_prototype(title))(m_pDso->GetSymbol("Title"));
		fun(author) = (fun_prototype(author))(m_pDso->GetSymbol("Author"));
		fun(version) = (fun_prototype(version))(m_pDso->GetSymbol("Version"));
		fun(describe) = (fun_prototype(describe))(m_pDso->GetSymbol("Describe"));
		fun(GetGlContextVersion) = (fun_prototype(GetGlContextVersion))(m_pDso->GetSymbol("GetGlContextVersion"));

		fun(Init) = (fun_prototype(Init))(m_pDso->GetSymbol("Init"));
		fun(Paint) = (fun_prototype(Paint))(m_pDso->GetSymbol("Paint"));
		fun(resize) = (fun_prototype(resize))(m_pDso->GetSymbol("resize"));
		fun(SetNotifyFn) = (fun_prototype(SetNotifyFn))(m_pDso->GetSymbol("SetNotifyFn"));
		fun(timerFunc) = (fun_prototype(timerFunc))(m_pDso->GetSymbol("timerFunc"));
		
		fun(setXRotation) = (fun_prototype(setXRotation))(m_pDso->GetSymbol("setXRotation"));
		fun(setYRotation) = (fun_prototype(setYRotation))(m_pDso->GetSymbol("setYRotation"));
		fun(setZRotation) = (fun_prototype(setZRotation))(m_pDso->GetSymbol("setZRotation"));
		
		fun(xRotation) = (fun_prototype(xRotation))(m_pDso->GetSymbol("xRotation"));
		fun(yRotation) = (fun_prototype(yRotation))(m_pDso->GetSymbol("yRotation"));
		fun(zRotation) = (fun_prototype(zRotation))(m_pDso->GetSymbol("zRotation"));

		fun(getTestInstances) = (fun_prototype(getTestInstances))(m_pDso->GetSymbol("GetTestInstances"));
		fun(destroyTestInstances) = (fun_prototype(destroyTestInstances))(m_pDso->GetSymbol("DestroyTestInstances"));
	}

	m_Test = NULL;
	m_nMajor = 0;
	memset(m_pFn,0,64);

	m_nSubSceneCount = 0;
	m_nSbuSceneIndex = -1;

	m_nSelectFunc = -1;
}

CScene::~CScene()
{
	if (NULL != m_pDso)
	{
		delete m_pDso;
		m_pDso = NULL;
	}

	DeleteTestPrt();
}

const char* CScene::Title()
{
	if (NULL != fun(title))
		return fun(title)();

	return NULL;
}

const char* CScene::Author()
{
	if (NULL != fun(author))
		return fun(author)();

	return NULL;
}

const char* CScene::Version()
{
	if (NULL != fun(version))
		return fun(version)();

	return NULL;
}

const char* CScene::Describe()
{
	if (NULL != fun(describe))
		return fun(describe)();

	return NULL;
}

void CScene::GetGlContextVersion(unsigned int *pMajor,unsigned int *pMinor,unsigned int *pContextProfile)
{

	if (NULL != fun(GetGlContextVersion))
		return fun(GetGlContextVersion)(pMajor,pMinor,pContextProfile);

	return;
}

void CScene::Init()
{
	if (NULL != fun(Init))
	{
		return fun(Init)();
	}

	return;
}

void CScene::Paint()
{
	if (NULL != fun(Paint))
	{
		return fun(Paint)();
	}

	return;
}

void CScene::resize(int width, int height)
{
	if (NULL != fun(resize))
	{
		return fun(resize)(width,height);
	}

	return;
}

void CScene::timerFunc()
{
	if (NULL != fun(timerFunc))
	{
		return fun(timerFunc)();
	}

	return;
}

void CScene::SetNotifyFn(FnNotify pFn,void* pGlWin)
{
	if (NULL != fun(SetNotifyFn))
	{
		return fun(SetNotifyFn)(pFn,pGlWin);
	}

	return;
}

void CScene::setXRotation(int angle)
{
	if (NULL != fun(setXRotation))
	{
		return fun(setXRotation)(angle);
	}

	return;
}

void CScene::setYRotation(int angle)
{
	if (NULL != fun(setYRotation))
	{
		return fun(setYRotation)(angle);
	}

	return;
}

void CScene::setZRotation(int angle)
{
	if (NULL != fun(setZRotation))
	{
		return fun(setZRotation)(angle);
	}

	return;
}

int CScene::xRotation()
{
	if (NULL != fun(xRotation))
	{
		return fun(xRotation)();
	}

	return 0;
}

int CScene::yRotation()
{
	if (NULL != fun(yRotation))
	{
		return fun(yRotation)();
	}

	return 0;
}

int CScene::zRotation()
{
	if (NULL != fun(zRotation))
	{
		return fun(zRotation)();
	}

	return 0;
}

int CScene::GetTestInstances(TEST_INFO*& pTestInfo)
{
	if (NULL != fun(getTestInstances))
	{
		TEST_INFO *pTestTemp = NULL;
		list<struct ti*>::iterator pos;
		int nMajorTest = fun(getTestInstances)(pTestTemp);
		if (0 < nMajorTest && NULL != pTestTemp)
		{
			m_Test = new TEST_INFO[nMajorTest];
			if (NULL != m_Test)
			{
				m_nMajor = nMajorTest;
				int fnIndex = 0;
				TEST_INFO* pMajor = NULL;
				for (int i = 0; i < nMajorTest; ++i)
				{
					m_Test[i].Name = pTestTemp[i].Name;
					m_Test[i].Describe = pTestTemp[i].Describe;
					int nMinor = pTestTemp[i].SubTest.size();
					if (nMinor <= 0)
					{
						// dso测试函数表出错
						DeleteTestPrt();
						return 0;
					}
					bool bSubScene = false;
					if (m_Test[i].Name == "Sub Scene")
					{
						// 子场景处理
						m_nSubSceneCount = nMinor;
						m_nSbuSceneIndex = i;
						bSubScene = true;
					}
					pMajor = &m_Test[i];
					TEST_INFO* pMinor = new TEST_INFO[nMinor];
					if (NULL == pMinor)
					{
						DeleteTestPrt();
						return 0;
					}

					for (int k = 0; k < nMinor; ++k)
					{
						int j = ++i;
						pMinor[k].Name = pTestTemp[j].Name;
						pMinor[k].Describe = pTestTemp[j].Describe;
						pMinor[k].TestIndex = fnIndex++; 
						m_pFn[pMinor[k].TestIndex] = pTestTemp[j].TestPtr;
						if (bSubScene && -1 == m_nSelectFunc)
							m_nSelectFunc = pMinor[k].TestIndex;
						m_TestName[pMinor[k].TestIndex] = pMinor[k].Name;
						pMajor->SubTest.push_back(&pMinor[k]);
					}
				}
			}

			if (NULL != fun(destroyTestInstances))
				fun(destroyTestInstances)(pTestTemp);
		}
	}
	pTestInfo = m_Test;
	return m_nMajor;
}

void CScene::DeleteTestPrt()
{
	if (NULL == m_Test)
		return;

	list<struct ti*>::iterator pos;
	for (int i = 0; i < m_nMajor; ++i)
	{
		TEST_INFO* p = &m_Test[i];
		for (pos = p->SubTest.begin(); pos != p->SubTest.end(); ++pos)
		{
			TEST_INFO* psub = *pos;
			delete psub;
		}
		delete p;
	}

	m_Test = NULL;
}

void CScene::SetTestParam(bool bFullScreen,unsigned int grabFrom,unsigned int grabTo,const string& grabPath,unsigned int uTestInterval)
{
	m_bFullScreen = bFullScreen;
	m_uGrabFrom = grabFrom;
	m_uGrabTo = grabTo;
	m_GrabPath = grabPath;
	m_uTestInterval = uTestInterval;
	m_bStartGrab = false;
}

string CScene::TriggerTest(unsigned long long fMask,GlwHandle glWin)
{
	m_glWin = glWin;

	string sResult;
	char pReason[256] = {0};

	if (!m_GrabPath.empty())
	{
		// 需要设置抓图标记
		m_bStartGrab = true;			
		m_nPicCount = 1;
		/**< 如果DSO是以多线程设计的，这里会有问题，会抓到不想要的帧 
		解决方法，可以在DSO的每个test开始前，向框架上报一个消息，告知测试开始，完毕再报一个测试结束*/
	}

	if (m_bFullScreen)
		CGlWidgetManager::Instance()->FullScreen(m_glWin);

	m_nTestCase = 0;
	for (int i = 0; i < sizeof(fMask); ++i)
		if (fMask & (1 << i))
			++m_nTestCase;

	if (0 == m_nTestCase)
		return sResult;

	for (int i = 0; i < sizeof(fMask); ++i)
	{
		if (fMask & (1 << i) && NULL != m_pFn[i])
		{
			m_currentTest = m_TestName[i];
			m_pFn[i](pReason);
		}
	}

	return sResult;
}

bool CScene::IsFullScreen()
{
	return m_bFullScreen;
}

bool CScene::CanGrab(string& dirname,int *nPic,string &TestName)
{
	if (m_bStartGrab)
	{
		if (m_nPicCount >= m_uGrabFrom && m_nPicCount <= m_uGrabTo)
		{
			dirname = m_GrabPath;
			*nPic = m_nPicCount++;
			TestName = m_currentTest;
			return true;
		}
		++m_nPicCount;
	}
	

	return false;
}

int CScene::needWait()
{
	if (!--m_nTestCase)
	{
		if (m_bFullScreen)
		{
			CGlWidgetManager::Instance()->FullScreen(m_glWin,false);
			m_bFullScreen = false;
		}
		return 0;
	}

	if ((unsigned int)-1 == m_uTestInterval)
		m_uTestInterval = 0;

	return m_uTestInterval;
}

int CScene::GetSubScene(char (*pSubScene)[256])
{
	if (NULL == pSubScene)
		return m_nSubSceneCount;

	if (m_nSubSceneCount > 0)
	{
		int nSceneCount = 0;

		if (!m_Test[m_nSbuSceneIndex].SubTest.empty())
		{
			list<TEST_INFO*>::iterator pos;
			for (pos = m_Test[m_nSbuSceneIndex].SubTest.begin(); pos != m_Test[m_nSbuSceneIndex].SubTest.end(); ++pos)
			{
				TEST_INFO *pSub = *pos;
				strcpy(pSubScene[nSceneCount],pSub->Name.c_str());
				strcat(pSubScene[nSceneCount],"*");
				strcat(pSubScene[nSceneCount++],pSub->Describe.c_str());
			}
		}
	}
	
	return 0;
}

int CScene::SelectSubScene(int index)
{
	char cindex[24] = {0};
#ifndef WIN32
	sprintf(cindex,"%d",index);
#else
	itoa(index,cindex,10);
#endif
	if (m_pFn[m_nSelectFunc])
		m_pFn[m_nSelectFunc](cindex);
	return 0;
}
