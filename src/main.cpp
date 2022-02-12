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

struct MyApp : public winrt::Microsoft::UI::Xaml::ApplicationT<MyApp>
{
  void OnLaunched(const winrt::Microsoft::UI::Xaml::LaunchActivatedEventArgs &)
  {
    Resources().MergedDictionaries().Append(
        winrt::Microsoft::UI::Xaml::Controls::XamlControlsResources());
  }
};

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
{
  winrt::init_apartment(winrt::apartment_type::single_threaded);

  winrt::Microsoft::UI::Xaml::Application::Start(
    [](auto &&){ winrt::make<MyApp>(); });
  return 0;
}
