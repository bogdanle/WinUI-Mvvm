#include "pch.h"

#include <windows.system.h>
#include <dispatcherqueue.h>
#include <winrt/base.h>
#include <winrt/Windows.System.h>

#include "WindowsSystemDispatcherQueueHelper.h"

void WindowsSystemDispatcherQueueHelper::EnsureWindowsSystemDispatcherQueueController()
{
    // Check if a DispatcherQueue already exists for the current thread
    if (winrt::Windows::System::DispatcherQueue::GetForCurrentThread() != nullptr)
    {
        return; // One already exists
    }

    // Create the DispatcherQueueController if not already created
    if (!_dispatcherQueueController)
    {
        DispatcherQueueOptions options = {};
        options.dwSize = sizeof(DispatcherQueueOptions);
        options.threadType = DQTYPE_THREAD_CURRENT; // 2
        options.apartmentType = DQTAT_COM_STA;      // 2

        winrt::com_ptr<IDispatcherQueueController> controller;
        HRESULT hr = CreateDispatcherQueueController(options, reinterpret_cast<PDISPATCHERQUEUECONTROLLER*>(controller.put_void()));

        if (SUCCEEDED(hr))
        {
            _dispatcherQueueController = std::move(controller);
        }
        else
        {
            throw winrt::hresult_error(hr);
        }
    }
}
