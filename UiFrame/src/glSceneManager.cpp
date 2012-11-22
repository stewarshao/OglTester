#include "glSceneManager.h"
#include <stddef.h>
#include <QDir>
#include <QDebug>
#include "uiframe.h"

#if (!defined(QT_NO_DEBUG))  
	#define DSO_PATH "../../dso/Debug"
#else
	#define DSO_PATH "../../dso/Release"
#endif
CSceneManager* CSceneManager::m_pMan = NULL;

CSceneManager::CSceneManager()
{
}

CSceneManager::~CSceneManager()
{

}

CSceneManager* CSceneManager::Instance()
{
	if (NULL == m_pMan)
	{
		m_pMan = new CSceneManager();
		if (NULL != m_pMan)
		{
			int nRet = m_pMan->init();
			if (0 != nRet)
			{
				m_pMan->deinit();
				delete m_pMan;
				m_pMan = NULL;
			}
		}
	}
	return m_pMan;
}

int CSceneManager::init()
{
	RefreshScene();

	return 0;
}

void CSceneManager::deinit()
{
}

void CSceneManager::destroy()
{
	if (NULL != m_pMan)
	{
		m_pMan->deinit();
		delete m_pMan;
		m_pMan = NULL;
	}
}

void CSceneManager::RefreshScene()
{
	QDir dsoDir(QString(DSO_PATH));
	QString dsoFullPath = dsoDir.absolutePath();
	QStringList filters;
#ifdef WIN32
    filters << "*.dll";
#else
	filters << "*.so";
#endif
    dsoDir.setNameFilters(filters);
	QStringList fl = dsoDir.entryList();
	map<string,s_Scene*>::iterator pos;
	for (int i = 0; i < fl.count(); ++i)
	{
		if (!m_pScene.empty())
		{
			pos = m_pScene.find(fl.at(i).toStdString());
			if (pos != m_pScene.end())
				continue;		// 此模块已被加载
		}
		QString dsoFile = dsoFullPath + "/" + fl.at(i);
		s_Scene* pScene = new s_Scene;
		pScene->instance = 1;
		pScene->SceneName = dsoFile.toStdString();
		pScene->Scene = new CScene(dsoFile.toStdString().c_str());
		m_pScene.insert(map<string,s_Scene*>::value_type(fl.at(i).toStdString(),pScene));
	}
}

int CSceneManager::GetSceneList(string*& TitleList,string*& AuthorList,string*& VersionList,string*& DesList,string*& fileList)
{
	if (m_pScene.empty())
		return 0;

	int nScene = m_pScene.size();
	TitleList = new string[nScene];
	AuthorList = new string[nScene];
	VersionList = new string[nScene];
	DesList = new string[nScene];
	fileList = new string[nScene];

	int i = 0;
	map<string,s_Scene*>::iterator pos;
	for (pos = m_pScene.begin(); pos != m_pScene.end(); ++pos)
	{
		s_Scene* pS = pos->second;
		if (NULL != pS)
		{
			fileList[i] = pos->first;
			TitleList[i] = pS->Scene->Title();
			AuthorList[i] = pS->Scene->Author();
			VersionList[i] = pS->Scene->Version();
			DesList[i] = pS->Scene->Describe();
			++i;
		}
	}

	return i;
}

int CSceneManager::GetTestList(const string& sScene,TEST_INFO*& pTestInfo)
{
	if (sScene.empty())
		return 0;

	map<string,s_Scene*>::iterator pos = m_pScene.find(sScene);

	if (pos == m_pScene.end())
		return 0;
	else
	{
		s_Scene* ps = pos->second;
		return ps->Scene->GetTestInstances(pTestInfo);
	}

	return 0;
}

string CSceneManager::TriggerTest(const string& sScene,
				   unsigned long long fMask,
				   unsigned int picFrom,unsigned int picto, const string& picsavepath,
				   unsigned int testinterval,
				   bool bFullscreen)
{
	string sResult;

	if (sScene.empty())
		return sResult;				// 此种情况不应该发生

	map<string,s_Scene*>::iterator pos = m_pScene.find(sScene);

	if (pos == m_pScene.end())
		return sResult;				// 此种情况不应该发生
	else
	{
		s_Scene* ps = pos->second;

		// 设置测试参数
		ps->Scene->SetTestParam(bFullscreen,picFrom,picto,picsavepath,testinterval);
		
		// 开启场景ogl context
		GlwHandle glwin = g_MainWnd->CreateNewGLWin(ps->Scene);
		if ((GlwHandle)-1 == glwin)
			return sResult.append("Failed to create gl context!");

		// 开始测试
		sResult = ps->Scene->TriggerTest(fMask,glwin);
	}

	return sResult;
}
