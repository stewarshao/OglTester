/**************************************************************************
* @file glDsoCommon.h
* @brief	gl场景模块导出函数头文件 
* @author Stewart Shao
* @date 7/19/2012 10:17:30 AM
* @version 
**************************************************************************/

#ifndef __GL_DSO_COMMON__
#define __GL_DSO_COMMON__

#ifdef WIN32    
	#include <windows.h>     
	#ifdef __GL_COMMON_DSO
		#define DllAPI extern "C" __declspec(dllexport)     
	#else         
		#define DllAPI extern "C" __declspec(dllimport)   
	#endif    
	#ifndef STDCALL        
		#define STDCALL __stdcall    
	#endif
#else    
	#define DllAPI     extern "C"
	#define STDCALL
#endif

#include "NotifyMsg.h"
#include <string>
#include <list>
using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////
//////////						Common Section
///////////////////////////////////////////////////////////////////////////////////////////
/************************************************************************** 
* @brief Title 
* 
* 获取Gl场景模块的名称.
* @return char*  场景名称
**************************************************************************/
DllAPI	const char* Title();

/************************************************************************** 
* @brief Author 
* 
* 获取Gl场景模块的作者名.
* @return char*  作者名称
**************************************************************************/
DllAPI	const char* Author();

/************************************************************************** 
* @brief Version 
* 
* 获取Gl场景模块的版本.
* @return char*  版本号
**************************************************************************/
DllAPI	const char* Version();

/************************************************************************** 
* @brief Describe 
* 
* 场景细节描述.
* @return char*	描述  
**************************************************************************/
DllAPI	const char* Describe();

enum {
	NoProfile,
	CoreProfile,
	CompatibilityProfile
}; 

/************************************************************************** 
* @brief GetGlContextVersion 
* 获取支持的GL context版本信息
* Detailed description.
* @param[in] pMajor				主版本
* @param[in] pMinor				此版本
* @param[in] pContextProfile	core，compatible
* @return DllAPI void  
**************************************************************************/
DllAPI void GetGlContextVersion(unsigned int *pMajor,unsigned int *pMinor,unsigned int *pContextProfile);

/************************************************************************** 
* @brief Init 
* 初始化gl context
* Detailed description.
* @return DllAPI void  
**************************************************************************/
DllAPI void Init();

/************************************************************************** 
* @brief Paint 
* 绘制gl scene
* Detailed description.
* @return DllAPI void  
**************************************************************************/
DllAPI void Paint();

/************************************************************************** 
* @brief resize 
* 窗口尺寸变化响应
* Detailed description.
* @param[in] width		窗口宽度
* @param[in] height		窗口高度
* @return DllAPI void	
**************************************************************************/
DllAPI void resize(int width, int height);

/************************************************************************** 
* @brief timerFunc 
* 定时器函数
* Detailed description.
* @return DllAPI void  
**************************************************************************/
DllAPI void timerFunc();

extern FnNotify pNotify;
extern void *pOglWinP;
void Notify(int nMsgID,void *pParam);
/************************************************************************** 
* @brief SetNotifyFn 
* 设置消息回调函数
* Detailed description.
* @param[in] pFn		回调函数指针
* @return DllAPI void  
**************************************************************************/
DllAPI void SetNotifyFn(FnNotify pFn,void* pParam);

///////////////////////////////////////////////////////////////////////////////////////////
//////////						transform Section
///////////////////////////////////////////////////////////////////////////////////////////
DllAPI void setXRotation(int angle);

DllAPI void setYRotation(int angle);

DllAPI void setZRotation(int angle);

DllAPI int xRotation();

DllAPI int yRotation();

DllAPI int zRotation();

///////////////////////////////////////////////////////////////////////////////////////////
//////////						Test Section
///////////////////////////////////////////////////////////////////////////////////////////
typedef int (*FnTest)(char* pReason);
// 测试信息结构体
typedef struct ti
{
	string Name;			 /**< 测试名称 */
	string Describe;		 /**< 测试描述 */
	FnTest TestPtr;			 /**< 函数指针 */
	list<struct ti*> SubTest;	 /**< 测试小项 */
	int TestIndex;			 /**< 测试索引 */
}TEST_INFO;

DllAPI int GetTestInstances(TEST_INFO*& pTestInfo);

DllAPI void DestroyTestInstances(TEST_INFO* pTestInfo);

#endif	// __GL_DSO_COMMON_s