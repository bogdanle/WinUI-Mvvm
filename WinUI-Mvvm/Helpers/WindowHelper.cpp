#include "pch.h"

#include <psapi.h>
#include <winrt/Windows.Graphics.Display.h>

#include "Helpers/WindowHelper.h"

const int WindowHelper::DefaultWindowWidth = 1100;
const int WindowHelper::DefaultWindowHeight = 720;

#pragma comment(lib, "Psapi.lib")

using namespace winrt;

float WindowHelper::ScalingFactor(HWND hwnd)
{
    return static_cast<float>(::GetDpiForWindow(hwnd)) / 96.0f;
}

void WindowHelper::SetWindowPos(HWND hwnd, int left, int top, int width, int height, int state)
{
    double scaling = ScalingFactor(hwnd);

    Windows::Graphics::SizeInt32 size;
    size.Height = static_cast<int>(scaling * height);
    size.Width = static_cast<int>(scaling * width);

    ::SetWindowPos(hwnd, nullptr, left, top, width, height, SWP_NOZORDER);
    ::ShowWindow(hwnd, state);
}

void WindowHelper::CenterWindow(HWND hwnd)
{
    CenterWindow(hwnd, DefaultWindowWidth, DefaultWindowHeight);
}

void WindowHelper::CenterWindow(HWND hwnd, int width, int height)
{
    double scaling = ::GetDpiForWindow(hwnd) / 96.0;

    winrt::Windows::Graphics::SizeInt32 size;
    size.Height = static_cast<int>(scaling * height);
    size.Width = static_cast<int>(scaling * width);
        
    // Retrieve display information
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

    // Get the monitor dimensions
    int monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;
    int monitorHeight = monitorInfo.rcMonitor.bottom - monitorInfo.rcMonitor.top;

    auto left = (monitorWidth - size.Width) / 2;
    auto top = (monitorHeight - size.Height) / 2;
        
    ::SetWindowPos(hwnd, nullptr, left, top, size.Width, size.Height, SWP_NOZORDER);    
}

void WindowHelper::SetWindowIcon(HWND hwnd, int iconId)
{
    HICON hIconSmall = static_cast<HICON>(LoadImage(
        GetModuleHandle(nullptr),
        MAKEINTRESOURCE(iconId), // Resource identifier of the icon
        IMAGE_ICON,
        16, 16,  // Desired size for the small icon
        LR_DEFAULTCOLOR));

    if (hIconSmall)
    {
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIconSmall));
    }

    HICON hIconBig = static_cast<HICON>(LoadImage(
        GetModuleHandle(nullptr),
        MAKEINTRESOURCE(iconId), // Resource identifier of the icon
        IMAGE_ICON,
        32, 32,  // Desired size for the small icon
        LR_DEFAULTCOLOR));

    if (hIconBig)
    {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIconBig));
    }
}

void WindowHelper::TopRightWindow(HWND hwnd, int width, int height)
{
    double scaling = ::GetDpiForWindow(hwnd) / 96.0;

    winrt::Windows::Graphics::SizeInt32 size;
    size.Height = static_cast<int>(scaling * height);
    size.Width = static_cast<int>(scaling * width);

    // Retrieve display information
    MONITORINFO monitorInfo;
    monitorInfo.cbSize = sizeof(MONITORINFO);
    GetMonitorInfo(MonitorFromWindow(hwnd, MONITOR_DEFAULTTOPRIMARY), &monitorInfo);

    // Get the monitor dimensions
    int monitorWidth = monitorInfo.rcMonitor.right - monitorInfo.rcMonitor.left;

    int xMargin = static_cast<int>(15 * scaling);
    int yMargin = static_cast<int>(20 * scaling);
    auto left = (monitorWidth - size.Width) - xMargin;
    auto top = yMargin;

    ::SetWindowPos(hwnd, nullptr, left, top, size.Width, size.Height, SWP_NOZORDER);    
}
