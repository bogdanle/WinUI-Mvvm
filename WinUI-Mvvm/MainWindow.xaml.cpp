#include "pch.h"
#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

#include <winrt/Windows.System.h>
#include <winrt/Windows.UI.h>
#include <winrt/Windows.UI.Xaml.Interop.h>
#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Windowing.h>
#include <winrt/Microsoft.UI.Xaml.Input.h>
#include <winrt/Microsoft.UI.Dispatching.h>
#include <microsoft.ui.xaml.window.h>

#include "Helpers/WindowHelper.h"

using namespace winrt;
using namespace Windows::Storage;
using namespace Windows::System;
using namespace Microsoft::UI::Xaml;
using namespace Microsoft::UI::Xaml::Controls;
using namespace Microsoft::UI::Xaml::Media;
using namespace Microsoft::UI::Dispatching;
using namespace Microsoft::UI::Xaml::Shapes;
using namespace Microsoft::UI::Xaml::Media::Imaging;
using namespace Microsoft::UI::Windowing;
using namespace Microsoft::UI::Composition::SystemBackdrops;


namespace winrt::WinUI_Mvvm::implementation
{
    MainWindow::MainWindow()
    {
    }

    void MainWindow::InitializeComponent()
    {
        // Call base InitializeComponent() to register with the Xaml runtime
        MainWindowT::InitializeComponent();

        _dispatcher = winrt::Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();

        SetAcrylicBackdrop(true);
    }

    bool MainWindow::SetAcrylicBackdrop(bool useAcrylicThin)
    {
        // Check if Desktop Acrylic is supported
        if (DesktopAcrylicController::IsSupported())
        {
            _wsdqHelper.EnsureWindowsSystemDispatcherQueueController();

            // Hooking up the configuration object
            _configurationSource = SystemBackdropConfiguration();

            // Access the XAML root element for theme change events
            auto frameworkElement = this->Content().try_as<FrameworkElement>();
            if (frameworkElement)
            {
                frameworkElement.ActualThemeChanged({ this, &MainWindow::Window_ThemeChanged });
            }

            // Initial configuration
            _configurationSource.IsInputActive(true);
            SetConfigurationSourceTheme();

            // Create and configure the Acrylic Controller
            _acrylicController = DesktopAcrylicController();
            _acrylicController.Kind(useAcrylicThin ? DesktopAcrylicKind::Thin : DesktopAcrylicKind::Base);

            // Enable the system backdrop
            auto compositionSupportsBackdrop = this->try_as<Microsoft::UI::Composition::ICompositionSupportsSystemBackdrop>();
            if (compositionSupportsBackdrop)
            {
                _acrylicController.AddSystemBackdropTarget(compositionSupportsBackdrop);
                _acrylicController.SetSystemBackdropConfiguration(_configurationSource);
            }

            return true;
        }

        return false;
    }

    void MainWindow::Window_ThemeChanged(FrameworkElement const&, IInspectable const&)
    {
        if (_configurationSource)
        {
            SetConfigurationSourceTheme();
        }
    }

    void MainWindow::SetConfigurationSourceTheme() const
    {
        if (_configurationSource)
        {
            auto frameworkElement = this->Content().try_as<FrameworkElement>();
            switch (frameworkElement.ActualTheme())
            {
            case ElementTheme::Dark:
                _configurationSource.Theme(SystemBackdropTheme::Dark);
                break;

            case ElementTheme::Light:
                _configurationSource.Theme(SystemBackdropTheme::Light);
                break;

            case ElementTheme::Default:
                _configurationSource.Theme(SystemBackdropTheme::Default);
                break;
            }
        }
    }

    HWND MainWindow::GetWindowHandle() const
    {
        HWND hwnd = nullptr;

        auto windowNative = this->try_as<IWindowNative>();
        if (windowNative != nullptr)
        {
            windowNative->get_WindowHandle(&hwnd);
        }

        return hwnd;
    }

    void MainWindow::ConfigureWindow(HWND hwnd)
    {
        // Get the current window style
        LONG_PTR style = GetWindowLongPtr(hwnd, GWL_STYLE);

        // Modify the style to remove minimize and maximize buttons and disable resizing
        style &= ~(WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU);
        style |= WS_SIZEBOX;

        // Set the new window style
        SetWindowLongPtr(hwnd, GWL_STYLE, style);
    }

    IAsyncAction MainWindow::Window_Activated(IInspectable const&, WindowActivatedEventArgs /*args*/)
    {
        if (!_initialized)
        {
            _initialized = true;            

            HWND hwnd = GetWindowHandle();
            auto pos = WindowPos::Parse(L"0,0,800,600,1");
            if (pos.has_value())
            {
                auto p = pos.value();
                WindowHelper::SetWindowPos(hwnd, p.Left, p.Top, p.Width, p.Height, p.State);
                WindowHelper::CenterWindow(hwnd);
            }

            navigationView().SelectedItem(navigationView().MenuItems().GetAt(0));
        }

        co_return;
    }

    void MainWindow::Window_Closed(IInspectable const&, WindowEventArgs /*args*/)
    {
        HWND hwnd = GetWindowHandle();
        WINDOWPLACEMENT state{};
        if (GetWindowPlacement(hwnd, &state))
        {
            WindowPos pos(state.rcNormalPosition, state.showCmd);
            // AppSettings::SaveWindowPos(WindowPos::ToString(pos));
        }

        if (_acrylicController)
        {
            _acrylicController.Close();
            _acrylicController = nullptr;
        }
    }

    void MainWindow::NavigationView_SelectionChanged(NavigationView const& sender, NavigationViewSelectionChangedEventArgs const& args)
    {
        UNREFERENCED_PARAMETER(sender);

        if (args.IsSettingsSelected())
        {
            contentFrame().Navigate(xaml_typename<SettingsPage>());

            auto selectedItem = args.SelectedItem().try_as<NavigationViewItem>();
            if (selectedItem)
            {
                navigationView().Header(selectedItem.Content());
            }
        }
        else
        {
            auto selectedItem = args.SelectedItem().try_as<NavigationViewItem>();
            if (selectedItem)
            {
                auto selectedItemTag = winrt::unbox_value<hstring>(selectedItem.Tag());
                std::wstring pageName = selectedItemTag.c_str(); // Convert to std::wstring

                static std::unordered_map<std::wstring, winrt::Windows::UI::Xaml::Interop::TypeName> pageTypeMap =
                {
                    { L"HomePage", xaml_typename<HomePage>() },                    
                };

                auto it = pageTypeMap.find(pageName);
                if (it != pageTypeMap.end())
                {
                    contentFrame().Navigate(it->second);
                    navigationView().Header(selectedItem.Content());
                }
                else
                {
                    OutputDebugStringW((L"Page not found " + pageName).c_str());
                }
            }
        }
    }
}
