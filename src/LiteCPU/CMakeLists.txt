
add_library(
    LiteCPU          	   	     
    Computer.cpp
    Computer.h        
)

target_link_libraries(LiteCPU
	fmt        
)

target_compile_features(LiteCPU PUBLIC cxx_std_20)    

target_include_directories(LiteCPU
    PRIVATE ${LiteCPU_SOURCE_DIR}/contrib/spdlog/include
)


add_compile_definitions(_CRT_SECURE_NO_WARNINGS)       

target_compile_definitions(LiteCPU
    PRIVATE
        # If the debug configuration pass the DEBUG define to the compiler 
        DEBUG
)

target_compile_features(LiteCPU PUBLIC cxx_std_20)



