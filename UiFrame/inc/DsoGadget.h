/**************************************************************************
* @file DsoGadget.h
* @brief 跨平台动态库加载器
* @author Stewart Shao
* @date 7/19/2012 1:19:53 PM
* @version 
**************************************************************************/

#ifndef __DSO_GADGET__
#define __DSO_GADGET__

#include <string>
using namespace std;

#ifdef WIN32
	#include <Windows.h>
	#define  DSOHandle				HMODULE
	#define  INVALID_DSO_HANDLE		NULL
#else //! WIN32
	#define  DSOHandle				void*
	#define  INVALID_DSO_HANDLE		NULL
#endif //end WIN32

#ifdef WIN32
	#define  DSO_PREFIX  ""
#else //! WIN32
	#define  DSO_PREFIX  "lib"
#endif //end WIN32

#ifdef WIN32
	#define  DSO_SUFFIX  ".dll"
#else //! WIN32
	#define  DSO_SUFFIX  ".so"
#endif //end WIN32

class CDsoGadget
{
public:
    CDsoGadget();
    CDsoGadget( const char* pDsoName );
    ~CDsoGadget();

	/************************************************************************** 
	* @brief Load 
	* 
	* 加载DSO.
	* @param[in] pDsoName	待加载的dso名字	 
	* @return int	0	成功 \n
	*				1	参数空指针 \n
	*				2	错误
	**************************************************************************/
	int  Load( const char* pDsoName );
   
	/************************************************************************** 
	* @brief Unload 
	* 
	* 卸载DSO.
	* @return int	0	成功   
	**************************************************************************/
    int  Unload();

	/************************************************************************** 
	* @brief IsLoaded 
	* 
	* 是否已加载DSO.
	* @return bool true or false    
	**************************************************************************/
    bool    IsLoaded();

    /**
     * @brief   get symbol from DLL
     *
     * @param[in]   pSymbolName symbol to get
     * @return  symbol address \n
     *          NULL
     */
	/************************************************************************** 
	* @brief GetSymbol 
	* 
	* 从DSO中获取符号.
	* @param[in] pSymbolName	符号名 
	* @return void*    
	**************************************************************************/
    void*   GetSymbol( const char* pSymbolName );

	/************************************************************************** 
	* @brief GetRawDsoName 
	* 
	* 获取dso名称.如strDsoName为gears.dll返回gears，Linux下strDsoName为libgears.so返回gears
	* @param[inout] strDsoName dso名称
	* @return string	dso名称或空字符创  
	**************************************************************************/
    static  string GetRawDsoName( const string& strDsoName );

	/************************************************************************** 
	* @brief GetFullDsoName 
	* 
	* 获取完整dso名称.如strDsoName为gears在win下返回gears.dll，Linux下返回libgears.so
	* @param[in] strDsoName dso名称
	* @return string  dso名称或空字符串
	**************************************************************************/
    static  string GetFullDsoName( const string& strDsoName );

private:
    DSOHandle  m_handle;
};

#endif	// __DSO_GADGET__