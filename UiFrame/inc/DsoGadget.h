/**************************************************************************
* @file DsoGadget.h
* @brief ��ƽ̨��̬�������
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
	* ����DSO.
	* @param[in] pDsoName	�����ص�dso����	 
	* @return int	0	�ɹ� \n
	*				1	������ָ�� \n
	*				2	����
	**************************************************************************/
	int  Load( const char* pDsoName );
   
	/************************************************************************** 
	* @brief Unload 
	* 
	* ж��DSO.
	* @return int	0	�ɹ�   
	**************************************************************************/
    int  Unload();

	/************************************************************************** 
	* @brief IsLoaded 
	* 
	* �Ƿ��Ѽ���DSO.
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
	* ��DSO�л�ȡ����.
	* @param[in] pSymbolName	������ 
	* @return void*    
	**************************************************************************/
    void*   GetSymbol( const char* pSymbolName );

	/************************************************************************** 
	* @brief GetRawDsoName 
	* 
	* ��ȡdso����.��strDsoNameΪgears.dll����gears��Linux��strDsoNameΪlibgears.so����gears
	* @param[inout] strDsoName dso����
	* @return string	dso���ƻ���ַ���  
	**************************************************************************/
    static  string GetRawDsoName( const string& strDsoName );

	/************************************************************************** 
	* @brief GetFullDsoName 
	* 
	* ��ȡ����dso����.��strDsoNameΪgears��win�·���gears.dll��Linux�·���libgears.so
	* @param[in] strDsoName dso����
	* @return string  dso���ƻ���ַ���
	**************************************************************************/
    static  string GetFullDsoName( const string& strDsoName );

private:
    DSOHandle  m_handle;
};

#endif	// __DSO_GADGET__