#pragma once

#include "Helpers/WindowPos.h"

class WindowHelper
{
public:
    static void SetWindowPos(HWND hwnd, int left, int top, int width, int height, int state);
    static void CenterWindow(HWND hwnd);
    static void CenterWindow(HWND hwnd, int width, int height);
    static void TopRightWindow(HWND hwnd, int width, int height);    
    static float ScalingFactor(HWND hwnd);
    static HWND FindAppWindow();

private:
    static const int DefaultWindowWidth;
    static const int DefaultWindowHeight;
};
