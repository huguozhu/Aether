

################################################ IMGUI ################################################
set(IMGUI_FILES
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imconfig.h
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imgui_demo.cpp
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imgui_draw.cpp
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imgui_internal.h
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imgui_tables.cpp
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imgui_widgets.cpp
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imgui.cpp
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imgui.h
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imstb_rectpack.h
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imstb_textedit.h
    ${AETHER_THIRDPARTY_DIR}/imgui/include/imstb_truetype.h
)
if(TARGET_PLATFORM_WINDOWS)
    list(APPEND IMGUI_FILES ${AETHER_THIRDPARTY_DIR}/imgui/include/backends/imgui_impl_win32.cpp)
    list(APPEND IMGUI_FILES ${AETHER_THIRDPARTY_DIR}/imgui/include/backends/imgui_impl_dx11.cpp)
elseif(TARGET_PLATFORM_MACOS)
    list(APPEND IMGUI_FILES ${AETHER_THIRDPARTY_DIR}/imgui/include/backends/imgui_impl_metal.mm
                            ${AETHER_THIRDPARTY_DIR}/imgui/include/backends/imgui_impl_osx.mm)
elseif(TARGET_PLATFORM_IOS)
    list(APPEND IMGUI_FILES ${AETHER_THIRDPARTY_DIR}/imgui/include/backends/imgui_impl_metal.mm)
endif()

add_library(IMGUI STATIC ${IMGUI_FILES})
target_include_directories(IMGUI PUBLIC
    ${AETHER_THIRDPARTY_DIR}/imgui/include
    ${AETHER_THIRDPARTY_DIR}/imgui/include/backends
    ${AETHER_THIRDPARTY_DIR}/glfw/include
)

set_target_properties(IMGUI PROPERTIES
    FOLDER "ThirdParty/imgui"
    XCODE_ATTRIBUTE_MACOSX_DEPLOYMENT_TARGET "10.13"
)



################################################ libpng ################################################
add_library(libpng INTERFACE)
target_include_directories(libpng INTERFACE ${AETHER_THIRDPARTY_DIR}/libpng/include)
target_link_libraries(libpng INTERFACE ${AETHER_THIRDPARTY_DIR}/libpng/libs/${TARGET_PLATFORM}/libpng16_static.lib)

################################################ turbojpeg ################################################
add_library(turbojpeg INTERFACE)
target_include_directories(turbojpeg INTERFACE ${AETHER_THIRDPARTY_DIR}/libjpeg-turbo/include)
target_link_libraries(libpng INTERFACE ${AETHER_THIRDPARTY_DIR}/libjpeg-turbo/libs/${TARGET_PLATFORM}/turbojpeg-static.lib)

################################################ rapidjson ################################################
add_library(rapidjson INTERFACE)
target_include_directories(rapidjson INTERFACE ${AETHER_THIRDPARTY_DIR}/rapidjson/include)

################################################ eigen ################################################
add_library(eigen INTERFACE)
target_include_directories(eigen INTERFACE ${AETHER_THIRDPARTY_DIR}/eigen/include)

################################################ zlib ################################################
add_library(zlib INTERFACE)
target_include_directories(zlib INTERFACE ${AETHER_THIRDPARTY_DIR}/zlib/include)
target_link_libraries(zlib INTERFACE ${AETHER_THIRDPARTY_DIR}/zlib/libs/${TARGET_PLATFORM}/Zlib_19.lib)