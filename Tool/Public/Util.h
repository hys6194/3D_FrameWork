#pragma once

#include "Tool_Defines.h"

class Util
{
public:
	static std::string	WstrToStr(const std::wstring& wide_str);
	static std::wstring	StrToWstr(const std::string& narrow_str);
	static wstring		Get_FileName(const wstring& path);
};

