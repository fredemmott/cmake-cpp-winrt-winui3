# Patch up the generated vcxproj
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Directory.Build.props"
  "${CMAKE_CURRENT_BINARY_DIR}/Directory.Build.props"
  COPYONLY
)
configure_file(
  "${CMAKE_CURRENT_SOURCE_DIR}/Directory.Build.targets"
  "${CMAKE_CURRENT_BINARY_DIR}/Directory.Build.targets"
  COPYONLY
)

add_executable(
  DemoApp
  WIN32
  app.manifest
  App.xaml.cpp App.xaml.h App.xaml App.idl
  DemoPage.xaml.cpp DemoPage.xaml.h DemoPage.xaml DemoPage.idl
  MainWindow.xaml.cpp MainWindow.xaml.h MainWindow.xaml MainWindow.idl
)
set_property(
  SOURCE App.xaml
  PROPERTY VS_XAML_TYPE
  "ApplicationDefinition"
)
set_property(
  TARGET DemoApp
  PROPERTY VS_PACKAGE_REFERENCES
  "Microsoft.Windows.CppWinRT_2.0.230706.1"
  "Microsoft.WindowsAppSDK_1.4.231115000"
  "Microsoft.Windows.SDK.BuildTools_10.0.22621.756"
  "Microsoft.Windows.ImplementationLibrary_1.0.230629.1"
)
target_include_directories(
  DemoApp
  PRIVATE
  "${CMAKE_CURRENT_SOURCE_DIR}"
)
target_precompile_headers(
  DemoApp
  PRIVATE
  pch.h
)
set_target_properties(
  DemoApp
  PROPERTIES
  CXX_STANDARD 20
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF
  # ----- C++/WinRT, Windows App SDK, and WinUI stuff starts here -----
  VS_GLOBAL_RootNamespace DemoApp
  VS_GLOBAL_AppContainerApplication false
  VS_GLOBAL_AppxPackage false
  VS_GLOBAL_CppWinRTOptimized true
  VS_GLOBAL_CppWinRTRootNamespaceAutoMerge true
  VS_GLOBAL_UseWinUI true
  VS_GLOBAL_ApplicationType "Windows Store"
  VS_GLOBAL_WindowsPackageType None
  VS_GLOBAL_EnablePreviewMsixTooling true
  VS_GLOBAL_WindowsAppSDKSelfContained true
)

# Set source file dependencies properly for Xaml and non-Xaml IDL
# files.
#
# Without this, `module.g.cpp` will not include the necessary headers
# for non-Xaml IDL files, e.g. value converters
get_target_property(SOURCES DemoApp SOURCES)

foreach(SOURCE ${SOURCES})
  cmake_path(GET SOURCE EXTENSION LAST_ONLY EXTENSION)

  if(NOT "${EXTENSION}" STREQUAL ".idl")
    continue()
  endif()

  set(IDL_SOURCE "${SOURCE}")

  cmake_path(REMOVE_EXTENSION SOURCE LAST_ONLY OUTPUT_VARIABLE BASENAME)
  set(XAML_SOURCE "${BASENAME}.xaml")

  if("${XAML_SOURCE}" IN_LIST SOURCES)
    set_property(
      SOURCE "${IDL_SOURCE}"
      PROPERTY VS_SETTINGS
      "SubType=Code"
      "DependentUpon=${XAML_SOURCE}"
    )
  else()
    set_property(
      SOURCE "${IDL_SOURCE}"
      PROPERTY VS_SETTINGS
      "SubType=Code"
    )
    set_property(
      SOURCE "${BASENAME}.h"
      PROPERTY VS_SETTINGS
      "DependentUpon=${IDL_SOURCE}"
    )
    set_property(
      SOURCE "${BASENAME}.cpp"
      PROPERTY VS_SETTINGS
      "DependentUpon=${IDL_SOURCE}"
    )
  endif()
endforeach()