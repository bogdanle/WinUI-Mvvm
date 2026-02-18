#pragma once

#include "App.xaml.g.h"

namespace winrt::WinUI_Mvvm::implementation
{
    struct App : AppT<App>
    {
        App();

        void OnLaunched(Microsoft::UI::Xaml::LaunchActivatedEventArgs const&);

        static App* Current() noexcept { return _current; }
        winrt::Microsoft::UI::Xaml::Window GetMainWindow() const noexcept { return _window; }

    private:
        winrt::Microsoft::UI::Xaml::Window _window{ nullptr };
        static App* _current;
    };
}
