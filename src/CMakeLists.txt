add_executable(
  DemoApp
  WIN32
  app.manifest
  App.xaml.cpp App.xaml App.idl
  MainWindow.xaml.cpp MainWindow.xaml MainWindow.idl
)
set_property(
  SOURCE App.xaml
  PROPERTY VS_XAML_TYPE
  "ApplicationDefinition"
)
set_property(
  TARGET DemoApp
  PROPERTY VS_PACKAGE_REFERENCES
  "Microsoft.Windows.CppWinRT_2.0.220608.4"
  "Microsoft.WindowsAppSDK_1.1.1"
  "Microsoft.Windows.SDK.BuildTools_10.0.22621.1"
)
set_target_properties(
  DemoApp
  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  VS_GLOBAL_RootNamespace DemoApp
  VS_GLOBAL_AppContainerApplication false
  VS_GLOBAL_AppxPackage false
  VS_GLOBAL_CppWinRTEnabled true
  VS_GLOBAL_CppWinRTOptimized true
  VS_GLOBAL_CppWinRTRootNamespaceAutoMerge true
  VS_GLOBAL_UseWinUI true
  VS_GLOBAL_ApplicationType "Windows Store"
  VS_GLOBAL_WindowsPackageType None
  VS_GLOBAL_EnablePreviewMsixTooling true
  # Skip the "ResolveNuGetPackageAssets" target as it fails with an empty sequence error
  # Not certain why - I *think* that none of the packages we use have assets
  VS_GLOBAL_ResolveNuGetPackages false
)
