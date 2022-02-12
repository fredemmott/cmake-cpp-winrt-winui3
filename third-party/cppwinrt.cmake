include(ExternalProject)

ExternalProject_Add(
  cppwinrtBuild
  URL "https://github.com/microsoft/cppwinrt/releases/download/2.0.220131.2/Microsoft.Windows.CppWinRT.2.0.220131.2.nupkg"
  URL_HASH "SHA256=899c1c676c72404aea4c07ebd7e3314c245867f95918c79fc122642df85e168c"

  CONFIGURE_COMMAND ""
  BUILD_COMMAND "<SOURCE_DIR>/bin/cppwinrt.exe" -in local -output "<BINARY_DIR>/include"
  INSTALL_COMMAND ""

  EXCLUDE_FROM_ALL
)

ExternalProject_Get_property(cppwinrtBuild SOURCE_DIR)
add_executable(CppWinRT-Exe IMPORTED GLOBAL)
add_dependencies(CppWinRT-Exe cppwinrtBuild)
set_target_properties(CppWinRT-Exe PROPERTIES IMPORTED_LOCATION "${SOURCE_DIR}/bin/cppwinrt.exe")


add_library(CppWinRT-Lib INTERFACE)
add_dependencies(CppWinRT-Lib cppwinrtBuild)
target_link_libraries(CppWinRT-Lib INTERFACE WindowsApp.lib)

ExternalProject_Get_property(cppwinrtBuild BINARY_DIR)
target_include_directories(CppWinRT-Lib INTERFACE "${BINARY_DIR}/include")
