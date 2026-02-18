#pragma once

#include <windows.h>

#include "Helpers/WindowPos.h"

class WindowHelper
{
public:
    static void SetWindowIcon(HWND hwnd, int iconId);
    static void SetWindowPos(HWND hwnd, int left, int top, int width, int height, int state);
    static void CenterWindow(HWND hwnd);
    static void CenterWindow(HWND hwnd, int width, int height);
    static void TopRightWindow(HWND hwnd, int width, int height);    
    static float ScalingFactor(HWND hwnd);

private:
    static const int DefaultWindowWidth;
    static const int DefaultWindowHeight;
};
