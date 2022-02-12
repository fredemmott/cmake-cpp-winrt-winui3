#include <Unknwn.h>
#undef GetCurrentTime
#include <winrt/base.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Microsoft.Windows.ApplicationModel.DynamicDependency.h>
#include <winrt/Windows.UI.Xaml.Hosting.h>

// Undocumented static initializer for MddBootstrap; using it instead of documented
// APIs for now,
#include <MddBootstrapAutoInitializer.cpp>

namespace muxc = winrt::Microsoft::UI::Xaml::Controls;

struct MainWindow : public winrt::Microsoft::UI::Xaml::WindowT<MainWindow>
{
private:
  muxc::NavigationView mNav;
  muxc::Frame mContent;

public:
  MainWindow()
  {
    mNav.Header(winrt::box_value(L"Header"));
    mNav.Content(mContent);
    mNav.IsSettingsVisible(false);

    muxc::NavigationViewItem item1;
    item1.Content(winrt::box_value(L"Item 1"));
    item1.Icon(muxc::SymbolIcon(muxc::Symbol::Play));

    mNav.MenuItems().Append(item1);
    // This fails due to Microsoft.UI.Xaml.Controls.XamlControlResources not
    // being loaded, but loading it fails :(
    Content(mNav);
  }
};

struct MyApp : public winrt::Microsoft::UI::Xaml::ApplicationT<MyApp>
{
  MyApp()
  {
    ::winrt::Windows::Foundation::Uri resourceLocator{ L"ms-appx:///App.xaml" };
    ::winrt::Microsoft::UI::Xaml::Application::LoadComponent(*this, resourceLocator);

    UnhandledException([this](IInspectable const &, winrt::Microsoft::UI::Xaml::UnhandledExceptionEventArgs const &e)
                       {
        if (IsDebuggerPresent()) {
            auto errorMessage = e.Message();
            __debugbreak();
        } });
  }

  void OnLaunched(const winrt::Microsoft::UI::Xaml::LaunchActivatedEventArgs &)
  {
    // Exception thrown at 0x00007FFA07C74F69 (KernelBase.dll) in DemoApp.exe: WinRT originate error - 0x80004005 : 'Cannot find a resource with the given key: AcrylicBackgroundFillColorDefaultBrush.'.
    // ... if it werent' for that exception, I'd add it to `Resources()` here
    winrt::make<MainWindow>().Activate();
  }
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  winrt::init_apartment(winrt::apartment_type::single_threaded);

  winrt::Microsoft::UI::Xaml::Application::Start(
      [](auto &&)
      { winrt::make<MyApp>(); });
  return 0;
}
