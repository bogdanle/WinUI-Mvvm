#include "pch.h"
#include "RelayCommand.h"
#if __has_include("RelayCommand.g.cpp")
#include "RelayCommand.g.cpp"
#endif

using namespace winrt;
using namespace Windows::Foundation;

namespace winrt::WinUI_Mvvm::implementation
{
	RelayCommand::RelayCommand(std::function<void(IInspectable)> const& execute) :
		RelayCommand(execute, nullptr)
	{
	}

	RelayCommand::RelayCommand(std::function<void(IInspectable)> const& execute, std::function<bool(IInspectable)> const& canExecute)
	{
		if (execute == nullptr) 
		{
			throw hresult_invalid_argument(L"execute");
		}
		
		_executeDelegate = execute;
		_canExecuteDelegate = canExecute;
	}

	void RelayCommand::RaiseCanExecuteChanged()
	{
		_canExecuteChanged(*this, IInspectable());
	}

	event_token RelayCommand::CanExecuteChanged(EventHandler<IInspectable> const& handler)
	{
		return _canExecuteChanged.add(handler);
	}

	void RelayCommand::CanExecuteChanged(event_token const& token)
	{
		_canExecuteChanged.remove(token);
	}

	bool RelayCommand::CanExecute(IInspectable const& parameter)
	{
		if (_canExecuteDelegate == nullptr) 
		{
			return true;
		}

		bool canExecute = _canExecuteDelegate(parameter);

		if (_lastCanExecute != canExecute) 
		{
			_lastCanExecute = canExecute;
			RaiseCanExecuteChanged();
		}

		return _lastCanExecute;
	}

	void RelayCommand::Execute(IInspectable const& parameter)
	{
		if (_executeDelegate != nullptr) 
		{
			_executeDelegate(parameter);
		}
	}
}
