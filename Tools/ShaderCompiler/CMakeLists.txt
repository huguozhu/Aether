
cmake_minimum_required(VERSION 3.15) 
project(ShaderCompiler)
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
    ShaderCompiler.cpp
    spirv_reflect.c
    spirv_reflect.h
    ../../ThirdParty/shaderconductor/include/shaderconductor/ShaderConductor.hpp
)
source_group("Source" FILES ${SHADER_COMPILER_SRCS})
add_executable(ShaderCompiler ${SHADER_COMPILER_SRCS})

target_compile_definitions(ShaderCompiler PRIVATE NOMINMAX)

target_include_directories(ShaderCompiler PRIVATE
    ${AETHER_ROOT_DIR}
    ${AETHER_THIRDPARTY_DIR}/CLI11/include
    ${AETHER_THIRDPARTY_DIR}/shaderconductor/include
    ${CMAKE_CURRENT_BINARY_DIR}/../../generated
    ${RAPIDJSON_INCLUDE_DIR}
)

target_link_libraries(ShaderCompiler 
    PRIVATE
        rapidjson
        ${AETHER_THIRDPARTY_DIR}/shaderconductor/libs/${TARGET_PLATFORM}/ShaderConductor.lib
)

if(WIN32)
    target_link_libraries(ShaderCompiler PRIVATE d3dcompiler)
endif()

set_target_properties(ShaderCompiler PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY_DEBUG   ${CMAKE_CURRENT_BINARY_DIR}
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_CURRENT_BINARY_DIR}
)

if(APPLE)
    set_target_properties(ShaderCompiler PROPERTIES
        XCODE_ATTRIBUTE_ARCHS "$(ARCHS_STANDARD)"
        XCODE_ATTRIBUTE_VALID_ARCHS "x86_64"
        XCODE_ATTRIBUTE_LD_RUNPATH_SEARCH_PATHS "@executable_path ${shaderconductor_LIB_DIRS}"
    )
endif()

set(shaderconductor_LIB_DIRS ${AETHER_THIRDPARTY_DIR}/shaderconductor/libs/${TARGET_PLATFORM})
if(WIN32)
    add_custom_command(TARGET ShaderCompiler POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${shaderconductor_LIB_DIRS}/dxcompiler.dll      $<TARGET_FILE_DIR:ShaderCompiler>
        COMMAND ${CMAKE_COMMAND} -E copy_if_different ${shaderconductor_LIB_DIRS}/ShaderConductor.dll $<TARGET_FILE_DIR:ShaderCompiler>
    )
endif()
