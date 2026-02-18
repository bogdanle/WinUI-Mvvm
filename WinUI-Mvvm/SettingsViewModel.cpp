#include "pch.h"
#include "SettingsViewModel.h"
#if __has_include("SettingsViewModel.g.cpp")
#include "SettingsViewModel.g.cpp"
#endif

#include "App.xaml.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::WinUI_Mvvm::implementation
{
    void SettingsViewModel::IsLightTheme(bool value)
    {
        if (_isLightTheme != value)
        {
            _isLightTheme = value;
            NotifyPropertyChanged(__func__);
            ApplyTheme();
        }
    }

    void SettingsViewModel::IsDarkTheme(bool value)
    {
        if (_isDarkTheme != value)
        {
            _isDarkTheme = value;
            NotifyPropertyChanged(__func__);
            ApplyTheme();
        }
    }

    void SettingsViewModel::IsDefaultTheme(bool value)
    {
        if (_isDefaultTheme != value)
        {
            _isDefaultTheme = value;
            NotifyPropertyChanged(__func__);
            ApplyTheme();
        }
    }

    void SettingsViewModel::ApplyTheme()
    {
        ElementTheme theme = ElementTheme::Default;
        if (_isDarkTheme)
        {
            theme = ElementTheme::Dark;
        }
        else if (_isLightTheme)
        {
            theme = ElementTheme::Light;
        }

        // Get the root element of the current window
        if (auto window = App::Current()->GetMainWindow())
        {
            if (auto content = window.Content().try_as<FrameworkElement>())
            {
                content.RequestedTheme(theme);
            }
        }
    }
}
