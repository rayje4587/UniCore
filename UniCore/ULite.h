﻿/*! \file ULite.h
    \brief 该头文件包含了一些常用的函数.
    该头文件是UniCore库的精简版,没有cpp文件,直接包含即可使用.
    方便测试或者简单开发时使用,不能和UniCore库的其他头文件一起使用.

    \author unigauldoth@gmail.com
    \date 2013-5-28
*/
#ifndef UNICORE_ULITE_H
#define UNICORE_ULITE_H

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#include <cassert>
#include <typeinfo>
#include <locale.h>
#include <stdarg.h>
#include <stdio.h>
#include <string>
#include <vector>

/*! \fn void DebugMessage(const char *format,...)
    \brief 格式化输出调试信息到调试器，使用方法和printf类似。
    
    \param format   待格式化的字符串。
    \param ...      用于格式化的参数。
*/
inline void DebugMessage(const char *format,...)
{
    va_list ap;
    va_start(ap,format);
    //该函数返回格式化后的字符串长度，不包括0结束符。
    _locale_t loc = _create_locale(LC_ALL,"");
    const int len = _vscprintf_p_l(format,loc,ap)+1;
    char *buffer = new char[len];

    _vsprintf_p_l(buffer,len,format,loc,ap);
    _free_locale(loc);
    va_end(ap);
    OutputDebugStringA(buffer);

    delete[] buffer;
}

/*! \fn void DebugMessage(const wchar_t *format,...)
    \brief 格式化输出调试信息到调试器，使用方法和printf类似。

    
    \param format   待格式化的字符串。
    \param ...      用于格式化的参数。
*/
inline void DebugMessage(const wchar_t *format,...)
{
    va_list ap;
    va_start(ap,format);
    //该函数返回格式化后的字符串长度，不包括0结束符。
    _locale_t loc = _create_locale(LC_ALL,"");
    const int len = _vscwprintf_p_l(format,loc,ap)+1;
    wchar_t *buffer = new wchar_t[len];

    _vswprintf_p_l(buffer,len,format,loc,ap);
    _free_locale(loc);
    va_end(ap);
    OutputDebugStringW(buffer);

    delete[] buffer;
}

//! 将std::string转换为std::wstring。
/*!
    ws2s使用指定的locale转换MBCS字符串到Unicode字符串。
    \param s 源字符串。
    \param locale locale名字，请参考setlocale函数。
    \return 转换后的字符串。
    使用方法：
    \code
    //Test.cpp
    //Test.cpp使用到<string>中定义的类，应该包含<string>。
    #include <string>
    #include "UCast.h"

    using namespace std;
    wstring temp = s2ws("我们");  //使用系统当前的区域设置进行转换。
    \endcode
*/
inline std::wstring s2ws(const std::string &s,const char *locale = "")
{
    using namespace std;
    wstring result;
    const size_t len = s.size()+1;
    wchar_t *dest = new wchar_t[len];
    size_t numOfCharConverted = 0;
    _locale_t loc = _create_locale(LC_CTYPE,locale);  //使用指定的locale。
    if(!loc)
    {
        //指定的locale创建失败，使用实现指定的本地环境。
        loc = _create_locale(LC_CTYPE,"");
    }
    errno_t err = _mbstowcs_s_l(&numOfCharConverted,dest,len,s.c_str(),_TRUNCATE,loc);
    _free_locale(loc);
    if(err == 0)
    {
        result = dest;
    }
    else if(err == STRUNCATE)
    {
        OutputDebugStringA("UniCore s2ws 目标缓冲区不足，字符串被截断。");
        result = dest;
    }
    else
    {
        assert(!"UniCore s2ws 转换MBCS字符串到Unicode字符串时失败。");
        OutputDebugStringA("UniCore s2ws 转换MBCS字符串到Unicode字符串时失败。");
    }
    delete []dest;
    return result;
}


//! 将std::wstring转换为std::string。
/*!
    ws2s使用指定的locale转换Unicode字符串到MBCS字符串。
    \param ws 源字符串。
    \param locale locale名字，请参考setlocale函数。
    \return 转换后的字符串。
    使用方法：
    \code
    //Test.cpp
    //Test.cpp使用到<string>中定义的类，应该包含<string>。
    #include <string>
    #include "UCast.h"

    using namespace std;
    string temp = ws2s(L"我们");  //使用系统当前的区域设置进行转换。
    \endcode
*/
inline std::string ws2s(const std::wstring &ws,const char *locale = "")
{
    using namespace std;
    string result;
    const size_t len = ws.size()*4+1;
    char *dest = new char[len];
    size_t numOfCharConverted = 0;
    _locale_t loc = _create_locale(LC_CTYPE,locale);
    if(!loc)
    {
        loc = _create_locale(LC_CTYPE,"");
    }
    errno_t err = _wcstombs_s_l(&numOfCharConverted,dest,len,ws.c_str(),_TRUNCATE,loc);
    _free_locale(loc);
    if(err == 0)
    {
        result = dest;
    }
    else if(err == STRUNCATE)
    {
        OutputDebugStringA("UniCore ws2s 目标缓冲区不足，字符串被截断。");
        result = dest;
    }
    else
    {
        assert(!"UniCore ws2s 转换Unicode字符串到MBCS字符串时失败。");
        OutputDebugStringA("UniCore ws2s 转换Unicode字符串到MBCS字符串时失败。");
    }
    delete []dest;
    return result;
}

//! 将std::string转换为std::wstring.
/*!
    根据指定代码页将std::string转换为std::wstring.
    \param s 源字符串.
    \param codepage 转换时采用的代码页.
    \return 转换后的unicode字串.
*/
inline std::wstring s2ws( const std::string &s,int codepage )
{
    using namespace std;
    wstring result;

    int requiredSize = MultiByteToWideChar(codepage,0,s.c_str(),s.size(),0,0);
    if(requiredSize == 0)
    {
        DebugMessage("UniCore s2ws 转换字符串时发生错误.Last Error:%d",GetLastError());
        return result;
    }
    wchar_t *dest = new wchar_t[requiredSize];
    int numWritten = MultiByteToWideChar(codepage,0,s.c_str(),s.size(),dest,requiredSize);
    if(numWritten == 0)
    {
        delete []dest;
        DebugMessage("UniCore s2ws 转换字符串时发生错误.Last Error:%d",GetLastError());
        return result;
    }

    result.assign(dest,requiredSize);  //dest中保存的字符串不是以0结尾.
    delete []dest;

    return result;
}

//! 将std::wstring转换为std::string.
/*!
    根据指定代码页将wstring转换为string.
    \param ws utf-16编码的字符串.
    \param codepage 根据哪个代码页来转换.
    \return 转换后的字符串.
*/
inline std::string ws2s( const std::wstring &ws,int codepage )
{
    using namespace std;
    string result;

    int requiredSize = WideCharToMultiByte(codepage,0,ws.c_str(),ws.size(),0,0,0,0);
    if(requiredSize == 0)
    {
        DebugMessage("UniCore ws2s 转换字符串时发生错误.Last Error:%d",GetLastError());
        return result;
    }
    char *dest = new char[requiredSize];
    int numWritten = WideCharToMultiByte(codepage,0,ws.c_str(),ws.size(),dest,requiredSize,0,0);
    if(numWritten == 0)
    {
        delete []dest;
        DebugMessage("UniCore ws2s 转换字符串时发生错误.Last Error:%d",GetLastError());
        return result;
    }

    result.assign(dest,requiredSize);  //dest中保存的字符串不是以0结尾.
    delete []dest;

    return result;
}

//! 分割字符串.
/*!
    \param s 源字符串,被分割的字符串.
    \param delim 分隔符.这里的分隔符可能不止一个字符,而可以是字符串.
	\return 分割后的子字符串组成的数组.

    分割字符串s.每遇到一次分隔符就生成一个子字符串并保存到结果数组中,
	空的子字符串会被跳过.

    \code
    std::string s = "Acc Psw Credit";
    std::vector<std::string> results = split(s);
	//以空格分割字符串s,results将包含"Acc","Psw","Credit"三个字符串.
    \endcode
*/
inline std::vector<std::string> split( const std::string &s,const std::string &delim /*= " "*/ )
{
	using namespace std;
	vector<string> results;
	if(s.empty() || delim.empty())
	{
		//源字符串为空或者分隔符为空都无法进行分割.
		return results;
	}
	if(s.size() <= delim.size())
	{
		//源字符串长度小于等于分隔符时也无法分割.
		return results;
	}
	string::size_type currentPos = 0;
	string::size_type delimPos = 0;
	while(true)
	{
		delimPos = s.find(delim,currentPos);
		if(delimPos == string::npos)
		{
			//从当前位置开始找不到分隔符,将剩余的字符串加入结果数组.
			results.push_back(s.substr(currentPos));
			break;
		}
		else
		{
			//找到分隔符
			assert(delimPos >= currentPos);
			if(delimPos > currentPos)
			{
				//分隔符位置如果等于当前位置,则子字符串为空.
				results.push_back(s.substr(currentPos,delimPos-currentPos));
			}
			currentPos = delimPos+delim.length();  //当前位置指向分隔符后面一个位置.
			if(currentPos >= s.length())
			{
				//currentPos已经超出字符串范围.
				break;
			}
		}

	}
	return results;
}

//! 分割字符串.
/*!
    \param s 源字符串,被分割的字符串.
    \param delim 分隔符.这里的分隔符可能不止一个字符,而可以是字符串.
	\return 分割后的子字符串组成的数组.

    分割字符串s.每遇到一次分隔符就生成一个子字符串并保存到结果数组中,
	空的子字符串会被跳过.

    \code
    std::wstring s = L"Acc Psw Credit";
    std::vector<std::wstring> results = split(s);
	//以空格分割字符串s,results将包含L"Acc",L"Psw",L"Credit"三个字符串.
    \endcode
*/
inline std::vector<std::wstring> split( const std::wstring &s,const std::wstring &delim /*= L" "*/ )
{
	using namespace std;
	vector<wstring> results;
	if(s.empty() || delim.empty())
	{
		//源字符串为空或者分隔符为空都无法进行分割.
		return results;
	}
	if(s.size() <= delim.size())
	{
		//源字符串长度小于等于分隔符时也无法分割.
		return results;
	}
	wstring::size_type currentPos = 0;
	wstring::size_type delimPos = 0;
	while(true)
	{
		delimPos = s.find(delim,currentPos);
		if(delimPos == wstring::npos)
		{
			//从当前位置开始找不到分隔符,将剩余的字符串加入结果数组.
			results.push_back(s.substr(currentPos));
			break;
		}
		else
		{
			//找到分隔符
			assert(delimPos >= currentPos);
			if(delimPos > currentPos)
			{
				//分隔符位置如果等于当前位置,则子字符串为空.
				results.push_back(s.substr(currentPos,delimPos-currentPos));
			}
			currentPos = delimPos+delim.length();  //当前位置指向分隔符后面一个位置.
			if(currentPos >= s.length())
			{
				//currentPos已经超出字符串范围.
				break;
			}
		}

	}
	return results;

}

//! 从字符串的首尾剔除指定字符.
/*!
    \param s 处理的字符串.
    \paran trim 要剔除的字符集合,默认为空格.
    该函数从s的首尾剔除字符,任何出现在字符串两端的trimChars中包含的字符都会被剔除.
    s为空字符串时也将返回空字符串.
    \code
    std::string s = "    this is a string with space.    ";
    std::string trimed = trim(s);  //trimed: "this is a string with space."
    trimed = trim(s," .");  //trimed: "this is a string with space"  这里空格和'.'都被剔除了.
    \endcode
*/
inline std::string trim(const std::string &s,const std::string &trimChars = " ")
{
    using namespace std;
    std::string result = s;
    // Remove char at the end.
    while(!result.empty())
    {
        if(trimChars.find(result.back()) != string::npos)
        {
            result.pop_back();
        }
        else
        {
            break;
        }
    }
    // Remove char at the beginning.
    while(!result.empty())
    {
        if(trimChars.find(result.front()) != string::npos)
        {
            result.erase(result.begin());
        }
        else
        {
            break;
        }
    }
    return result;
}

//! 从字符串的首尾剔除指定字符.
/*!
    \param s 处理的字符串.
    \paran trim 要剔除的字符集合,默认为空格.
    该函数从s的首尾剔除字符,任何出现在字符串两端的trimChars中包含的字符都会被剔除.
    s为空字符串时也将返回空字符串.
    \code
    std::wstring s = L"    this is a string with space.    ";
    std::wstring trimed = trim(s);  //trimed: L"this is a string with space."
    trimed = trim(s,L". ");  //trimed: L"this is a string with space"  这里空格和'.'都被剔除了.
    \endcode
*/
inline std::wstring trim( const std::wstring &s,const std::wstring &trimChars = L" " )
{
    using namespace std;
    std::wstring result = s;
    // Remove char at the end.
    while(!result.empty())
    {
        if(trimChars.find(result.back()) != wstring::npos)
        {
            result.pop_back();
        }
        else
        {
            break;
        }
    }
    // Remove char at the beginning.
    while(!result.empty())
    {
        if(trimChars.find(result.front()) != wstring::npos)
        {
            result.erase(result.begin());
        }
        else
        {
            break;
        }
    }
    return result;
}

//! 简单的锁.
class ULock
{
public:
    ULock():atomic_(0) {}
    ~ULock() {}
    //! 上锁.
    void lock() 
    {
        do 
        {
            long prev = InterlockedCompareExchange(&atomic_,1,0);
            if(atomic_ == 1 && prev == 0)
            {
                break;
            }
            if(!SwitchToThread())
            {
                Sleep(1);
            }
        } while (true);
    }
    //! 解锁.
    void unlock()
    {
        InterlockedCompareExchange(&atomic_,0,1);
    }
private:
    ULock(const ULock&);
    ULock& operator=(const ULock&);
    volatile long atomic_;
};

//! 区域锁.
/*!
    声明的时候加锁,变量销毁时(退出作用域)解锁.
*/
class UScopedLock
{
public:
    explicit UScopedLock(ULock &ulock)
        :lock_(ulock)
    {
        lock_.lock();
    }
    ~UScopedLock()
    {
        lock_.unlock();
    }
private:
    UScopedLock(const UScopedLock&);
    UScopedLock& operator=(const UScopedLock&);
    ULock &lock_;
};


#endif//UNICORE_ULITE_H