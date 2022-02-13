#include <Unknwn.h>
#undef GetCurrentTime
#include <winrt/base.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.XamlTypeInfo.h>
#include <winrt/Microsoft.Windows.ApplicationModel.DynamicDependency.h>
#include <winrt/Windows.Foundation.Collections.h>

// Undocumented static initializer for MddBootstrap; using it instead of documented
// APIs for now, to be closer to what Visual Studio does
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
    mNav.Header(winrt::box_value(L"CMake, C++/WinRT, and WinUI 3 Demo App"));
    mNav.Content(mContent);
    mNav.IsSettingsVisible(false);

    muxc::NavigationViewItem item1;
    item1.Content(winrt::box_value(L"Item 1"));
    item1.Icon(muxc::SymbolIcon(muxc::Symbol::Play));
    mNav.MenuItems().Append(item1);

    muxc::NavigationViewItem item2;
    item2.Content(winrt::box_value(L"Item 2"));
    item2.Icon(muxc::SymbolIcon(muxc::Symbol::Copy));
    mNav.MenuItems().Append(item2);

    muxc::NavigationViewItem item3;
    item3.Content(winrt::box_value(L"Item 3"));
    item3.Icon(muxc::SymbolIcon(muxc::Symbol::Paste));
    mNav.MenuItems().Append(item3);

    mNav.IsSettingsVisible(true);

    Content(mNav);
  }
};

struct MyApp : public winrt::Microsoft::UI::Xaml::ApplicationT<MyApp, winrt::Microsoft::UI::Xaml::Markup::IXamlMetadataProvider>
{
private:
  winrt::Microsoft::UI::Xaml::XamlTypeInfo::XamlControlsXamlMetaDataProvider mXamlControlsMetaDataProvider;

public:
  MyApp()
  {
    ::winrt::Windows::Foundation::Uri resourceLocator{L"ms-appx:///App.xaml"};
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

  winrt::Microsoft::UI::Xaml::Markup::IXamlType
  GetXamlType(const winrt::hstring& fullName)
  {
    return mXamlControlsMetaDataProvider.GetXamlType(fullName);
  }

  winrt::Microsoft::UI::Xaml::Markup::IXamlType
  GetXamlType(const winrt::Windows::UI::Xaml::Interop::TypeName& type)
  {
    return mXamlControlsMetaDataProvider.GetXamlType(type);
  }

  winrt::com_array<winrt::Microsoft::UI::Xaml::Markup::XmlnsDefinition> GetXmlnsDefinitions()
  {
    return mXamlControlsMetaDataProvider.GetXmlnsDefinitions();
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
