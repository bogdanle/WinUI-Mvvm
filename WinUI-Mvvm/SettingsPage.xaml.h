#pragma once

#include "SettingsPage.g.h"

namespace winrt::WinUI_Mvvm::implementation
{
    struct SettingsPage : SettingsPageT<SettingsPage>
    {
        SettingsPage();     

        WinUI_Mvvm::SettingsViewModel ViewModel() const { return _viewModel; }
        void ViewModel(WinUI_Mvvm::SettingsViewModel const& value) { _viewModel = value; }

    private:
        WinUI_Mvvm::SettingsViewModel _viewModel{ nullptr };
    };
}

namespace winrt::WinUI_Mvvm::factory_implementation
{
    struct SettingsPage : SettingsPageT<SettingsPage, implementation::SettingsPage>
    {
    };
}
