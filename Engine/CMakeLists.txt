############################################ AetherEngin source files ############################################
set(ENGINE_CODE
   ${AETHER_ENGINE_SOURCE_DIR}/EngineModule.ixx

   # Components
   ${AETHER_ENGINE_SOURCE_DIR}/Components/AnimationComponent.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Components/AnimationComponent.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Components/AnimationImpl.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Components/AnimationImpl.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Components/CameraComponent.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Components/CameraComponent.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Components/Component.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Components/Entity.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Components/Entity.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Components/LightComponent.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Components/LightComponent.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Components/SceneComponent.ixx

   ## effect
   #${AETHER_ENGINE_SOURCE_DIR}/Effect/blur.ixx
   #${AETHER_ENGINE_SOURCE_DIR}/Effect/blur.cpp
   
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/Effect.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/Effect.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/Parameter.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/Parameter.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/ResourceManager.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/ResourceManager.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/ShaderHelper.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/ShaderHelper.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/Technique.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/Technique.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Effect/Variable.ixx

    # Kernel
    ${AETHER_ENGINE_SOURCE_DIR}/Kernel/Engine.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Kernel/Engine.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Kernel/EngineDefinition.ixx

    # SceneManager
    ${AETHER_ENGINE_SOURCE_DIR}/SceneManager/SceneManager.ixx
    #${AETHER_ENGINE_SOURCE_DIR}/SceneManager/SceneManager.cpp
    

   ## Math
   ${AETHER_ENGINE_SOURCE_DIR}/Math/AABBox.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/AABBox.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Box.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Color.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Color.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Frustum.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Frustum.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Hash.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/MathUtils.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/MathUtils.cpp
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
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Vector.ixx


   # Parallel
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/FrameSyncManager.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/FrameSyncManager.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/Job.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/Job.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/Mutex.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/Mutex.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/Semaphore.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/Semaphore.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/Thread.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/Thread.cpp

   # RHI Core
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIContext.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIContext.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHICommandList.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIDevice.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIFence.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIRenderState.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIRenderState.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIResource.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIShader.ixx
   #${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIShader.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIStruct.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHITexture.ixx
     
    # D3D12 RHI
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12CommandList.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12CommandList.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Context.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Context.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Definition.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Device.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Device.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Fence.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Fence.cpp
    #${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Shader.ixx
    #${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Shader.cpp
    #${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Texture.ixx
    #${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Texture.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Window.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Window.cpp

    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3DAdapter.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3DAdapter.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/DxgiHelper.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/DxgiHelper.cpp
    

    # Vulkan RHI
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Definition.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Definition.ixx

    ## Utils
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
    set(AETHER_STATICLIB_DEPENDENT_LIBS dxgi d3d12 d3dcompiler dxguid Windowscodecs Ws2_32 Winmm)
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