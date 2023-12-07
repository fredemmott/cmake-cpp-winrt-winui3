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

# START NEW STUFF

include(FetchContent)

function(FetchContent_Declare_nuget NAME)
  set(FUN_OPTS "") 
  set(FUN_SINGLE_ARG NAME VERSION)
  set(FUN_MULTI_ARG "")
  cmake_parse_arguments(ARG "${FUN_OPTS}" "${FUN_SINGLE_ARG}" "${FUN_MULTI_ARG}" ${ARGN})

  FetchContent_Declare(
    "${NAME}"
    URL "https://www.nuget.org/api/v2/package/${ARG_NAME}/${ARG_VERSION}"
    
    EXCLUDE_FROM_ALL
    DOWNLOAD_EXTRACT_TIMESTAMP ON
  )
endfunction()

set(WINDOWS_SDK_VERSION 10.0.22621.0)

# C++/WinRT

FetchContent_Declare_nuget(
  cppwinrt 
  NAME "Microsoft.Windows.CppWinRT"
  VERSION "2.0.230706.1"
)
FetchContent_MakeAvailable(cppwinrt)

set(CPPWINRT_EXE "${cppwinrt_SOURCE_DIR}/bin/cppwinrt.exe")
message(STATUS "cppwinrt.exe: ${CPPWINRT_EXE}")

set(CPPWINRT_WINDOWS_SDK_DIR "${cppwinrt_BINARY_DIR}/include")
add_custom_command(
  OUTPUT "${CPPWINRT_WINDOWS_SDK_DIR}/winrt/base.h"
  COMMAND
  "${CPPWINRT_EXE}"
  -input "${WINDOWS_SDK_VERSION}+"
  -reference "${WINDOWS_SDK_VERSION}+"
  -output "${CPPWINRT_WINDOWS_SDK_DIR}"
  -optimize
  -base
)
add_custom_target(
  CppWinRT-WindowsSDK-gen
  DEPENDS
  "${CPPWINRT_WINDOWS_SDK_DIR}/winrt/base.h"
  EXCLUDE_FROM_ALL
)
add_library(CppWinRT-WindowsSDK INTERFACE)
add_dependencies(CppWinRT-WindowsSDK CppWinRT-WindowsSDK-gen)
target_include_directories(CppWinRT-WindowsSDK INTERFACE "${CPPWINRT_WINDOWS_SDK_DIR}")

# ABI Tool

FetchContent_Declare_nuget(
  abiwinrt
  NAME "Microsoft.Windows.AbiWinRT"
  VERSION "2.0.210330.2"
)
FetchContent_MakeAvailable(abiwinrt)
set(ABI_EXE "${abiwinrt_SOURCE_DIR}/bin/abi.exe")
message(STATUS "abi.exe: ${ABI_EXE}")

# Windows App SDK

FetchContent_Declare_nuget(
  windowsappsdk
  NAME "Microsoft.WindowsAppSDK"
  VERSION "1.4.231115000"
)
FetchContent_MakeAvailable(windowsappsdk)
set(WINDOWS_APP_SDK_CPPWINRT_DIR "${windowsappsdk_BINARY_DIR}/include")
set(WINDOWS_APP_SDK_WINMD_DIR "${windowsappsdk_BINARY_DIR}/winmd")
file(
  GLOB_RECURSE
  WINMD_SOURCES
  "${windowsappsdk_SOURCE_DIR}/lib/**/*.winmd"
)
file(COPY ${WINMD_SOURCES} DESTINATION "${WINDOWS_APP_SDK_WINMD_DIR}")
add_custom_command(
  OUTPUT "${WINDOWS_APP_SDK_CPPWINRT_DIR}/winrt/Microsoft.UI.h"
  COMMAND
  "${CPPWINRT_EXE}"
  -input "${WINDOWS_APP_SDK_WINMD_DIR}"
  -output "${WINDOWS_APP_SDK_CPPWINRT_DIR}"
  -reference "${WINDOWS_SDK_VERSION}+"
  -optimize
)
add_custom_target(
  WindowsAppSDK-CppWinRT
  DEPENDS
  "${WINDOWS_APP_SDK_CPPWINRT_DIR}/winrt/Microsoft.UI.h"
  EXCLUDE_FROM_ALL
)
set(WINDOWS_APP_SDK_ABI_DIR "${windowsappsdk_BINARY_DIR}/abi/include")
add_custom_command(
  OUTPUT "${WINDOWS_APP_SDK_ABI_DIR}/Microsoft.Foundation.h"
  COMMAND
  "${ABI_EXE}"
  -input "${WINDOWS_APP_SDK_WINMD_DIR}"
  -output "${WINDOWS_APP_SDK_ABI_DIR}"
  -reference "${WINDOWS_SDK_VERSION}+"
  -reference "${WINDOWS_APP_SDK_WINMD_DIR}"
  -ns-prefix
  -lowercase-include-guard
  -enable-header-deprecation
)
add_custom_target(
  WindowsAppSDK-ABI
  DEPENDS
  "${WINDOWS_APP_SDK_ABI_DIR}/Microsoft.Foundation.h"
  EXCLUDE_FROM_ALL
)

# Remove bogus "<file name="" />" entry
set(MANIFESTS_IN "${windowsappsdk_SOURCE_DIR}/manifests")
set(MANIFESTS_OUT "${windowsappsdk_BINARY_DIR}/manifests")
set(BROKEN_MANIFEST_IN "${MANIFESTS_IN}/Microsoft.WindowsAppSdk.Foundation.manifest")
set(BROKEN_MANIFEST_OUT "${MANIFESTS_OUT}/Microsoft.WindowsAppSdk.Foundation.manifest")
file(READ "${BROKEN_MANIFEST_IN}" CONTENT)
string(REPLACE "<file name=\"\" />" "" CONTENT "${CONTENT}")
file(WRITE "${BROKEN_MANIFEST_OUT}" "${CONTENT}")
set(WINDOWS_APP_SDK_MANIFEST "${MANIFESTS_OUT}/merged.manifest")

find_program(MT_EXE "mt.exe" REQUIRED)
message(STATUS "mt.exe: ${MT_EXE}")
add_custom_command(
  OUTPUT "${WINDOWS_APP_SDK_MANIFEST}"
  COMMAND
  "${MT_EXE}"
  -manifest
  "${MANIFESTS_IN}/manifests/Microsoft.WindowsAppSdk.WinUI.manifest"
  "${MANIFESTS_IN}/Microsoft.InteractiveExperiences.manifest"
  "${MANIFESTS_OUT}/Microsoft.WIndowsAppSdk.Foundation.manifest"
  "-out:${WINDOWS_APP_SDK_MANIFEST}"
)
add_custom_target(
  WindowsAppSDK-manifest
  DEPENDS
  "${WINDOWS_APP_SDK_MANIFEST}"
  EXCLUDE_FROM_ALL
)

# Extract the MSIX (TODO: ARM)
if(CMAKE_SIZEOF_VOID_P EQUAL 8)
  set(WINDOWS_APP_SDK_MSIX_IN_DIR "${windowsappsdk_SOURCE_DIR}/tools/MSIX/win10-x64")
else()
  set(WINDOWS_APP_SDK_MSIX_IN_DIR "${windowsappsdk_SOURCE_DIR}/tools/MSIX/win10-x86")
endif()
set(WINDOWS_APP_SDK_MSIX_DIR "${windowsappsdk_BINARY_DIR}/msix")
file(MAKE_DIRECTORY "${WINDOWS_APP_SDK_MSIX_DIR}")
# Globbing instead of reading the manifest to match what msbuild does
file(GLOB MSIX_SOURCES "${WINDOWS_APP_SDK_MSIX_IN_DIR}/Microsoft.WindowsAppRuntime.*.msix")
foreach(MSIX_SOURCE ${MSIX_SOURCES})
  execute_process(
    COMMAND
    "${CMAKE_COMMAND}" -E tar xf "${MSIX_SOURCE}"
    WORKING_DIRECTORY
    "${WINDOWS_APP_SDK_MSIX_DIR}"
  )
endforeach()

add_library(
  WindowsAppSDK INTERFACE
)
add_dependencies(WindowsAppSDK WindowsAppSDK-CppWinRT WindowsAppSDK-ABI WindowsAppSDK-manifest)
target_include_directories(WindowsAppSDK INTERFACE "${WINDOWS_APP_SDK_CPPWINRT_DIR}" "${WINDOWS_APP_SDK_ABI_DIR}")
target_link_libraries(WindowsAppSDK INTERFACE CppWinRT-WindowsSDK)


# START LEGACY STUFF # 
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