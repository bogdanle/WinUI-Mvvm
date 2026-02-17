#include "pch.h"
#include "HomeViewModel.h"
#if __has_include("HomeViewModel.g.cpp")
#include "HomeViewModel.g.cpp"
#endif

#include <chrono>
#include <thread>

#include "Core/RelayCommand.h"

namespace winrt::WinUI_Mvvm::implementation
{
    HomeViewModel::HomeViewModel()
    {
        _submitCommand = make<RelayCommand>(
            [this](IInspectable const&) { this->OnSubmit(); },
            [this](IInspectable const&) -> bool { return this->CanSubmit(); });

        // Define a regex pattern for email validation
        _emailRegex = std::wregex(LR"((^[^\s@]+@[^\s@]+\.[^\s@]+$))");

        _dispatcher = winrt::Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();
    }

    IAsyncAction HomeViewModel::OnSubmit()
    {
        IsBusy(true);
        IsSuccess(false);
        IsFailure(false);
        bool success = false;

        co_await winrt::resume_after(std::chrono::seconds(2));
        success = true; // Simulate a successful submission

        // Raise the event
        _submitFinishedEvent(*this, success);

        _dispatcher.TryEnqueue([this, success]()
        {
            IsBusy(false);
            if (success)
            {
                IsSuccess(true);
            }
            else
            {
                IsFailure(true);
            }
        });
    }

    void HomeViewModel::ValidateForm()
    {
        // Validate email
        _isEmailInvalid = !std::regex_match(_email.c_str(), _emailRegex);

        // Validate name
        _isNameInvalid = _name.size() < 2;

        // Enable submit button if both fields are valid
        _isSubmitEnabled = !_isEmailInvalid && !_isNameInvalid && !_isBusy;

        // Notify UI
        NotifyPropertyChanged(L"IsEmailInvalid");
        NotifyPropertyChanged(L"IsNameInvalid");
        _submitCommand.RaiseCanExecuteChanged();
    }
}
