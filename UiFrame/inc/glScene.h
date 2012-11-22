#ifndef __GL_SCENE_H__
#define  __GL_SCENE_H__

#include	"globaldef.h"
#include	"NotifyMsg.h"

#include	<string>
#include	<list>
using namespace std;

class CDsoGadget;

#define import_fun(retvalue,name,param) \
	typedef retvalue (*fn_##name)(param);\
fn_##name m_pfn_##name;\

#define importM_fun(retvalue,name,...) \
	typedef retvalue (*fn_##name)(...);\
	fn_##name m_pfn_##name;\

	
#define fun_prototype(name)	fn_##name
#define fun(name) m_pfn_##name

typedef int (*FnTest)(char* pReason);
// 测试信息结构体
typedef struct ti
{
	string Name;			 /**< 测试名称 */
	string Describe;		 /**< 测试描述 */
	FnTest TestPtr;			 /**< 函数指针 */
	list<struct ti*> SubTest;		 /**< 测试小项 */
	int TestIndex;			 /**< 测试索引 */
}TEST_INFO;

class CScene
{
public:
	CScene(const char* pDsoFullPath);
	~CScene();

	///////////////////////////////essential//////////////////////////////////////
	const char* Title();
	const char* Author();
	const char* Version();
	const char* Describe();
	void GetGlContextVersion(unsigned int *pMajor,unsigned int *pMinor,unsigned int *pContextProfile);
	void Init();
	void Paint();
	void resize(int width, int height);
	void SetNotifyFn(FnNotify pFn,void* pGlWin);
	void timerFunc();

	//////////////////////////////transform//////////////////////////////////
	void setXRotation(int angle);
	void setYRotation(int angle);
	void setZRotation(int angle);

	int xRotation();
	int yRotation();
	int zRotation();

	/////////////////////////////////////test////////////////////////////////
	int GetTestInstances(TEST_INFO*& pTestInfo);

	void DeleteTestPrt();

	int GetSubScene(char (*pSubScene)[256]);

	int SelectSubScene(int index);

	void SetTestParam(bool bFullScreen,unsigned int grabFrom,unsigned int grabTo,const string& grabPath,unsigned int uTestInterval);

	string TriggerTest(unsigned long long fMask,GlwHandle glWin);

	bool IsFullScreen();

	bool CanGrab(string& dirname,int *nPic,string &TestName);

	int needWait();				 /**< 是否需要等待，还是直接开始下一个测试 */

private:
	CDsoGadget *m_pDso;				// Dso加载工具
	TEST_INFO *m_Test;				// test信息列表
	int m_nMajor;					// 测试主项
	FnTest m_pFn[64];				// 测试函数指针数组	
	string m_TestName[64];			 /**< 测试名称 */
	string m_currentTest;			 /**< 正在进行的测试名称*/
	GlwHandle m_glWin;				 /**< 关联的gl context */

	bool m_bFullScreen;
	unsigned int m_uGrabFrom;
	unsigned int m_uGrabTo;
	string m_GrabPath;
	unsigned int m_uTestInterval;
	int m_nTestCase;

	bool m_bStartGrab;				/**< 开始抓图 */
	int	m_nPicCount;				/**< 已抓图数量 */

	int m_nSubSceneCount;			/**< 子场景数量 */
	int m_nSbuSceneIndex;			/**< 子场景在m_Test中的索引 */
	int m_nSelectFunc;

	import_fun(const char* ,title,void);
	import_fun(const char* ,author,void);
	import_fun(const char* ,version,void);
	import_fun(const char* ,describe,void);
	importM_fun(void,GetGlContextVersion,unsigned int *pMajor,unsigned int *pMinor,unsigned int *pContextProfile);
	import_fun(void,Init,void);
	import_fun(void,Paint,void);
	importM_fun(void,resize,int width, int height);
	importM_fun(void,SetNotifyFn,FnNotify pFn,void* pParam);
	import_fun(void,timerFunc,void);

	import_fun(void,setXRotation,int angle);
	import_fun(void,setYRotation,int angle);
	import_fun(void,setZRotation,int angle);
	import_fun(int,xRotation,void);
	import_fun(int,yRotation,void);
	import_fun(int,zRotation,void);
	
	import_fun(int ,getTestInstances,TEST_INFO*& pTestInfo);  //&在importM_fun中会出问题
	import_fun(void, destroyTestInstances,TEST_INFO* pTestInfo);
	
};


#endif // __GL_SCENE_H__

