# Hybrid CRT
# https://github.com/microsoft/WindowsAppSDK/blob/main/docs/Coding-Guidelines/HybridCRT.md

# Enable CMAKE_MSVC_RUNTIME_LIBRARY variable
cmake_policy(SET CMP0091 NEW)
set(
	CMAKE_MSVC_RUNTIME_LIBRARY
	# Statically link the C++ runtime libraries, but partially override this below
	"MultiThreaded$<$<CONFIG:Debug>:Debug>"
)
add_link_options(
	"/DEFAULTLIB:ucrt$<$<CONFIG:Debug>:d>.lib" # include the dynamic UCRT
	"/NODEFAULTLIB:libucrt$<$<CONFIG:Debug>:d>.lib" # ignore the static UCRT
)
