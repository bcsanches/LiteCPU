
add_executable(Runner 
	CPUWidget.cpp
	CPUWidget.h
	main.cpp 
	MainApp.cpp
	MainApp.h
)

target_compile_features(Runner PUBLIC cxx_std_20)    

target_include_directories(Runner 
    PRIVATE ${LiteCPU_SOURCE_DIR}/contrib/imgui
    PRIVATE ${LiteCPU_SOURCE_DIR}/contrib/fmt/include
    PRIVATE ${LiteCPU_SOURCE_DIR}/contrib/SDL/include
    PRIVATE ${LiteCPU_SOURCE_DIR}/contrib/spdlog/include
    PRIVATE ${LiteCPU_SOURCE_DIR}/src/LiteCPU/
)

target_link_libraries(Runner         
	PRIVATE fmt
        PRIVATE IMGUI
	PRIVATE LiteCPU
        PRIVATE SDL3::SDL3  
)

if(WIN32)

	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    
	set_property(TARGET Runner PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")    
      
else(WIN32)



endif(WIN32)

