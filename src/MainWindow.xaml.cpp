#include "MainWindow.xaml.h"
#if __has_include("MainWindow.g.cpp")
#include "MainWindow.g.cpp"
#endif

namespace winrt::DemoApp::implementation {

MainWindow::MainWindow() {
  InitializeComponent();

  auto arg = box_value(to_hstring(L"Hello, world"));
  Frame().Navigate(xaml_typename<DemoPage>(), arg);
  Frame().Navigate(xaml_typename<DemoPage>(), arg);
}

void MainWindow::Navigate(
  const IInspectable& sender,
  const Microsoft::UI::Xaml::Controls::NavigationViewItemInvokedEventArgs& args) noexcept {
  if (args.IsSettingsInvoked()) {
    // TODO
    return;
  }

  auto item = args.InvokedItemContainer().try_as<
    Microsoft::UI::Xaml::Controls::NavigationViewItem
  >();

  if (!item) {
    // FIXME: show an error?
    return;
  }

  // TODO: you probably want to use item.Tag() to identify a specific item
  Frame().Navigate(xaml_typename<DemoPage>(), item.Content());
}

void MainWindow::GoBack(
  const IInspectable& sender,
  const Microsoft::UI::Xaml::Controls::NavigationViewBackRequestedEventArgs&) noexcept {
  Frame().GoBack();
}

}
