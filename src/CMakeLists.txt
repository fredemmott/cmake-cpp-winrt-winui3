# This is a really really ugly proof of concept that needs refactoring
get_filename_component(WIN10_SDK_ROOT  "[HKEY_LOCAL_MACHINE\\SOFTWARE\\Microsoft\\Windows Kits\\Installed Roots;KitsRoot10]" ABSOLUTE CACHE)
file(GLOB WIN10_SDK_VERSIONS RELATIVE "${WIN10_SDK_ROOT}/UnionMetadata" "${WIN10_SDK_ROOT}/UnionMetadata/10.*")
list(GET WIN10_SDK_VERSIONS 0 WIN10_SDK_VERSION)
foreach(VERSION ${WIN10_SDK_VERSIONS})
  if(${VERSION} VERSION_GREATER ${WIN10_SDK_VERSION})
    set(WIN10_SDK_VERSION ${VERSION})
  endif()
endforeach()
message(STATUS "Using Win10 SDK version ${WIN10_SDK_VERSION}")
set(WIN10_METADATA_DIR "${WIN10_SDK_ROOT}/UnionMetadata/${WIN10_SDK_VERSION}")

function(add_winmd TARGET IDL_SOURCE)
  get_filename_component(BASENAME "${IDL_SOURCE}" NAME_WE)
  set(OUTFILE "${CMAKE_CURRENT_BINARY_DIR}/${BASENAME}.winmd")
  string(REPLACE "/" "\\" OUTFILE_BACKSLASH "${OUTFILE}")
  string(REPLACE "/" "\\" IDL_SOURCE_BACKSLASH "${CMAKE_CURRENT_SOURCE_DIR}/${IDL_SOURCE}")
  string(REPLACE "/" "\\" WIN10_METADATA_DIR_BACKSLASH "${WIN10_METADATA_DIR}")
  add_custom_command(
    OUTPUT "${OUTFILE}"
    COMMAND
      WindowsSDK::midlrt
        /winrt
        /winmd "${OUTFILE_BACKSLASH}"
        /metadata_dir "${WIN10_METADATA_DIR_BACKSLASH}"
        /nomidl
        /reference "${WINDOWS_APP_SDK_DIR}/lib/uap10.0/Microsoft.UI.Xaml.winmd"
        "${IDL_SOURCE_BACKSLASH}"
    MAIN_DEPENDENCY "${IDL_SOURCE}"
  )
  add_custom_target(
    "${TARGET}"
    DEPENDS "${OUTFILE}"
  )
endfunction()

add_winmd(MyPage.winmd MyPage.idl)
add_cppwinrt(DemoApp-winrt MyPage.winmd)

add_executable(DemoApp WIN32 main.cpp app.manifest)
target_link_libraries(
  DemoApp
  WindowsAppSDK
  DemoApp-winrt
)
target_include_directories(DemoApp PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/include")
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
  DEPENDS DemoApp
)
