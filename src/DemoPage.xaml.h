#pragma once

#include "pch.h"
#include "DemoPage.g.h"

#include <winrt/Microsoft.UI.Dispatching.h>
using namespace winrt::Microsoft::UI::Dispatching;

namespace winrt::DemoApp::implementation {
struct DemoPage : DemoPageT<DemoPage> {
  DemoPage() noexcept;

  void OnNavigatedTo(const Microsoft::UI::Xaml::Navigation::NavigationEventArgs&) noexcept;
  private:
  DispatcherQueueController mDQC { nullptr };
};
}

namespace winrt::DemoApp::factory_implementation {
struct DemoPage : DemoPageT<DemoPage, implementation::DemoPage> {};
}

