include(ExternalProject)

ExternalProject_Add(
  WindowsAppSDKSource
  URL "https://www.nuget.org/api/v2/package/Microsoft.WindowsAppSDK/1.0.0"
  URL_HASH "SHA256=b5319bc52f99cf2f9bb75ae1a77dce08ec2e57738b9d5a3c3849335bcd11c8ee"
  DEPENDS CppWinRT-Exe

  CONFIGURE_COMMAND ""
  BUILD_COMMAND
    "$<TARGET_FILE:CppWinRT-Exe>"
    -in "<SOURCE_DIR>/lib/uap10.0.18362"
    -reference local
    -reference "<SOURCE_DIR>/lib/uap10.0"
    -output "<BINARY_DIR>/include"
  INSTALL_COMMAND ""

  EXCLUDE_FROM_ALL
)

ExternalProject_Get_property(WindowsAppSDKSource SOURCE_DIR)
ExternalProject_Get_property(WindowsAppSDKSource BINARY_DIR)
add_library(WindowsAppSDK INTERFACE)
add_dependencies(WindowsAppSDK WindowsAppSDKSource)
target_link_libraries(WindowsAppSDK INTERFACE CppWinRT-Lib)
target_include_directories(
  WindowsAppSDK
  INTERFACE
  "${SOURCE_DIR}/include"
  "${BINARY_DIR}/include"
)

add_library(WindowsAppRuntime-Bootstrap SHARED IMPORTED GLOBAL)
add_dependencies(WindowsAppRuntime-Bootstrap WindowsAppSDKSource)
set_target_properties(
  WindowsAppRuntime-Bootstrap
  PROPERTIES
  IMPORTED_IMPLIB "${SOURCE_DIR}/lib/win10-x64/Microsoft.WindowsAppRuntime.Bootstrap.lib"
  IMPORTED_LOCATION "${SOURCE_DIR}/runtimes/win10-x64/native/Microsoft.WindowsAppRuntime.Bootstrap.dll"
)
target_link_libraries(WindowsAppRuntime-Bootstrap INTERFACE WindowsAppSDK)

add_library(
  WindowsAppRuntime-MddBootstrapAutoInitializer
  STATIC
  "${SOURCE_DIR}/include/MddBootstrapAutoInitializer.cpp"
)
set_target_properties(
  WindowsAppRuntime-MddBootstrapAutoInitializer
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
)
target_link_libraries(
  WindowsAppRuntime-MddBootstrapAutoInitializer
  PRIVATE WindowsAppRuntime-Bootstrap
)


add_library(WindowsAppRuntime-MRM SHARED IMPORTED GLOBAL)
add_dependencies(WindowsAppRuntime-MRM WindowsAppSDKSource)
set_target_properties(
  WindowsAppRuntime-MRM
  PROPERTIES
  IMPORTED_IMPLIB "${SOURCE_DIR}/lib/win10-x64/MRM.lib"
)

add_library(WindowsAppRuntime SHARED IMPORTED GLOBAL)
add_dependencies(WindowsAppRuntime WindowsAppSDKSource)
set_target_properties(
  WindowsAppRuntime
  PROPERTIES
  IMPORTED_IMPLIB "${SOURCE_DIR}/lib/win10-x64/Microsoft.WindowsAppRuntime.lib"
)
target_link_libraries(WindowsAppRuntime INTERFACE WindowsAppRuntime-Bootstrap WindowsAppRuntime-MRM)
target_link_libraries(WindowsAppSDK INTERFACE WindowsAppRuntime)
