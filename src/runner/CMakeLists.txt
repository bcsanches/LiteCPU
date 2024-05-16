
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
    PRIVATE ${LiteCPU_SOURCE_DIR}/contrib/SDL/include
    PRIVATE ${LiteCPU_SOURCE_DIR}/contrib/spdlog/include
)

if(WIN32)
    target_link_libraries(Runner   
        PRIVATE SDL3::SDL3   
        PRIVATE IMGUI
	)

	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    
    set_property(TARGET Runner PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")    
      
else(WIN32)

	target_link_libraries(Runner         
		PRIVATE SDL3::SDL3     
        PRIVATE IMGUI
	)

endif(WIN32)

