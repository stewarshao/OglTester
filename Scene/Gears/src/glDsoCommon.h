/**************************************************************************
* @file glDsoCommon.h
* @brief	gl����ģ�鵼������ͷ�ļ� 
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
* ��ȡGl����ģ�������.
* @return char*  ��������
**************************************************************************/
DllAPI	const char* Title();

/************************************************************************** 
* @brief Author 
* 
* ��ȡGl����ģ���������.
* @return char*  ��������
**************************************************************************/
DllAPI	const char* Author();

/************************************************************************** 
* @brief Version 
* 
* ��ȡGl����ģ��İ汾.
* @return char*  �汾��
**************************************************************************/
DllAPI	const char* Version();

/************************************************************************** 
* @brief Describe 
* 
* ����ϸ������.
* @return char*	����  
**************************************************************************/
DllAPI	const char* Describe();

enum {
	NoProfile,
	CoreProfile,
	CompatibilityProfile
}; 

/************************************************************************** 
* @brief GetGlContextVersion 
* ��ȡ֧�ֵ�GL context�汾��Ϣ
* Detailed description.
* @param[in] pMajor				���汾
* @param[in] pMinor				�˰汾
* @param[in] pContextProfile	core��compatible
* @return DllAPI void  
**************************************************************************/
DllAPI void GetGlContextVersion(unsigned int *pMajor,unsigned int *pMinor,unsigned int *pContextProfile);

/************************************************************************** 
* @brief Init 
* ��ʼ��gl context
* Detailed description.
* @return DllAPI void  
**************************************************************************/
DllAPI void Init();

/************************************************************************** 
* @brief Paint 
* ����gl scene
* Detailed description.
* @return DllAPI void  
**************************************************************************/
DllAPI void Paint();

/************************************************************************** 
* @brief resize 
* ���ڳߴ�仯��Ӧ
* Detailed description.
* @param[in] width		���ڿ��
* @param[in] height		���ڸ߶�
* @return DllAPI void	
**************************************************************************/
DllAPI void resize(int width, int height);

/************************************************************************** 
* @brief timerFunc 
* ��ʱ������
* Detailed description.
* @return DllAPI void  
**************************************************************************/
DllAPI void timerFunc();

extern FnNotify pNotify;
extern void *pOglWinP;
void Notify(int nMsgID,void *pParam);
/************************************************************************** 
* @brief SetNotifyFn 
* ������Ϣ�ص�����
* Detailed description.
* @param[in] pFn		�ص�����ָ��
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
// ������Ϣ�ṹ��
typedef struct ti
{
	string Name;			 /**< �������� */
	string Describe;		 /**< �������� */
	FnTest TestPtr;			 /**< ����ָ�� */
	list<struct ti*> SubTest;	 /**< ����С�� */
	int TestIndex;			 /**< �������� */
}TEST_INFO;

DllAPI int GetTestInstances(TEST_INFO*& pTestInfo);

DllAPI void DestroyTestInstances(TEST_INFO* pTestInfo);

#endif	// __GL_DSO_COMMON_s