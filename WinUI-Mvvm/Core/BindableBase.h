#pragma once

#include "BindableBase.g.h"

#include <winrt/Microsoft.UI.Xaml.Data.h>

#include "Helpers/StringUtils.h"

namespace winrt::WinUI_Mvvm::implementation
{
    using namespace Microsoft::UI::Xaml::Data;

    struct BindableBase : BindableBaseT<BindableBase>
    {
        BindableBase() = default;

        void NotifyPropertyChanged(std::string const& propertyName)
        {
            _propertyChanged(*this, PropertyChangedEventArgs(to_wstring(propertyName)));
        }

        void NotifyPropertyChanged(hstring const& propertyName)
        {
            _propertyChanged(*this, PropertyChangedEventArgs(propertyName));
        }

        event_token PropertyChanged(PropertyChangedEventHandler const& value)
        {
            return _propertyChanged.add(value);
        }

        void PropertyChanged(winrt::event_token const& token) noexcept
        {
            _propertyChanged.remove(token);
        }

    private:
        event<PropertyChangedEventHandler> _propertyChanged;
    };
}

namespace winrt::WinUI_Mvvm::factory_implementation
{
    struct BindableBase : BindableBaseT<BindableBase, implementation::BindableBase>
    {
    };
}
