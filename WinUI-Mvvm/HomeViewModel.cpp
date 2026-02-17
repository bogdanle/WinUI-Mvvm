#include "pch.h"
#include "HomeViewModel.h"
#if __has_include("HomeViewModel.g.cpp")
#include "HomeViewModel.g.cpp"
#endif

#include <regex>

#include "Core/RelayCommand.h"

namespace winrt::WinUI_Mvvm::implementation
{
    HomeViewModel::HomeViewModel()
    {
        _submitCommand = make<RelayCommand>(
            [this](IInspectable const&) { this->OnSubmit(); },
            [this](IInspectable const&) -> bool { return this->CanSubmit(); });

        _dispatcher = winrt::Microsoft::UI::Dispatching::DispatcherQueue::GetForCurrentThread();
    }

    IAsyncAction HomeViewModel::OnSubmit()
    {
        IsBusy(true);
        IsSuccess(false);
        IsFailure(false);
        bool success = false;

        IsBusy(false);

        // Raise the event
        _submitFinishedEvent(*this, success);

        co_return;
    }

    void HomeViewModel::ValidateForm()
    {
        // Validate email
        std::wstring email = _email.c_str();

        // Define a regex pattern for email validation
        const std::wregex emailRegex(LR"((^[^\s@]+@[^\s@]+\.[^\s@]+$))");
        _isEmailInvalid = !std::regex_match(email, emailRegex);

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
