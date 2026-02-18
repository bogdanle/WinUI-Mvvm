#include "pch.h"
#include "SettingsPage.xaml.h"
#if __has_include("SettingsPage.g.cpp")
#include "SettingsPage.g.cpp"
#endif

#include "SettingsViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::WinUI_Mvvm::implementation
{
    SettingsPage::SettingsPage()
    {
        auto vm = winrt::make<SettingsViewModel>();
        ViewModel(vm);
    }
}
