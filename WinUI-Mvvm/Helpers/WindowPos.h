#pragma once

#include <string>
#include <sstream>
#include <optional>
#include <iostream>
#include <winuser.h>

struct WindowPos 
{
    WindowPos()
    {
        Left = 0;
        Top = 0;
        Width = 0;
        Height = 0;
        State = 0;
    }

    WindowPos(RECT rc, int state)
    {
        Left = rc.left;
        Top = rc.top;
        Width = rc.right - rc.left;
        Height = rc.bottom - rc.top;
        State = state;
    }

    int Left;
    int Top;
    int Width;
    int Height;
    int State;
    mutable std::wstring StateString;

    static std::optional<WindowPos> Parse(const std::wstring& input)
    {
        WindowPos pos;
        wchar_t delimiter;

        std::wistringstream iss(input);
        if ((iss >> pos.Left >> delimiter && delimiter == L',') &&
            (iss >> pos.Top >> delimiter && delimiter == L',') &&
            (iss >> pos.Width >> delimiter && delimiter == L',') &&
            (iss >> pos.Height >> delimiter && delimiter == L',') &&
            std::getline(iss, pos.StateString))
        {
            
            pos.State = SW_NORMAL;            
            if (pos.StateString == L"Minimized")
            {
                pos.State = SW_MINIMIZE;
            }
            else if (pos.StateString == L"Maximized")
            {
                pos.State = SW_MAXIMIZE;
            }                

            return pos;
        }

        return std::nullopt;
    }

    static std::wstring ToString(const WindowPos& pos)
    {
        switch (pos.State)
        {
            case SW_NORMAL:
                pos.StateString = L"Normal";
                break;

            case SW_MINIMIZE:       
                pos.StateString = L"Minimized";
                break;

            case SW_MAXIMIZE:
                pos.StateString = L"Maximized";
                break;
        }

        std::wostringstream oss;
        oss << pos.Left << ','
            << pos.Top << ','
            << pos.Width << ','
            << pos.Height << ','
            << pos.StateString;        
        return oss.str();
    }
};
