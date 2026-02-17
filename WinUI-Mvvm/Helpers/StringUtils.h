#pragma once

#include <windows.h>
#include <ntsecapi.h>
#include <string>
#include <winrt/Windows.Foundation.h>

std::string to_string(const winrt::hstring& hstr);
std::string to_string(const std::wstring& wstr, bool lowercase = false);
std::string to_string(const wchar_t* wstr, bool lowercase = false);
std::wstring to_wstring(const std::string& str, bool lowercase = false);
std::wstring to_lower(const std::wstring& str);
std::string to_lower(const std::string& str);
