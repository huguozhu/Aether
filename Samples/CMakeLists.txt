##################################  sample common  ##########################################
set(SAMPLE_COMMON_PROJECT_NAME SampleCommon)

set(SAMPLE_COMMON_SRCS
	${AETHER_SAMPLES_DIR}/Common/Fake.cpp
    ${AETHER_SAMPLES_DIR}/Common/AppFramework.ixx
    ${AETHER_SAMPLES_DIR}/Common/AppFramework.cpp
    ${AETHER_SAMPLES_DIR}/Common/AppImgui.ixx
    ${AETHER_SAMPLES_DIR}/Common/AppImgui.cpp

    ${AETHER_SAMPLES_DIR}/Common/CommonModule.ixx
)

set(SAMPLE_DEPENDENT_LIBS)

add_library(${SAMPLE_COMMON_PROJECT_NAME} STATIC ${SAMPLE_COMMON_SRCS})

#add_dependencies(${SAMPLE_COMMON_PROJECT_NAME} ${AETHER_SHAREDLIB_NAME})
set_target_properties(${SAMPLE_COMMON_PROJECT_NAME} PROPERTIES FOLDER "Samples")

# link
target_link_libraries(${SAMPLE_COMMON_PROJECT_NAME} PUBLIC ${SAMPLE_DEPENDENT_LIBS} ${AETHER_STATICLIB_NAME})
target_link_libraries(${SAMPLE_COMMON_PROJECT_NAME}
    PUBLIC
        IMGUI
        rapidjson
        eigen
        vulkan
        ${SAMPLE_DEPENDENT_LIBS}
)

# include
target_include_directories(${SAMPLE_COMMON_PROJECT_NAME} PUBLIC
    ${AETHER_THIRDPARTY_DIR}/imgui/include
    ${AETHER_ENGINE_SOURCE_DIR}
    ${AETHER_SAMPLES_DIR}
)

##################################  sample  ##########################################
function(add_sample_project sample_name)
    # source code
    set(SAMPLE_CODE
        ${AETHER_SAMPLES_DIR}/${sample_name}/${sample_name}.cpp
    )

    # resource file/dir
    set(SAMPLE_RESOURCE_FILES "")
    set(EMBED_FRAMEWORKS "")

    source_group(Resources FILES ${SAMPLE_RESOURCE_FILES})
    source_group(TREE ${AETHER_SAMPLES_DIR}/${sample_name}/ FILES ${SAMPLE_CODE})
    # execute
    add_executable(${sample_name} ${SAMPLE_CODE} ${SAMPLE_RESOURCE_FILES})
    target_compile_definitions(${sample_name} PRIVATE _CRT_SECURE_NO_WARNINGS)
    target_link_libraries(${sample_name} PUBLIC ${SAMPLE_COMMON_PROJECT_NAME})
	
    # property
    set_target_properties(${sample_name} PROPERTIES FOLDER "Samples")

    if (WIN32)
        set_target_properties(${sample_name}    PROPERTIES LINK_FLAGS "/ignore:4099")
    endif()

    target_include_directories(${sample_name} PUBLIC 
        ${AETHER_ENGINE_SOURCE_DIR}
        ${AETHER_SAMPLES_DIR}/Common
    )
endfunction(add_sample_project)

add_sample_project(01.Tutorial)


