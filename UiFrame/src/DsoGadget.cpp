#include "DsoGadget.h"

#ifndef WIN32
	#include	<dlfcn.h>
	#include	<string.h>
#endif //end !WIN32

CDsoGadget::CDsoGadget()
:m_handle(INVALID_DSO_HANDLE)
{
}

CDsoGadget::CDsoGadget(const char* pDsoName)
:m_handle(INVALID_DSO_HANDLE)
{
	// 自动加载
	if (NULL != pDsoName)
		Load(pDsoName);
}

CDsoGadget::~CDsoGadget()
{
	Unload();
}

int CDsoGadget::Load(const char* pDsoName)
{
	if (NULL ==  pDsoName)
	{
		return 1;
	}

	if (IsLoaded())
	{
		// 若已加载，就返回错误，之后根据需要添加，根据同名库版本号，进行重新加载
		return 2;
		//Unload();
	}

	// get loadable full DLL name
	string strFullDsoName = GetFullDsoName(pDsoName);

#ifdef WIN32
	WCHAR wName[128] = {0};
	MultiByteToWideChar(CP_ACP, 0, strFullDsoName.c_str(), strFullDsoName.length(), wName, sizeof(wName));
	m_handle = ::LoadLibrary((LPCWSTR)wName);
#else //! WIN32
	m_handle = ::dlopen(strFullDsoName.c_str(), RTLD_LAZY);
#endif //end WIN32

	return INVALID_DSO_HANDLE != m_handle ? 0 : 2;
}

int CDsoGadget::Unload()
{
	if (INVALID_DSO_HANDLE != m_handle)
	{
#ifdef WIN32
		::FreeLibrary(m_handle);
#else //! WIN32
		::dlclose(m_handle);
#endif //end WIN32

		m_handle = INVALID_DSO_HANDLE;
	}
	return 0;
}

bool CDsoGadget::IsLoaded()
{
	return (INVALID_DSO_HANDLE != m_handle);
}

void* CDsoGadget::GetSymbol(const char* pSymbolName)
{
	if (NULL == pSymbolName)
		return NULL;

	if (INVALID_DSO_HANDLE != m_handle)
	{
#ifdef WIN32
		return (void*)(::GetProcAddress(m_handle, pSymbolName));
#else //! WIN32
		return ::dlsym(m_handle, pSymbolName);
#endif //end WIN32
	}

	return NULL;
}

string CDsoGadget::GetRawDsoName(const string& strDsoName)
{
	string strRawDsoName = strDsoName;

#ifndef WIN32 //! WIN32
	string::size_type lenFull = strRawDsoName.length();
	string::size_type lenPrefix = ::strlen(DSO_PREFIX);
	if (lenFull > lenPrefix)
	{
		string strDLLPrefix(strRawDsoName, 0, lenPrefix);
		if (strDLLPrefix == DSO_PREFIX)
			strRawDsoName = strRawDsoName.c_str() + lenPrefix;
	}
#else
	string::size_type lenFull = strRawDsoName.length();
	string::size_type lenSuffix = ::strlen(DSO_SUFFIX);
	if (lenFull > lenSuffix)
	{
		string strDsoSuffix(strRawDsoName, lenFull-lenSuffix);
		if (strDsoSuffix == DSO_SUFFIX)
			strRawDsoName.resize(lenFull-lenSuffix);
	}
#endif //end !WIN32

	return strRawDsoName;
}

string CDsoGadget::GetFullDsoName(const string& strDsoName)
{
	string strFullDsoName = strDsoName;

#ifndef WIN32 //! WIN32
	/*
	string::size_type lenRaw = strFullDsoName.length();
	string::size_type lenPrefix = ::strlen(DSO_PREFIX);
	if ( lenRaw > lenPrefix ) // check if need append DLL prefix
	{
		string strDsoPrefix(strFullDsoName, 0, lenPrefix);
		if (DSO_PREFIX != strDsoPrefix)
			strFullDsoName.insert(0, DSO_PREFIX);
	}
	else
	{
		strFullDsoName.insert(0, DSO_PREFIX);
	}
	*/
#else
	string::size_type lenRaw = strFullDsoName.length();
	string::size_type lenSuffix = ::strlen(DSO_SUFFIX);
	if (lenRaw > lenSuffix)
	{
		string strDLLSuffix(strFullDsoName, lenRaw-lenSuffix);
		if (DSO_SUFFIX != strDLLSuffix)
			strFullDsoName.append(DSO_SUFFIX);
	}
	else
		strFullDsoName.append(DSO_SUFFIX);
#endif //end !WIN32

	return strFullDsoName;
}

