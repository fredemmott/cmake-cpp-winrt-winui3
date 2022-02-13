#include <Unknwn.h>
#undef GetCurrentTime
#include <winrt/base.h>
#include <winrt/Microsoft.UI.Xaml.h>
#include <winrt/Microsoft.UI.Xaml.Controls.h>
#include <winrt/Microsoft.UI.Xaml.Markup.h>
#include <winrt/Microsoft.UI.Xaml.Navigation.h>
#include <winrt/Microsoft.UI.Xaml.XamlTypeInfo.h>
#include <winrt/Microsoft.Windows.ApplicationModel.DynamicDependency.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.UI.Xaml.Interop.h>

#include <functional>

// Undocumented static initializer for MddBootstrap; using it instead of documented
// APIs for now, to be closer to what Visual Studio does
#include <MddBootstrapAutoInitializer.cpp>

namespace muxc = winrt::Microsoft::UI::Xaml::Controls;

struct MainWindow : public winrt::Microsoft::UI::Xaml::WindowT<MainWindow>
{
private:
  muxc::NavigationView mNav;

  muxc::Frame mContent;

  muxc::NavigationViewItem mNav1, mNav2, mNav3;

public:
  MainWindow()
  {
    Title(L"CMake, C++/WinRT, and WinUI 3 Demo App");

    mNav.Header(winrt::box_value(L"CMake, C++/WinRT, and WinUI 3 Demo App"));
    mNav.Content(mContent);
    mNav.IsSettingsVisible(false);

    mNav1.Content(winrt::box_value(L"Item 1"));
    mNav1.Icon(muxc::SymbolIcon(muxc::Symbol::Play));
    mNav.MenuItems().Append(mNav1);

    mNav2.Content(winrt::box_value(L"Item 2"));
    mNav2.Icon(muxc::SymbolIcon(muxc::Symbol::Copy));
    mNav.MenuItems().Append(mNav2);

    mNav3.Content(winrt::box_value(L"Item 3"));
    mNav3.Icon(muxc::SymbolIcon(muxc::Symbol::Paste));
    mNav.MenuItems().Append(mNav3);

    mNav.IsSettingsVisible(true);

    mNav.SelectionChanged({this, &MainWindow::OnSelectionChanged});

    mNav.SelectedItem(mNav1);
    Content(mNav);
  }

  void OnSelectionChanged(const IInspectable &, const muxc::NavigationViewSelectionChangedEventArgs &args)
  {
    if (args.IsSettingsSelected())
    {
      mContent.Content(winrt::box_value(L"Settings"));
      return;
    }

    const auto &item = args.SelectedItem();
    if (item == mNav1)
    {
      mContent.Content(winrt::box_value(L"Item 1"));
      return;
    }

    if (item == mNav2)
    {
      mContent.Content(winrt::box_value(L"Item 2"));
      return;
    }

    if (item == mNav3)
    {
      mContent.Content(winrt::box_value(L"Item 3"));
      return;
    }
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
    winrt::make<MainWindow>().Activate();
  }

  winrt::Microsoft::UI::Xaml::Markup::IXamlType
  GetXamlType(const winrt::hstring &fullName)
  {
    return mXamlControlsMetaDataProvider.GetXamlType(fullName);
  }

  winrt::Microsoft::UI::Xaml::Markup::IXamlType
  GetXamlType(const winrt::Windows::UI::Xaml::Interop::TypeName &type)
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
