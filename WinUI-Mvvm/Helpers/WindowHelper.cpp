#include "pch.h"

#include <psapi.h>
#include <winrt/Windows.Graphics.Display.h>

#include "Helpers/WindowHelper.h"

const int WindowHelper::DefaultWindowWidth = 1100;
const int WindowHelper::DefaultWindowHeight = 725;

#pragma comment(lib, "Psapi.lib")

using namespace winrt;

void SetWindowIcon(HWND hwnd)
{
    HICON hIconSmall = static_cast<HICON>(LoadImage(
        GetModuleHandle(nullptr),
        MAKEINTRESOURCE(101), // Resource identifier of the icon
        IMAGE_ICON,
        16, 16,  // Desired size for the small icon
        LR_DEFAULTCOLOR));

    if (hIconSmall)
    {
        SendMessage(hwnd, WM_SETICON, ICON_SMALL, reinterpret_cast<LPARAM>(hIconSmall));
    }

    HICON hIconBig = static_cast<HICON>(LoadImage(
        GetModuleHandle(nullptr),
        MAKEINTRESOURCE(101), // Resource identifier of the icon
        IMAGE_ICON,
        32, 32,  // Desired size for the small icon
        LR_DEFAULTCOLOR));

    if (hIconBig)
    {
        SendMessage(hwnd, WM_SETICON, ICON_BIG, reinterpret_cast<LPARAM>(hIconBig));
    }
}

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

    SetWindowIcon(hwnd);
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

    SetWindowIcon(hwnd);
}

static bool iequals(std::wstring a, std::wstring b) 
{
    auto tl = [](wchar_t c) { return (wchar_t)towlower(c); };
    std::transform(a.begin(), a.end(), a.begin(), tl);
    std::transform(b.begin(), b.end(), b.begin(), tl);
    return a == b;
}

static std::wstring basename(const std::wstring& path) 
{
    size_t p = path.find_last_of(L"\\/");
    return (p == std::wstring::npos) ? path : path.substr(p + 1);
}

static bool ProcessMatches(DWORD pid, const std::wstring& baseNameNoExt)
{
    HANDLE hProc = OpenProcess(PROCESS_QUERY_LIMITED_INFORMATION | PROCESS_VM_READ, FALSE, pid);
    if (!hProc)
    {
        std::cout << "OpenProcess failed for PID " << pid << ", error " << GetLastError() << std::endl;
        return false;
    }

    wchar_t modName[MAX_PATH] = { 0 };
    DWORD n = GetModuleBaseNameW(hProc, nullptr, modName, MAX_PATH); // main module
    CloseHandle(hProc);
    if (n == 0) return false;

    std::wstring base = basename(modName);          // e.g., "IPROOVUI.exe"
    std::wstring noext = base;
    size_t dot = noext.rfind(L'.');
    if (dot != std::wstring::npos) noext = noext.substr(0, dot);

    return iequals(noext, baseNameNoExt);
}

struct FindCtx 
{
    std::wstring Caption = L"WinUI Desktop";
    std::wstring Class = L"WinUIDesktopWin32WindowClass";
    std::wstring Process = L"IPROOVUI"; // compare without extension
    HWND Handle = nullptr;
};

static BOOL CALLBACK EnumProc(HWND hwnd, LPARAM lParam)
{
    FindCtx* ctx = reinterpret_cast<FindCtx*>(lParam);

    // Skip invisible/minimized checks if you want; leaving them out is safer.
    wchar_t cls[256] = { 0 };
    if (GetClassNameW(hwnd, cls, (int)(std::size(cls))) == 0) return TRUE;
    if (!iequals(cls, ctx->Class)) return TRUE;

    wchar_t title[512] = { 0 };
    GetWindowTextW(hwnd, title, (int)(std::size(title)));
    if (!iequals(title, ctx->Caption)) return TRUE;
    DWORD pid = 0;
    GetWindowThreadProcessId(hwnd, &pid);
    if (pid == 0) return TRUE;

    if (!ProcessMatches(pid, ctx->Process)) return TRUE;

    ctx->Handle = hwnd;
    return FALSE; // stop enumeration
}

/// <summary>
/// Finds the main application window (for iProovUI process).
/// Must be called with Admin privileges, otherwise will always return nullptr.
/// </summary>
/// <returns></returns>
HWND WindowHelper::FindAppWindow()
{
    FindCtx ctx;
    EnumWindows(EnumProc, (LPARAM)&ctx);
    return ctx.Handle;
}
