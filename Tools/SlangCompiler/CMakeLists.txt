
cmake_minimum_required(VERSION 3.15) 
project(SlangCompiler)
cmake_policy(SET CMP0054 NEW)

if(NOT DEFINED AETHER_ROOT_DIR)
    message(FATAL_ERROR "must specify AETHER_ROOT_DIR")
endif()
if(NOT DEFINED RAPIDJSON_INCLUDE_DIR)
    message(FATAL_ERROR "must specify RAPIDJSON_INCLUDE_DIR")
endif()

set(SHADER_COMPILER_SRCS
    ../../Engine/Math/Hash.ixx
    ../../Engine/Effect/ShaderHelper.ixx
    ../../Engine/Effect/ShaderHelper.cpp
    ShaderHelperModule.ixx
    SlangCompiler.cpp
    SlangProcess.h
    SlangProcess.cpp
    SlangHelper.h
    SlangHelper.cpp
    spirv_reflect.c
    spirv_reflect.h
    ../../ThirdParty/slang/include/slang.h
)
source_group("Source" FILES ${SHADER_COMPILER_SRCS})
add_executable(SlangCompiler ${SHADER_COMPILER_SRCS})

target_compile_definitions(SlangCompiler PRIVATE NOMINMAX)

target_include_directories(SlangCompiler PRIVATE
    ${AETHER_ROOT_DIR}
    ${AETHER_THIRDPARTY_DIR}/CLI11/include
    ${AETHER_THIRDPARTY_DIR}/slang/include
    ${CMAKE_CURRENT_BINARY_DIR}/../../generated
    ${RAPIDJSON_INCLUDE_DIR}
)

target_link_libraries(SlangCompiler 
    PRIVATE
        rapidjson
        ${AETHER_THIRDPARTY_DIR}/slang/libs/${TARGET_PLATFORM}/slang.lib
        ${AETHER_THIRDPARTY_DIR}/slang/libs/${TARGET_PLATFORM}/slang-compiler.lib
)

if(WIN32)
    target_link_libraries(SlangCompiler PRIVATE d3dcompiler)
endif()

set_target_properties(SlangCompiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}
)

if(WIN32)
    set(slang_LIB_DIRS ${AETHER_THIRDPARTY_DIR}/slang/libs/${TARGET_PLATFORM})
    add_custom_command(TARGET SlangCompiler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${slang_LIB_DIRS}/slang.dll      $<TARGET_FILE_DIR:SlangCompiler>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${slang_LIB_DIRS}/slang-compiler.dll $<TARGET_FILE_DIR:SlangCompiler>
    )
endif()
