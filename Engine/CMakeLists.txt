############################################ AetherEngin source files ############################################
set(ENGINE_CODE
   # Components
   #${AETHER_ENGINE_SOURCE_DIR}/Components/entity.h
   #${AETHER_ENGINE_SOURCE_DIR}/Components/entity.cpp
   #${AETHER_ENGINE_SOURCE_DIR}/Components/component.h

   ## effect
   #${AETHER_ENGINE_SOURCE_DIR}/Effect/blur.h
   #${AETHER_ENGINE_SOURCE_DIR}/Effect/blur.cpp

    # Kernel
    ${AETHER_ENGINE_SOURCE_DIR}/Kernel/Engine.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Kernel/Engine.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/Kernel/EngineDefinition.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/Kernel/KernelModule.ixx
    

   ## Math
   ${AETHER_ENGINE_SOURCE_DIR}/Math/AABBox.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/AABBox.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Box.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Color.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Color.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Frustum.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Frustum.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Math/Hash.ixx   
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

   ${AETHER_ENGINE_SOURCE_DIR}/Math/MathModule.ixx


   # Parallel
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/FrameSyncManager.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/FrameSyncManager.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/Job.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/Parallel/Job.cpp

   # RHI Core
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/RHIModule.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIContext.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIContext.cpp
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHICommandList.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIDevice.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIResource.ixx
   ${AETHER_ENGINE_SOURCE_DIR}/RHI/Core/RHIStruct.ixx
     
    # D3D12 RHI
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Context.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Context.cpp
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Definition.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Device.ixx
    ${AETHER_ENGINE_SOURCE_DIR}/RHI/D3D12/D3D12Device.cpp
    
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