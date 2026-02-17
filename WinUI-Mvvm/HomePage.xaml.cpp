#include "pch.h"
#include "HomePage.xaml.h"
#if __has_include("HomePage.g.cpp")
#include "HomePage.g.cpp"
#endif

#include "HomeViewModel.h"

using namespace winrt;
using namespace Microsoft::UI::Xaml;

namespace winrt::WinUI_Mvvm::implementation
{
    HomePage::HomePage()
    {
        auto vm = winrt::make<HomeViewModel>();
        ViewModel(vm);

        // Subscribe to SubmitFinished event
        vm.SubmitFinished([this](auto const&, bool /*success*/)
            {
                _dispatcher.TryEnqueue([this]()
                    {
                        // WebcamPreview().IsFrozen(false);
                    });
            });
    }

    void HomePage::InitializeComponent()
    {
        // Call base InitializeComponent() to register with the Xaml runtime
        HomePageT::InitializeComponent();

        _dispatcher = Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();
    }
}
