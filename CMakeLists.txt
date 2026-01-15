cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_SCAN_FOR_MODULES ON)
cmake_policy(SET CMP0054 NEW)
 
project(Aether VERSION 0.0.1 DESCRIPTION "Aether Rendering Engine" HOMEPAGE_URL "https:://github.com/huguozhu/Aether")

set(SHADER_DEPEND_FILE_SUFFIX ".depend")
set(SHADER_REFLECT_FILE_SUFFIX ".reflect")
set(SHADER_META_FILE_SUFFIX ".meta")
set(SHADER_TAG_FILE_SUFFIX ".tag")
set(SHADER_COMPILE_FILE_SUFFIX ".compile")
set(SHADER_DEBUG_DIR_SUFFIX "_debug")

########################### target platform & arch ######################
if(WIN32)
    set(TARGET_PLATFORM_WINDOWS 1)
    set(TARGET_PLATFORM "windows")
elseif(APPLE)
    set(TARGET_PLATFORM_APPLE 1)
    if(NOT IOS)
        set(TARGET_PLATFORM_MACOS 1)
        set(TARGET_PLATFORM "macos")
    else()
        set(TARGET_PLATFORM_IOS 1)
        set(TARGET_PLATFORM "ios")
    endif()
endif()


if(TARGET_PLATFORM_WINDOWS)
    add_definitions(-DAETHER_PLATFORM_WINDOWS)
    add_definitions(-DNOMINMAX) # disable macro min/max in Windows.h
elseif(TARGET_PLATFORM_MACOS)
    add_definitions(-DAETHER_PLATFORM_MAC -DAETHER_PLATFORM_MACOS)
elseif(TARGET_PLATFORM_IOS)
    add_definitions(-DAETHER_PLATFORM_IOS)
elseif(TARGET_PLATFORM_WEB)
    add_definitions(-DAETHER_PLATFORM_WEB)
endif()


# code path
set(AETHER_ROOT_DIR                "${CMAKE_CURRENT_LIST_DIR}")
set(AETHER_SHADER_SOURCE_DIR       "${AETHER_ROOT_DIR}/Shader")
set(AETHER_ENGINE_SOURCE_DIR       "${AETHER_ROOT_DIR}/Engine")
set(AETHER_SAMPLES_DIR             "${AETHER_ROOT_DIR}/Samples")
set(AETHER_TOOLS_DIR               "${AETHER_ROOT_DIR}/Tools")
set(AETHER_THIRDPARTY_DIR          "${AETHER_ROOT_DIR}/ThirdParty")
set(AETHER_DEPENDENCIES_DIR        "${AETHER_ROOT_DIR}/Dependencies")

set(AETHER_GENERATED_FILE_DIR      "${CMAKE_CURRENT_BINARY_DIR}/Generated")
set(AETHER_GENERATED_SHADER_DIR    "${AETHER_GENERATED_FILE_DIR}/Shader")
set(AETHER_GENERATED_DEPEND_DIR    "${AETHER_GENERATED_SHADER_DIR}/${SHADER_DEPEND_FILE_SUFFIX}")
set(AETHER_GENERATED_META_DIR      "${AETHER_GENERATED_SHADER_DIR}/${SHADER_META_FILE_SUFFIX}")
set(AETHER_GENERATED_TAG_DIR       "${AETHER_GENERATED_SHADER_DIR}/${SHADER_TAG_FILE_SUFFIX}")


configure_file(${AETHER_ROOT_DIR}/aether.config.h.in ${AETHER_GENERATED_FILE_DIR}/aether.config.h @ONLY)

######################### global include path #########################
include_directories(${AETHER_ROOT_DIR})
include_directories(${AETHER_ROOT_DIR}/common/include)
include_directories(${AETHER_GENERATED_FILE_DIR})

add_definitions(-DAETHER_CPP)


######################### function definitions #########################
function(create_directory folder_path)
    if(NOT EXISTS ${folder_path})
        file(MAKE_DIRECTORY ${folder_path})
        message(STATUS "MAKE_DIRECTORY ${folder_path}")
    endif()
endfunction()



######################### add_subdirectory #########################
add_subdirectory(ThirdParty)
add_subdirectory(Shader)
add_subdirectory(Engine)
add_subdirectory(Tools)
add_subdirectory(Samples)