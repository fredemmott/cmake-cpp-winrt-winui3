add_executable(DemoApp WIN32 main.cpp app.manifest)
target_link_libraries(
  DemoApp
  CppWinRT-Lib
  WindowsAppSDK
  #WindowsAppRuntime-MddBootstrapAutoInitializer
)
set_target_properties(
  DemoApp
  PROPERTIES
  CXX_STANDARD 17
  CXX_STANDARD_REQUIRED ON
  CXX_EXTENSIONS OFF)

# Make it so it can be ran from the build directory
add_custom_target(
  CopyRuntimeFiles
  ALL
  COMMAND
  ${CMAKE_COMMAND} -E copy_if_different
  "$<TARGET_RUNTIME_DLLS:DemoApp>"
  "${CMAKE_CURRENT_SOURCE_DIR}/App.xaml"
  "$<TARGET_FILE_DIR:DemoApp>"
  COMMAND_EXPAND_LISTS
)
