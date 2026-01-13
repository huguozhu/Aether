module;
#include <fstream>
#include <atomic>
#include "shader/shader_content.h"
#include "aether.config.h"
#include "tools/ShaderCompiler/shader_helper.h"

module Aether:ResourceManager;
import :ResourceManager;
import <string>;

namespace Aether
{
    static inline ShaderContent RHIQueryGeneratedShaderMetaContent(const std::string& shaderName)
    {
        return QueryShaderMetaContent(shaderName);
    }

    static inline ShaderContent RHIQueryGeneratedShaderCodeContent(const std::string& shaderLanguage, const std::string& shaderName, bool isDebug)
    {
        if (isDebug)
        {
#if defined(AETHER_SHADER_GENERATE_DEBUG)
            return RHIQueryShaderCodeContent_debug(shaderLanguage, shaderName);
#else
            LOG_ERROR("debug shader is not generated, please reconfigure with AETHER_SHADER_GENERATE_DEBUG enabled");
            return ShaderContent{ nullptr, 0 };
#endif
        }
        else
        {
            return QueryShaderCodeContent(shaderLanguage, shaderName);
        }
    }

    static inline ShaderContent RHIQueryGeneratedShaderReflectContent(const std::string& shaderLanguage, const std::string& shaderName, bool isDebug)
    {
        if (isDebug)
        {
#if defined(AETHER_SHADER_GENERATE_DEBUG)
            return RHIQueryShaderReflectContent_debug(shaderLanguage, shaderName);
#else
            LOG_ERROR("debug shader is not generated, please reconfigure with AETHER_SHADER_GENERATE_DEBUG enabled");
            return ShaderContent{ nullptr, 0 };
#endif
        }
        else
        {
            return QueryShaderReflectContent(shaderLanguage, shaderName);
        }
    }

    static ResourceID GenerateResourceID()
    {
        static std::atomic<ResourceID> __global_resource_id{ 0 };
        return ++__global_resource_id;
    }

    IResource::IResource(ResourceManager* mgr)
    : resourceMgr(mgr)
    {
        _id = GenerateResourceID();
    }
    std::string ResourceManager::ResourceRootPath()
    {
        static std::string root_path;

        if (root_path.empty())
        {
#if defined(AETHER_PLATFORM_IOS)
            CFURLRef resourceURL = CFBundleCopyResourcesDirectoryURL(CFBundleGetMainBundle());
            char resourcePath[PATH_MAX];
            if (CFURLGetFileSystemRepresentation(resourceURL, true, (UInt8*)resourcePath, PATH_MAX))
            {
                if (resourceURL != NULL)
                {
                    CFRelease(resourceURL);
                }
            }
            root_path = std::string(resourcePath);
#endif
        }
        return root_path;
    }

    std::string ResourceManager::GetShaderMetaPath(const std::string& shaderName)
    {
        return std::string{ AETHER_GENERATED_META_DIR "/" } + shaderName + SHADER_META_FILE_SUFFIX;
    }

    std::string ResourceManager::GetShaderCodePath(const std::string& shaderName)
    {
        ERHIType rhi_type = m_pEngine->GetRHIType();
        std::string debug_dir_suffix = "";
        switch (rhi_type)
        {
         case ERHIType::D3D12:
            return std::string{ AETHER_GENERATED_SHADER_DIR } + "/dxil" + debug_dir_suffix + "/" + shaderName + ".dxil";        
        default:
            LOG_ERROR("unsupported RHIType %d", rhi_type);
            return "";
        }
    }

    std::string ResourceManager::GetShaderReflectPath(const std::string& shaderName)
    {
        ERHIType rhi_type = m_pEngine->GetRHIType();
        std::string debug_dir_suffix = "";
        //if (m_pEngine->IsDebug())
        //    debug_dir_suffix = SHADER_DEBUG_DIR_SUFFIX;
        switch (rhi_type)
        {
        case ERHIType::D3D12:
            return std::string{ AETHER_GENERATED_SHADER_DIR } + "/hlsl" + debug_dir_suffix + "/" + shaderName + SHADER_REFLECT_FILE_SUFFIX;
        default:
            LOG_ERROR("unsupported RHIType %d", rhi_type);
            return "";
        }
    }

    const std::string& ResourceManager::GetShaderLanguageStr()
    {
        static const std::string d3d11_shaderlanguage = "hlsl";
        static const std::string d3d12_shaderlanguage = "dxil";
        static const std::string metal_mac_shaderlanguage = "msl_macos";
        static const std::string metal_ios_shaderlanguage = "msl_ios";
        static const std::string opengles_shaderlanguage = "essl";
        static const std::string empty_str = "";
        ERHIType rhi_type = m_pEngine->GetRHIType();
        switch (rhi_type)
        {
        case ERHIType::D3D12:
            return d3d12_shaderlanguage;
        default:
            return empty_str;
        }
    }

    ResourceManager::ResourceManager(AetherEngine* engine)
        : m_pEngine(engine)
    {
    }

    FileResourcePtr ResourceManager::LoadFileResource(const std::string& filePath)
    {
        auto resIt = m_fileRes.find(filePath);
        if (resIt == m_fileRes.end())
        {
            FileResourcePtr _res = MakeSharedPtr<FileResource>(filePath);
            if (!_res->IsAvailable())
            {
                LOG_ERROR("ResourceManager, load file resource %s fail", filePath.c_str());
                return nullptr;
            }

            resIt = m_fileRes.insert({ filePath, _res }).first;
        }
        return resIt->second;
    }

    MetaShaderResourcePtr ResourceManager::LoadMetaShaderResource(const std::string& shaderName)
    {
        auto resIt = m_metaShaderRes.find(shaderName);
        if (resIt == m_metaShaderRes.end())
        {
            MetaShaderResourcePtr _res = MakeSharedPtr<MetaShaderResource>(this);
            AResult ret = _res->Load(shaderName);
            if (AETHER_CHECKFAILED(ret))
            {
                LOG_ERROR("ResourceManager, load meta shader resource %s fail", shaderName.c_str());
                return nullptr;
            }

            resIt = m_metaShaderRes.insert({ shaderName, _res }).first;
        }

        return resIt->second;
    }

    ShaderResourcePtr ResourceManager::LoadShaderResource(const std::string& shaderName)
    {
        auto resIt = m_shaderRes.find(shaderName);
        if (resIt == m_shaderRes.end())
        {
            ShaderResourcePtr _res = MakeSharedPtr<ShaderResource>(this);
            AResult ret = _res->Load(shaderName);
            if (AETHER_CHECKFAILED(ret))
            {
                LOG_ERROR("ResourceManager, load shader resource %s fail", shaderName.c_str());
                return nullptr;
            }

            resIt = m_shaderRes.insert({ shaderName, _res }).first;
        }

        return resIt->second;
    }

    AResult MetaShaderResource::Load(const std::string& metaShaderName)
    {
        const void* metaData = nullptr;
        size_t metaSize = 0;
        auto content = RHIQueryGeneratedShaderMetaContent(metaShaderName);
        if (!content.first)
        {
            LOG_ERROR("get shader %s meta content fail", metaShaderName.c_str());
            return ERR_FILE_NOT_FOUND;
        }
        metaData = content.first;
        metaSize = content.second;

        _name = metaShaderName;
        shadercompiler::MetaJsonReader reader(reinterpret_cast<const char*>(metaData), metaSize);
        shadercompiler::MetaInfo _metaInfo;
        if (0 == reader.Read(_metaInfo))
        {
            metaInfo = _metaInfo;
            SetAvailable(true);
            return A_Success;
        }
        else
        {
            return ERR_INVALID_DATA;
        }
    }

    AResult ShaderResource::Load(const std::string& shaderName)
    {
        const void* codeData;
        size_t codeSize;
        const void* reflectData;
        size_t reflectSize;
        auto codeContent = RHIQueryGeneratedShaderCodeContent(resourceMgr->GetShaderLanguageStr(), shaderName, false);
        if (!codeContent.first)
        {
            LOG_ERROR("get shader %s code content fail", shaderName.c_str());
            return ERR_FILE_NOT_FOUND;
        }

        auto reflectContent = RHIQueryGeneratedShaderReflectContent(resourceMgr->GetShaderLanguageStr(), shaderName, false);
        if (!reflectContent.first)
        {
            LOG_ERROR("get shader %s code content fail", shaderName.c_str());
            return ERR_FILE_NOT_FOUND;
        }

        codeData = codeContent.first;
        codeSize = codeContent.second;
        reflectData = reflectContent.first;
        reflectSize = reflectContent.second;

        _name = shaderName;
        shadercompiler::ReflectJsonReader reader(reinterpret_cast<const char*>(reflectData), reflectSize);
        if (0 == reader.Read(reflectInfo))
        {
            sourceCode = codeData;
            sourceCodeSize = codeSize;
            SetAvailable(true);
            return A_Success;
        }
        else
        {
            return ERR_INVALID_DATA;
        }
    }


    /******************************************************************************
    * ResourceManager
    ******************************************************************************/

};
