#pragma once

#include "SettingsViewModel.g.h"

#include "Core/BindableBase.h"

namespace winrt::WinUI_Mvvm::implementation
{
    struct SettingsViewModel : SettingsViewModelT<SettingsViewModel, BindableBase>
    {
        SettingsViewModel() = default;

        bool IsLightTheme() const { return _isLightTheme; }
        void IsLightTheme(bool value);
        bool IsDarkTheme() const { return _isDarkTheme; }
        void IsDarkTheme(bool value);
        bool IsDefaultTheme() const { return _isDefaultTheme; }
        void IsDefaultTheme(bool value);

    private:
        bool _isLightTheme{ false };
        bool _isDarkTheme{ false };
        bool _isDefaultTheme{ true };

        void ApplyTheme();
    };
}

namespace winrt::WinUI_Mvvm::factory_implementation
{
    struct SettingsViewModel : SettingsViewModelT<SettingsViewModel, implementation::SettingsViewModel>
    {
    };
}
