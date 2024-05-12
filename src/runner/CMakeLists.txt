
add_executable(Runner main.cpp )

target_compile_features(Runner PUBLIC cxx_std_20)    

target_include_directories(Runner 
    PRIVATE ${litecpu_SOURCE_DIR}/contrib/imgui
    PRIVATE ${litecpu_SOURCE_DIR}/contrib/SDL/include
)

if(WIN32)
    target_link_libraries(Runner   
        PRIVATE SDL3-shared    
	)

	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
    
    set_property(TARGET Runner PROPERTY VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")    
      
else(WIN32)

	target_link_libraries(Runner         
		PRIVATE SDL3-shared     
	)

endif(WIN32)

