#pragma once

#include <functional>

#include "RelayCommand.g.h"

namespace winrt::WinUI_Mvvm::implementation
{
    using namespace Windows::Foundation;

    struct RelayCommand : RelayCommandT<RelayCommand>
    {
        RelayCommand(std::function<void(IInspectable)> const& execute);
        RelayCommand(std::function<void(IInspectable)> const& execute, std::function<bool(IInspectable)> const& canExecute);
        
        winrt::event_token CanExecuteChanged(EventHandler<IInspectable> const& handler);

        void RaiseCanExecuteChanged();
        void CanExecuteChanged(winrt::event_token const& token);
        bool CanExecute(IInspectable const& parameter);
        void Execute(IInspectable const& parameter);

    private:
        event<EventHandler<IInspectable>> _canExecuteChanged{};
        std::function<void(IInspectable)> _executeDelegate;
        std::function<bool(IInspectable)> _canExecuteDelegate;
        bool _lastCanExecute = true;
    };
}
