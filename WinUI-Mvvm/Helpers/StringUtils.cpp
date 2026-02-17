#include "pch.h"

#include <locale>
#include <codecvt>
#include <algorithm>
#include <string>

#include "Helpers/StringUtils.h"

std::wstring to_lower(const std::wstring& str)
{
    std::wstring result = str;
    std::ranges::transform(result, result.begin(),
                           [](wchar_t c) { return static_cast<wchar_t>(std::tolower(c)); });
    return result;

}

std::string to_lower(const std::string& str)
{
    std::string result = str;
    std::ranges::transform(result, result.begin(),
                           [](unsigned char c) { return static_cast<unsigned char>(std::tolower(c)); });
    return result;
}

std::string to_string(const wchar_t* wstr, bool lowercase)
{
    return to_string(std::wstring(wstr), lowercase);
}

std::wstring to_wstring(const std::string& str, bool lowercase)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return lowercase ? to_lower(conv.from_bytes(str)) : conv.from_bytes(str);
}

std::string to_string(const winrt::hstring& hstr)
{
    // Step 1: Extract the underlying wide string from the hstring
    std::wstring wstr = hstr.c_str();

    // Step 2: Use WideCharToMultiByte to convert from UTF-16 to UTF-8
    int size_needed = WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), nullptr, 0, nullptr, nullptr);
    std::string str(size_needed, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.c_str(), static_cast<int>(wstr.size()), str.data(), size_needed, nullptr, nullptr);

    return str;
}

std::string to_string(const std::wstring& wstr, bool lowercase)
{
    // Convert wstring to string
    std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
    return lowercase ? to_lower(conv.to_bytes(wstr)) : conv.to_bytes(wstr);
}
