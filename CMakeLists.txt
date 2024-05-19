cmake_minimum_required (VERSION 3.14)

#main project
project (LiteCPU)

set(VERSION_MAJOR "0" )
set(VERSION_MINOR "0" )
set(VERSION_PATCH "1" )
set(LITECPU_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}")

add_compile_definitions(LITECPU_VERSION=\"${LITECPU_VERSION}\")
add_compile_definitions(SPDLOG_FMT_EXTERNAL)

set( CMAKE_EXPORT_COMPILE_COMMANDS on)
set( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin )
set( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib )

file(MAKE_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

set_property( GLOBAL PROPERTY USE_FOLDERS ON)

add_subdirectory(contrib)
add_subdirectory(src)

set_property(TARGET fmt PROPERTY FOLDER "contrib")
set_property(TARGET gtest PROPERTY FOLDER "contrib")
set_property(TARGET gtest_main PROPERTY FOLDER "contrib")
set_property(TARGET gmock PROPERTY FOLDER "contrib")
set_property(TARGET gmock_main PROPERTY FOLDER "contrib")

set(BUILD_GMOCK ON CACHE BOOL "" FORCE)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

set_property(TARGET IMGUI PROPERTY FOLDER "contrib")
set_property(TARGET SDL3-shared PROPERTY FOLDER "contrib/SDL")
set_property(TARGET uninstall PROPERTY FOLDER "contrib/SDL")
    
set(SDL_TEST_LIBRARY OFF CACHE BOOL "" FORCE)


INSTALL(FILES contrib/imgui/misc/fonts/DroidSans.ttf DESTINATION bin/ )

include(InstallRequiredSystemLibraries)
