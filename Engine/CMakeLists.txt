############################################ AetherEngin source files ############################################
set(ENGINE_CODE   
   # components
   #${AETHER_ENGINE_SOURCE_DIR}/components/entity.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/entity.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/component.h
   #
   #${AETHER_ENGINE_SOURCE_DIR}/components/animation_component.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/animation_component.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/animation_impl.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/animation_impl.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/camera_component.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/camera_component.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/light_component.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/light_component.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/mesh_component.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/mesh_component.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/particle_component.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/particle_component.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/scene_component.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/scene_component.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/skeletal_mesh_component.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/skeletal_mesh_component.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/skybox_component.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/skybox_component.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/shape_mesh_component.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/shape_mesh_component.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/components/watermark_component.h
   #${AETHER_ENGINE_SOURCE_DIR}/components/watermark_component.cpp
   #
   ## effect
   #${AETHER_ENGINE_SOURCE_DIR}/effect/blur.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/blur.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/deferred_shading_renderer.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/deferred_shading_renderer.cpp   
   #${AETHER_ENGINE_SOURCE_DIR}/effect/effect.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/effect.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/forward_shading_renderer.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/forward_shading_renderer.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/gi.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/gi.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/hdr_postprocess.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/hdr_postprocess.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/ibl.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/ibl.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/ldr_postprocess.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/ldr_postprocess.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/parameter.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/parameter.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/postprocess.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/postprocess.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/scene_renderer.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/scene_renderer.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/shadow_layer.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/shadow_layer.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/taa.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/taa.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/technique.h
   #${AETHER_ENGINE_SOURCE_DIR}/effect/technique.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/effect/variable.h

   ## kernel
   ${AETHER_ENGINE_SOURCE_DIR}/Kernel/Kernel.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Kernel/KernelModule.ixx
   #${AETHER_ENGINE_SOURCE_DIR}/kernel/context.h
   #${AETHER_ENGINE_SOURCE_DIR}/kernel/context.cpp


   ## math
   ${AETHER_ENGINE_SOURCE_DIR}/Math/AABBox.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/AABBox.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Box.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Color.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Color.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Frustum.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Frustum.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Hash.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/MathModule.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Matrix.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Matrix.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Plane.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Plane.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/QuadMeshProcess.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/QuadMeshProcess.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Quaternion.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Quaternion.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Ray.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Ray.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Rect.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Transform.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Transform.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Utility.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Utility.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Vector.ixx

   # rhi
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/RHIModule.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHICommandList.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIStruct.ixx


    #
    # d3d12 rhi
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Definition.ixx

    ## utils
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Buffer.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Buffer.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Compression.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Compression.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/DllLoader.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/DllLoader.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Error.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Exposure.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Exposure.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/File.ixx   
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Format.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Format.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/ImageDecode.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/ImageDecode.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Log.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Log.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/SafeC.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/SafeC.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Timer.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Timer.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/UtilsModule.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Utils.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/Utils.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/ZBase64.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Utils/ZBase64.cpp

)
source_group(TREE ${AETHER_ENGINE_SOURCE_DIR}/ FILES ${ENGINE_CODE})

############################################ aether_engine target ############################################
set(AETHER_SHAREDLIB_NAME ${PROJECT_NAME} CACHE STRING "" FORCE)
set(AETHER_STATICLIB_NAME ${PROJECT_NAME}-static CACHE STRING "" FORCE)

if(TARGET_PLATFORM_WINDOWS)
    set(AETHER_BUILD_SHAREDLIB TRUE)
    set(AETHER_STATICLIB_DEPENDENT_LIBS dxgi d3d11 d3dcompiler dxguid Windowscodecs Ws2_32 Winmm)
endif()    


############################################ aether_engine static lib ############################################
add_library(${AETHER_STATICLIB_NAME} STATIC ${ENGINE_CODE})
#add_dependencies(${AETHER_STATICLIB_NAME} GenerateShaders)
target_link_libraries(${AETHER_STATICLIB_NAME} PUBLIC
    turbojpeg
    libpng
    rapidjson
    eigen
    zlib
    ${AETHER_STATICLIB_DEPENDENT_LIBS}
)
target_include_directories(${AETHER_STATICLIB_NAME} PUBLIC ${AETHER_ENGINE_SOURCE_DIR})



############################################ aether_engine shared lib ############################################
if(AETHER_BUILD_SHAREDLIB)
    #add_library(${AETHER_SHAREDLIB_NAME} SHARED ${AETHER_ENGINE_SOURCE_DIR}/fake.cpp)
    #target_link_libraries(${AETHER_SHAREDLIB_NAME} PUBLIC ${AETHER_STATICLIB_NAME})
endif()