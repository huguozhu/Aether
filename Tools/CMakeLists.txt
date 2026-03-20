# ShaderCompiler & SlangCompiler
set(RAPIDJSON_INCLUDE_DIR "${AETHER_THIRDPARTY_DIR}/rapidjson/include")
set(SLANG_INCLUDE_DIR "${AETHER_THIRDPARTY_DIR}/slang/include")

if(TARGET_PLATFORM_WINDOWS OR TARGET_PLATFORM_MACOS OR TARGET_PLATFORM_LINUX)
    add_subdirectory(ShaderCompiler)
	add_subdirectory(SlangCompiler)
endif()

if(WIN32)
    set(ShaderCompiler_bin ${CMAKE_CURRENT_BINARY_DIR}/ShaderCompiler/ShaderCompiler.exe CACHE STRING "" FORCE)
	set(SlangCompiler_bin ${CMAKE_CURRENT_BINARY_DIR}/SlangCompiler/SlangCompiler.exe CACHE STRING "" FORCE)
else()
    set(ShaderCompiler_bin ${CMAKE_CURRENT_BINARY_DIR}/ShaderCompiler/ShaderCompiler CACHE STRING "" FORCE)
	set(SlangCompiler_bin ${CMAKE_CURRENT_BINARY_DIR}/SlangCompiler/SlangCompiler CACHE STRING "" FORCE)
endif()

set_target_properties(ShaderCompiler PROPERTIES FOLDER "Tools")
set_target_properties(SlangCompiler PROPERTIES FOLDER "Tools")