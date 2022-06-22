#include "DemoPage.xaml.h"
#if __has_include("DemoPage.g.cpp")
#include "DemoPage.g.cpp"
#endif

#include <winrt/Microsoft.UI.Input.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>

using namespace winrt::Microsoft::UI::Input;

namespace winrt::DemoApp::implementation {

DemoPage::DemoPage() noexcept {
  InitializeComponent();

  auto weakThis = get_weak();

  mDQC = DispatcherQueueController::CreateOnDedicatedThread();
  mDQC.DispatcherQueue().TryEnqueue([weakThis]() noexcept {
    auto strongThis = weakThis.get();
    if (!strongThis) {
      return;
    }
    strongThis->SwapChainPanel().CreateCoreIndependentInputSource(
      InputPointerSourceDeviceKinds::Mouse | InputPointerSourceDeviceKinds::Pen
      | InputPointerSourceDeviceKinds::Touch);
  });
}

void DemoPage::OnNavigatedTo(
  const Microsoft::UI::Xaml::Navigation::NavigationEventArgs& args
) noexcept {
  Title().Text(winrt::unbox_value<winrt::hstring>(args.Parameter()));
}

}
