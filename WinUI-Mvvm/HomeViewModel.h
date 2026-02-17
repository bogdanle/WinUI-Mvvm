#pragma once

#include "HomeViewModel.g.h"

#include "Core/BindableBase.h"

#include <winrt/Microsoft.UI.Xaml.Input.h>

namespace winrt::WinUI_Mvvm::implementation
{
    using namespace winrt;
    using namespace Windows::Foundation;
    using namespace Microsoft::UI::Xaml::Data;
    using namespace Microsoft::UI::Xaml::Input;    

    struct HomeViewModel : HomeViewModelT<HomeViewModel, BindableBase>
    {
        HomeViewModel();

        hstring Email() const { return _email; }
        void Email(hstring const& value)
        {
            if (_email != value)
            {
                _email = value;
                BindableBase::NotifyPropertyChanged(L"Email");
                ValidateForm();
            }
        }

        hstring Name() const { return _name; }
        void Name(hstring const& value)
        {
            if (_name != value)
            {
                _name = value;
                NotifyPropertyChanged(L"Name");
                ValidateForm();
            }
        }

        ICommand SubmitCommand() const { return _submitCommand; }
        bool CanSubmit() const { return _isSubmitEnabled; }
        IAsyncAction OnSubmit();

        bool IsEmailInvalid() const { return _isEmailInvalid; }
        bool IsNameInvalid() const { return _isNameInvalid; }
        bool IsBusy() const { return _isBusy; }
        void IsBusy(bool value)
        {
            _isBusy = value;
            NotifyPropertyChanged(L"IsBusy");
            ValidateForm();
        }

        bool IsSuccess() const { return _isSuccess; }
        void IsSuccess(bool value)
        {
            _isSuccess = value;
            NotifyPropertyChanged(L"IsSuccess");
        }

        bool IsFailure() const { return _isFailure; }
        void IsFailure(bool value)
        {
            _isFailure = value;
            NotifyPropertyChanged(L"IsFailure");
        }

        // Event
        event_token SubmitFinished(EventHandler<bool> const& handler) { return _submitFinishedEvent.add(handler); }
        void SubmitFinished(event_token const& token) noexcept { _submitFinishedEvent.remove(token); }

    private:
        hstring _email;
        hstring _name;
        bool _isSubmitEnabled{ false };
        bool _isEmailInvalid{ true };
        bool _isNameInvalid{ true };
        bool _isBusy{ false };
        bool _isSuccess{ false };
        bool _isFailure{ false };
        winrt::WinUI_Mvvm::RelayCommand _submitCommand{ nullptr };
        winrt::Microsoft::UI::Dispatching::DispatcherQueue _dispatcher{ nullptr };
        event<EventHandler<bool>> _submitFinishedEvent;

        void ValidateForm();
    };
}

namespace winrt::WinUI_Mvvm::factory_implementation
{
    struct HomeViewModel : HomeViewModelT<HomeViewModel, implementation::HomeViewModel>
    {
    };
}
