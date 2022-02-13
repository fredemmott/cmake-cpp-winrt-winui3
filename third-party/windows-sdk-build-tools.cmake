include(FetchContent)
# Using FetchContent instead of ExternalProject_Add so that it is fetched at
# configure time, which lets us define variable targets that vary based on the
# contents.
FetchContent_Declare(
  windows_sdk_build_tools
  URL "https://www.nuget.org/api/v2/package/Microsoft.Windows.SDK.BuildTools/10.0.22000.197"
  URL_HASH "SHA256=8619a033bc5a7a86fad708fcb3496435662305e2061cb0a41352a87b608ee366"
)

FetchContent_MakeAvailable(windows_sdk_build_tools)

file(GLOB TOOLS "${windows_sdk_build_tools_SOURCE_DIR}/bin/10.0.22000.0/x64/*.exe")
foreach(TOOL_PATH ${TOOLS})
  get_filename_component(TOOL "${TOOL_PATH}" NAME_WE)
  set(TARGET "WindowsSDK::${TOOL}")
  add_executable("${TARGET}" IMPORTED GLOBAL)
  set_target_properties("${TARGET}" PROPERTIES IMPORTED_LOCATION "${TOOL_PATH}")
  add_dependencies("${TARGET}" windowsSdkBuildTools)
  message(STATUS "Added target ${TARGET} -> ${TOOL_PATH}")
endforeach()
