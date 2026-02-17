#pragma once

#include "HomePage.g.h"

namespace winrt::WinUI_Mvvm::implementation
{
    struct HomePage : HomePageT<HomePage>
    {
        HomePage();
        void InitializeComponent();

        WinUI_Mvvm::HomeViewModel ViewModel() const { return _viewModel; }
        void ViewModel(WinUI_Mvvm::HomeViewModel const& value) { _viewModel = value; }

    private:
        WinUI_Mvvm::HomeViewModel _viewModel{ nullptr };
        Microsoft::UI::Dispatching::DispatcherQueue _dispatcher{ nullptr };
    };
}

namespace winrt::WinUI_Mvvm::factory_implementation
{
    struct HomePage : HomePageT<HomePage, implementation::HomePage>
    {
    };
}
