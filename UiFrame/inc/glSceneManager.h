#ifndef __GL_SCENE_MANAGER__
#define __GL_SCENE_MANAGER__

#include "globaldef.h"
#include "glScene.h"
#include <map>
#include <string>

using namespace std;

class CScene;

#define SCENE_NUM 50	 /**< �ɹ���ĳ������� */

class CSceneManager
{
public:
	static CSceneManager* Instance();

	static void destroy();

	int init();

	void deinit();

	/************************************************************************** 
	* @brief RefreshScene 
	* 
	* ��ȡ�����б�.
	**************************************************************************/
	void RefreshScene();

	/************************************************************************** 
	* @brief GetSceneList 
	* 
	* ��ȡ���г��������Ƽ�����.
	* @param[in] TitleList		�����б��ڴ��ֱ����ú������䣬���ú�������
	* @param[in] AuthorList		�����б��ڴ��ֱ����ú������䣬���ú�������
	* @param[in] VersionList		�汾�б��ڴ��ֱ����ú������䣬���ú�������
	* @param[in] DesList			�����б��ڴ��ֱ����ú������䣬���ú�������
	* @param[in] fileList			�ļ����б��ڴ��ֱ����ú������䣬���ú�������
	* @return int  ��������
	**************************************************************************/
	int GetSceneList(string*& TitleList,string*& AuthorList,string*& VersionList,string*& DesList,string*& fileList);

	/************************************************************************** 
	* @brief GetTestList 
	* 
	* ��ȡ�����б�.
	* @param[in] sScene			ģ������ 
	* @param[in] pTestInfo		�����б�ָ��
	* @return int  ���Ը���
	**************************************************************************/
	int GetTestList(const string& sScene,TEST_INFO*& pTestInfo);

	/************************************************************************** 
	* @brief TriggerTest 
	* ���в���
	* Detailed description.
	* @param[in] sScene					ģ������
	* @param[in] fMask					������������
	* @param[in] picFrom				ץ֡���
	* @param[in] picto					ץ֡�յ�
	* @param[in] picsavepath			֡�洢��
	* @param[in] testinterval			���Լ��ʱ��
	* @param[in] bFullscreen			�Ƿ�ȫ��ģʽ
	* @return string					���Խ�����ַ�����
	**************************************************************************/
	string TriggerTest(const string& sScene,
						unsigned long long fMask,
						unsigned int picFrom,unsigned int picto, const string& picsavepath,
						unsigned int testinterval,
						bool bFullscreen);

private:
	CSceneManager();
	~CSceneManager();

	static CSceneManager* m_pMan;

	typedef struct
	{
		CScene* Scene;			 /**< ����ָ�� */
		string SceneName;		 /**< �������� */
		int instance;			 /**< ����ʵ�� */
	}s_Scene;
	
	map<string,s_Scene*>	m_pScene;	 /**< �������Ƶ�����ָ���ӳ�� */

};

#endif	// __GL_SCENE_MANAGER__