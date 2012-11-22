#ifndef __GL_SCENE_MANAGER__
#define __GL_SCENE_MANAGER__

#include "globaldef.h"
#include "glScene.h"
#include <map>
#include <string>

using namespace std;

class CScene;

#define SCENE_NUM 50	 /**< 可管理的场景总数 */

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
	* 获取场景列表.
	**************************************************************************/
	void RefreshScene();

	/************************************************************************** 
	* @brief GetSceneList 
	* 
	* 获取所有场景的名称及描述.
	* @param[in] TitleList		名称列表，内存又被调用函数分配，调用函数销毁
	* @param[in] AuthorList		作者列表，内存又被调用函数分配，调用函数销毁
	* @param[in] VersionList		版本列表，内存又被调用函数分配，调用函数销毁
	* @param[in] DesList			描述列表，内存又被调用函数分配，调用函数销毁
	* @param[in] fileList			文件名列表，内存又被调用函数分配，调用函数销毁
	* @return int  场景个数
	**************************************************************************/
	int GetSceneList(string*& TitleList,string*& AuthorList,string*& VersionList,string*& DesList,string*& fileList);

	/************************************************************************** 
	* @brief GetTestList 
	* 
	* 获取测试列表.
	* @param[in] sScene			模块名称 
	* @param[in] pTestInfo		测试列表指针
	* @return int  测试个数
	**************************************************************************/
	int GetTestList(const string& sScene,TEST_INFO*& pTestInfo);

	/************************************************************************** 
	* @brief TriggerTest 
	* 进行测试
	* Detailed description.
	* @param[in] sScene					模块名称
	* @param[in] fMask					测试例程掩码
	* @param[in] picFrom				抓帧起点
	* @param[in] picto					抓帧终点
	* @param[in] picsavepath			帧存储点
	* @param[in] testinterval			测试间隔时间
	* @param[in] bFullscreen			是否全屏模式
	* @return string					测试结果，字符描述
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
		CScene* Scene;			 /**< 场景指针 */
		string SceneName;		 /**< 场景名称 */
		int instance;			 /**< 场景实例 */
	}s_Scene;
	
	map<string,s_Scene*>	m_pScene;	 /**< 场景名称到场景指针的映射 */

};

#endif	// __GL_SCENE_MANAGER__