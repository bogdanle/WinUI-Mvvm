#pragma once

#include "SettingsViewModel.g.h"

#include "Core/BindableBase.h"

namespace winrt::WinUI_Mvvm::implementation
{
    struct SettingsViewModel : SettingsViewModelT<SettingsViewModel, BindableBase>
    {
        SettingsViewModel() = default;
    };
}

namespace winrt::WinUI_Mvvm::factory_implementation
{
    struct SettingsViewModel : SettingsViewModelT<SettingsViewModel, implementation::SettingsViewModel>
    {
    };
}
