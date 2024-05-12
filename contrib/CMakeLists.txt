
add_subdirectory(fmt)
add_subdirectory(googletest)
add_subdirectory(SDL)

add_compile_definitions(IMGUI_DEFINE_MATH_OPERATORS)

add_library(IMGUI 
    imgui/imconfig.h
    imgui/imgui.cpp
    imgui/imgui.h
    imgui/imgui_demo.cpp
    imgui/imgui_draw.cpp
    imgui/imgui_internal.h
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imstb_rectpack.h
    imgui/imstb_textedit.h
    imgui/imstb_truetype.h

    imgui/backends/imgui_impl_sdl3.h
    imgui/backends/imgui_impl_sdl3.cpp
    imgui/backends/imgui_impl_sdlrenderer3.cpp
    imgui/backends/imgui_impl_sdlrenderer3.h   
)

target_include_directories(IMGUI 	
    PRIVATE ${LiteCPU_SOURCE_DIR}/contrib/imgui/	
    PRIVATE ${LiteCPU_SOURCE_DIR}/contrib/SDL/include
)

target_link_libraries(IMGUI)

target_compile_features(IMGUI PUBLIC cxx_std_20)