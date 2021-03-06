﻿#include "UCommon.h"

#define WIN32_LEAN_AND_MEAN
#include "windows.h"

#include <sstream>
#include <time.h>
#include <cassert>
#include <algorithm>
#include <locale>
#include <objbase.h>

#pragma comment(lib,"rpcrt4.lib")

using namespace std;

namespace uni
{

//GetRandomName会从这个数组中取词.
static const char *g_wordsForGetRandomName[] = {
"Angel",
"Apple",
"Agent",
"Another",
"Bad",
"Bubble",
"Cloudy",
"Cobra",
"Dark",
"Devine",
"Ghost",
"Good",
"Kuso",
"Lucky",
"QX",
"Orange",
"Rider",
"Rock",
"Sadllly",
"Shadowy",
"Sunny",
"Warm",
"X",
"XX",
"Zero",
"Zebra"
};

std::string GetRandomRoleName(int minSize /*= 0*/,int maxSize /*= 0*/,int wordCount /*= 1*/,int randomSuffixCount /*= 0*/)
{
    int totalWordCount = (sizeof(g_wordsForGetRandomName) / sizeof(g_wordsForGetRandomName[0]));
    static bool flag = false;
    if(!flag)
    {
        srand((unsigned int)time(0));
        flag = true;
    }
    
    //生成wordCount个随机的词语.
    vector<string> words;
    int currentNameLength = 0;  
    for(int i = 0; i < wordCount; i++)
    {
        words.push_back(g_wordsForGetRandomName[rand()%totalWordCount]);
        currentNameLength += words[i].length();
    }

    currentNameLength += randomSuffixCount;
    if(currentNameLength < minSize || currentNameLength > maxSize)
    {
        //长度不合适,重新生成.
        return GetRandomRoleName(minSize,maxSize,wordCount,randomSuffixCount);
    }

    //生成随机的数字后缀.
    if(randomSuffixCount > 255)
    {
        randomSuffixCount = 255;
    }
    char randomSuffix[256] = "";

    for(int i = 0; i < randomSuffixCount; i++)
    {
        randomSuffix[i] = 48+rand()%10;
    }

    string result;
    for(int i = 0; i < words.size(); i++)
    {
        result += words[i];
    }
    result += randomSuffix;

    return result;
}

vector<string> split( const string &s,const string &delim /*= " "*/ )
{

    vector<string> results;
    if(s.empty() || delim.empty())
    {
        //源字符串为空或者分隔符为空都无法进行分割.
        return results;
    }
    if(s.size() < delim.size())
    {
		//源字符串长度小于分隔符长度时也无法分割.
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
            if(delimPos >= currentPos)
            {
                //分隔符位置如果等于当前位置,则保存一个空字符串.
                results.push_back(s.substr(currentPos,delimPos-currentPos));
            }
            currentPos = delimPos+delim.length();  //当前位置指向分隔符后面一个位置.
        }

    }
    return results;
}

std::vector<std::wstring> split( const std::wstring &s,const std::wstring &delim /*= L" "*/ )
{

	vector<wstring> results;
	if(s.empty() || delim.empty())
	{
		//源字符串为空或者分隔符为空都无法进行分割.
		return results;
	}
	if(s.size() < delim.size())
	{
		//源字符串长度小于分隔符长度时也无法分割.
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
			if(delimPos >= currentPos)
			{
                //分隔符位置如果等于当前位置,则保存一个空字符串.
				results.push_back(s.substr(currentPos,delimPos-currentPos));
			}
			currentPos = delimPos+delim.length();  //当前位置指向分隔符后面一个位置.
		}

	}
	return results;

}

std::string trim(const std::string &s,const std::string &t /*= " "*/)
{
    std::string result = s;
    if(t.empty())
    {
      return s;
    }
    // Remove from the beginning.
    while(true)
    {
      size_t pos = result.find(t);
      if(pos != std::string::npos 
        && pos == 0)
      {
        result.erase(0,t.size());
      }
      else
      {
        break;
      }
    }

    // Remove from the end.
    while(true)
    {
      size_t pos = result.rfind(t);
      if(pos != std::string::npos
        && pos + t.size() == result.size())
      {
        result.erase(pos,t.size());
      }
      else
      {
        break;
      }
    }

    return result;
}

std::wstring trim( const std::wstring &s,const std::wstring &t /*= L" "*/ )
{
    std::wstring result = s;
    if(t.empty())
    {
      return s;
    }
    // Remove from the beginning.
    while(true)
    {
      size_t pos = result.find(t);
      if(pos != std::wstring::npos 
        && pos == 0)
      {
        result.erase(0,t.size());
      }
      else
      {
        break;
      }
    }

    // Remove from the end.
    while(true)
    {
      size_t pos = result.rfind(t);
      if(pos != std::wstring::npos
        && pos + t.size() == result.size())
      {
        result.erase(pos,t.size());
      }
      else
      {
        break;
      }
    }
    return result;
}

std::wstring ltrim( const std::wstring &s,const std::wstring &t /*= L" "*/ )
{
  std::wstring result = s;
  if(t.empty())
  {
    return s;
  }
  // Remove from the beginning.
  while(true)
  {
    size_t pos = result.find(t);
    if(pos != std::wstring::npos 
      && pos == 0)
    {
      result.erase(0,t.size());
    }
    else
    {
      break;
    }
  }
  return result;
}

std::wstring rtrim( const std::wstring &s,const std::wstring &t /*= L" "*/ )
{
  std::wstring result = s;
  if(t.empty())
  {
    return s;
  }
  // Remove from the end.
  while(true)
  {
    size_t pos = result.rfind(t);
    if(pos != std::wstring::npos
      && pos + t.size() == result.size())
    {
      result.erase(pos,t.size());
    }
    else
    {
      break;
    }
  }
  return result;
}

std::wstring pad( const std::wstring &s,const std::wstring &p )
{
  std::wstring result = s;
  size_t pos = result.find(p);
  if(pos == std::wstring::npos 
    || pos != 0)
  {
    result = p + result;
  }

  pos = result.rfind(p);
  if(pos == std::wstring::npos
    || pos + p.size() != result.size())
  {
    result = result + p;
  }
  return result;
}

std::wstring lpad(const std::wstring &s, const std::wstring &p)
{
  std::wstring result = s;
  size_t pos = result.find(p);
  if(pos == std::wstring::npos 
    || pos != 0)
  {
    result = p + result;
  }
  return result;
}

std::wstring rpad( const std::wstring &s,const std::wstring &p )
{
  std::wstring result = s;
  size_t pos = result.rfind(p);
  if(pos == std::wstring::npos
    || pos + p.size() != result.size())
  {
    result = result + p;
  }
  return result;
}

std::string join( const std::vector<std::string> &stringsToJoin,const std::string &delim )
{
    std::string result;
    for(int i = 0; i < stringsToJoin.size(); i++)
    {
        result += stringsToJoin[i];
        if(i != stringsToJoin.size() - 1)
        {
            result += delim;
        }
    }
    return result;
}

std::wstring join( const std::vector<std::wstring> &stringsToJoin,const std::wstring &delim )
{
    std::wstring result;
    for(int i = 0; i < stringsToJoin.size(); i++)
    {
        result += stringsToJoin[i];
        if(i != stringsToJoin.size() - 1)
        {
            result += delim;
        }
    }
    return result;
}

bool contains( const std::wstring &src, const std::wstring &pattern, CaseSensitivity caseSensitivity /*= CaseSensitive*/ )
{
    if(caseSensitivity == CaseSensitive)
    {
        return src.find(pattern) != std::wstring::npos;
    }
    std::wstring srcLowerCase = src;
    std::transform(src.begin(), src.end(), srcLowerCase.begin(),tolower);
    std::wstring patternLowerCase = pattern;
    std::transform(pattern.begin(),pattern.end(),patternLowerCase.begin(),tolower);

    return srcLowerCase.find(patternLowerCase) != std::wstring::npos;
}

bool contains( const std::string &src, const std::string &pattern, CaseSensitivity caseSensitivity /*= CaseSensitive*/ )
{
    if(caseSensitivity == CaseSensitive)
    {
        return src.find(pattern) != std::string::npos;
    }
    std::string srcLowerCase = src;
    std::transform(src.begin(), src.end(), srcLowerCase.begin(), tolower);
    std::string patternLowerCase = pattern;
    std::transform(pattern.begin(),pattern.end(),patternLowerCase.begin(),tolower);

    return srcLowerCase.find(patternLowerCase) != std::string::npos;
}

bool starts_with( const std::wstring &src, const std::wstring &pattern, CaseSensitivity caseSensitivity /*= CaseSensitive*/ )
{
    if(caseSensitivity == CaseSensitive)
    {
        return src.find(pattern) == 0;
    }
    
    std::wstring srcLowerCase = src;
    std::transform(src.begin(), src.end(), srcLowerCase.begin(),tolower);
    std::wstring patternLowerCase = pattern;
    std::transform(pattern.begin(),pattern.end(),patternLowerCase.begin(),tolower);

    return srcLowerCase.find(patternLowerCase) == 0;
}

bool starts_with( const std::string &src, const std::string &pattern, CaseSensitivity caseSensitivity /*= CaseSensitive*/ )
{
    if(caseSensitivity == CaseSensitive)
    {
        return src.find(pattern) == 0;
    }

    std::string srcLowerCase = src;
    std::transform(src.begin(), src.end(), srcLowerCase.begin(), tolower);
    std::string patternLowerCase = pattern;
    std::transform(pattern.begin(),pattern.end(),patternLowerCase.begin(),tolower);

    return srcLowerCase.find(patternLowerCase) == 0;
}

std::wstring GetRandomAlnumName( int length )
{
  std::wstring result;
  static bool init = false;
  if(!init)
  {
    srand((unsigned)time(0));
    init = true;
  }
  for(int i = 0; i < length; i++)
  {
    int u = (double)rand()/(RAND_MAX + 1)*36;
    if(u >= 0 && u < 10)
    {
      u += '0';
    }
    else if(u >= 10 && u < 36)
    {
      u -= 10;
      u += 'a';
    }
    result.push_back(u);
  }
  return result;
}

std::wstring GenerateUUID()
{
    GUID guid;

    if(CoCreateGuid(&guid) != S_OK)
    {
        return L"";
    }

    // Convert the GUID to a string
    wchar_t *guidString = 0;
    if(UuidToString(&guid,(RPC_WSTR*)&guidString) != RPC_S_OK)
    {
        return L"";
    }

    std::wstring result = guidString;

    RpcStringFree((RPC_WSTR*)&guidString);

    return result;
}

std::wstring to_lower( const std::wstring &ws )
{
	std::wstring result = ws;
	transform(result.begin(),result.end(),result.begin(),towlower);
	return result;
}

std::string to_lower( const std::string &s )
{
	std::string result = s;
	transform(result.begin(),result.end(),result.begin(),tolower);
	return result;
}

std::wstring to_upper( const std::wstring &ws )
{
	std::wstring result = ws;
	transform(result.begin(),result.end(),result.begin(),towupper);
	return result;
}

std::string to_upper( const std::string &s )
{
	std::string result = s;
	transform(result.begin(),result.end(),result.begin(),toupper);
	return result;
}



}//namespace uni