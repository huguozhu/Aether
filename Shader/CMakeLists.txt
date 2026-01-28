if(CMAKE_VERSION VERSION_GREATER_EQUAL "3.30")
    cmake_policy(SET CMP0147 NEW) # Visual Studio Generators build custom commands in parallel.
endif()

set(shader_shared_files
    #${AETHER_SHADER_SOURCE_DIR}/shared/ColorReplace.h
    ${AETHER_SHADER_SOURCE_DIR}/shared/Common.h
    #${AETHER_SHADER_SOURCE_DIR}/shared/GI.h
    #${AETHER_SHADER_SOURCE_DIR}/shared/ParticleCommon.h
    #${AETHER_SHADER_SOURCE_DIR}/shared/SkyBox.h
    #${AETHER_SHADER_SOURCE_DIR}/shared/Ssao.h
    #${AETHER_SHADER_SOURCE_DIR}/shared/WaterMark.h
)

set(shader_header_files
    ${AETHER_SHADER_SOURCE_DIR}/BRDF.ash
    #${AETHER_SHADER_SOURCE_DIR}/CalcSDF.ash
    ${AETHER_SHADER_SOURCE_DIR}/Common.ash
    #${AETHER_SHADER_SOURCE_DIR}/IBL.ash
    ${AETHER_SHADER_SOURCE_DIR}/Lighting.ash
    #${AETHER_SHADER_SOURCE_DIR}/MorphTarget.ash
    #${AETHER_SHADER_SOURCE_DIR}/Skeleton.ash
    ${AETHER_SHADER_SOURCE_DIR}/ToneMapping.ash
    #${AETHER_SHADER_SOURCE_DIR}/CommonCBuffer.ash
    #${AETHER_SHADER_SOURCE_DIR}/VertexCommon.ash
    #${AETHER_SHADER_SOURCE_DIR}/ParticleCommon.ash
    #${AETHER_SHADER_SOURCE_DIR}/Shadow.ash
)

set(shader_sources_files
	#TestVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/BlurPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/CopyPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/CubeMapVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/DeferredLightingVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/DeferredLightingPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/EmptyPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/EquirectangularToCubeMapPS.asf
    ${AETHER_SHADER_SOURCE_DIR}/ForwardRenderingCommonPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/FxaaPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/GenBRDF2DLutPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/GenerateCascadedShadowMapPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/GenerateCubeShadowMapPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/GenerateGBufferPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/GenerateRsmPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/GiRsmPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/GiLpvPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/GiVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/IrradianceConvolutionPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/LightCullingCS.asf
    #${AETHER_SHADER_SOURCE_DIR}/LiquidGlassPS.asf    
    #${AETHER_SHADER_SOURCE_DIR}/LPVInjectPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/LPVInjectVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/LPVPropagationPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/LPVPropagationVS.asf
    ${AETHER_SHADER_SOURCE_DIR}/MeshRenderingVS.asf
	#
    ${AETHER_SHADER_SOURCE_DIR}/PostProcessVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/PrefilterEnvPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/PreZMeshRenderingVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/Sprite2DMetaballPS.asf    
	#
    #${AETHER_SHADER_SOURCE_DIR}/ShadowCopyPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ShadowingDirectionalPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ShadowingPointPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ShadowingSpotPS.asf
    #
    #${AETHER_SHADER_SOURCE_DIR}/SkyBoxVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/SkyBoxPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/Sprite2DVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/SsaoVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/SsaoPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/TaaPS.asf
    ${AETHER_SHADER_SOURCE_DIR}/ToneMappingPS.asf
	#
    #${AETHER_SHADER_SOURCE_DIR}/WaterMarkPS.asf
    #${AETHER_SHADER_SOURCE_DIR}/WaterMarkVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/WaterMarkGenerateCS.asf
    ## Particle shaders
    #${AETHER_SHADER_SOURCE_DIR}/ParticleInitCS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ParticleTickBeginCS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ParticleEmitCS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ParticleSimulateCS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ParticleCullingCS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ParticlePreSortCS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ParticleBitonicSortCS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ParticleSortMatrixTransposeCS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ParticleRenderVS.asf
    #${AETHER_SHADER_SOURCE_DIR}/ParticleRenderPS.asf
)

################## shader_target_platform ##################
if(TARGET_PLATFORM_WINDOWS)
    set(shader_target_platform "dxil")
elseif(TARGET_PLATFORM_MACOS)
    set(shader_target_platform "msl_macos")
endif()

message(STATUS "generated target platform: ${shader_target_platform}")

############################## path ##############################
set(ShaderCompiler_working_dir "${AETHER_SHADER_SOURCE_DIR}")
if(NOT DEFINED ShaderCompiler_bin)
    message(FATAL_ERROR "ShaderCompiler_bin not defined")
endif()

set(generated_shader_depend_dir         ${AETHER_GENERATED_DEPEND_DIR})
set(generated_shader_tag_depend_dir     ${AETHER_GENERATED_TAG_DIR}/${SHADER_DEPEND_FILE_SUFFIX})
set(generated_shader_tag_compile_dir    ${AETHER_GENERATED_TAG_DIR}/${SHADER_COMPILE_FILE_SUFFIX})

############################## depend ##############################
set(all_generated_tag_depend_tagfiles)
set(all_generated_depend_dependfiles)
foreach(single_shader_sources_file ${shader_sources_files})
    get_filename_component(single_shader_filename ${single_shader_sources_file} NAME)
    get_filename_component(single_shader_basename ${single_shader_sources_file} NAME_WLE)

    set(single_tag_depend_tagfile "${generated_shader_tag_depend_dir}/${single_shader_basename}${SHADER_TAG_FILE_SUFFIX}")
    set(single_depend_dependfile  "${generated_shader_depend_dir}/${single_shader_basename}${SHADER_DEPEND_FILE_SUFFIX}")
    add_custom_command(
        OUTPUT ${single_tag_depend_tagfile} ${single_depend_dependfile}
        COMMAND ${ShaderCompiler_bin} --input "${single_shader_filename}" --depend
        DEPENDS ${single_shader_sources_file} ${shader_header_files} ${shader_shared_files} # also depend on ${ShaderCompiler_bin}, but ShaderCompiler always been build which make incremental build no meaning
        WORKING_DIRECTORY ${ShaderCompiler_working_dir}
    )
    list(APPEND all_generated_tag_depend_tagfiles ${single_tag_depend_tagfile})
    list(APPEND all_generated_depend_dependfiles ${single_depend_dependfile})
endforeach()
############################## compile ##############################
set(shader_compile_options)

if (shader_target_platform STREQUAL "hlsl")
    list(APPEND shader_compile_options "--define=\"AETHER_HLSL=1\"")
elseif (shader_target_platform STREQUAL "dxil")
    list(APPEND shader_compile_options "--define=\"AETHER_DXIL=1\"")  
elseif (shader_target_platform STREQUAL "spirv")
    list(APPEND shader_compile_options "--define=\"AETHER_SPIRV=1\"")
endif()

if (${AETHER_SHADER_GENERATE_DEBUG})
    list(APPEND shader_compile_options "--debug")
endif()

if (AETHER_SHADER_GENERATE_BYTE_CODE)
    list(APPEND shader_compile_options "--bytecode")
endif()

set(all_generated_tag_compile_tagfiles)
foreach(single_shader_sources_file ${shader_sources_files})
    get_filename_component(single_shader_filename ${single_shader_sources_file} NAME)
    get_filename_component(single_shader_basename ${single_shader_sources_file} NAME_WLE)

    set(single_depend_dependfile  "${generated_shader_depend_dir}/${single_shader_basename}${SHADER_DEPEND_FILE_SUFFIX}")
    set(single_tag_compile_tagfile "${generated_shader_tag_compile_dir}/${single_shader_basename}${SHADER_TAG_FILE_SUFFIX}")

    add_custom_command(
        OUTPUT ${single_tag_compile_tagfile}
        COMMAND ${ShaderCompiler_bin} --input=${single_shader_filename} --target=${shader_target_platform} ${shader_compile_options}
        DEPENDS ${single_shader_sources_file} ${single_depend_dependfile} # also depend on ${ShaderCompiler_bin}, but ShaderCompiler always been build which make incremental build no meaning
        WORKING_DIRECTORY ${ShaderCompiler_working_dir})

    list(APPEND all_generated_tag_compile_tagfiles ${single_tag_compile_tagfile})
endforeach()

message(STATUS ${CMAKE_CURRENT_LIST_DIR})
include(${CMAKE_CURRENT_LIST_DIR}/CMakeListst.gnerate_header.cmake)

# interface for engine
add_custom_target(GenerateShaders
    DEPENDS ${all_generated_tag_compile_tagfiles}
    SOURCES ${shader_shared_files} ${shader_header_files} ${shader_sources_files}
)
add_dependencies(GenerateShaders ShaderCompiler)
source_group("Shared" FILES ${shader_shared_files})
source_group("Header" FILES ${shader_header_files})
source_group("Source" FILES ${shader_sources_files})
