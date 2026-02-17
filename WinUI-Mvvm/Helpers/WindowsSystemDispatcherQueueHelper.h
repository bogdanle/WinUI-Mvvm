#pragma once

#include <dispatcherqueue.h>
#include <windows.system.h>

using namespace winrt::Windows::System;

class WindowsSystemDispatcherQueueHelper
{
public:
    void EnsureWindowsSystemDispatcherQueueController();

private:
    winrt::com_ptr<IDispatcherQueueController> _dispatcherQueueController;
};
