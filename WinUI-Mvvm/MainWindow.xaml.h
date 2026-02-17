#pragma once

#include "MainWindow.g.h"

#include <windows.system.h>
#include <winrt/Windows.System.h>
#include <winrt/Windows.Storage.Streams.h>
#include <winrt/Windows.Media.Capture.Frames.h>
#include <winrt/Microsoft.UI.Composition.SystemBackdrops.h>

#include "Helpers/WindowsSystemDispatcherQueueHelper.h"

namespace winrt::WinUI_Mvvm::implementation
{
    using namespace winrt;
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml;
    using namespace Microsoft::UI::Xaml::Input;
    using namespace Microsoft::UI::Xaml::Controls;
    using namespace Microsoft::UI::Composition::SystemBackdrops;    

    struct MainWindow : MainWindowT<MainWindow>
    {
        MainWindow();

        void InitializeComponent();

        // Event handlers
        IAsyncAction Window_Activated(IInspectable const&, WindowActivatedEventArgs args);
        void Window_Closed(IInspectable const&, WindowEventArgs args);        
        void NavigationView_SelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args);

    private:
        bool _initialized = false;
        hstring _errorMessage = {};
        Microsoft::UI::Dispatching::DispatcherQueue _dispatcher{ nullptr };
        DesktopAcrylicController _acrylicController{ nullptr };
        SystemBackdropConfiguration _configurationSource{ nullptr };
        WindowsSystemDispatcherQueueHelper _wsdqHelper;
        
        void Window_ThemeChanged(FrameworkElement const&, IInspectable const&);
        void SetConfigurationSourceTheme() const;
        bool SetAcrylicBackdrop(bool useAcrylicThin);
        HWND GetWindowHandle() const;
        static void ConfigureWindow(HWND hwnd);
    };

}

namespace winrt::WinUI_Mvvm::factory_implementation
{
    struct MainWindow : MainWindowT<MainWindow, implementation::MainWindow>
    {
    };
}
