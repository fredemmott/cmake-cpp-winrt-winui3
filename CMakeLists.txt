project("com.fredemmott.CmakeCppWinRTWinUI3")
cmake_minimum_required(VERSION 3.23)
add_compile_options("/fsanitize=address")
add_subdirectory(src)
