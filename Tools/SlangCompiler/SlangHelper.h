#include "slang.h"
#include "slang-com-helper.h"
#include "slang-com-ptr.h"
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include <set>

#if defined(_WIN32)
#include <windows.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <d3d12shader.h>
#include "dxcapi.h"
#endif

#include "spirv_reflect.h"

#define SUCCESS     0
#define FAIL        (-__LINE__)

import Aether;
using namespace Aether;

ID3D12ShaderReflection* GetReflectionFromDXIL(const void* dxilData, size_t dxilSize);
void ParseDxilReflectInfo(size_t size, const void* p_code, const std::string& stageName, ReflectInfo& reflectInfo);
void ParseSpirvReflectInfo(size_t size, const void* p_code, const std::string& stageName, SlangCompileTarget sl, ReflectInfo& reflectInfo);

static inline SlangStage ParseShaderStage(const std::string& stageName)
{
    if (stageName == "vs")      return SLANG_STAGE_VERTEX;
    else if (stageName == "ps") return SLANG_STAGE_FRAGMENT;
    else if (stageName == "gs") return SLANG_STAGE_GEOMETRY;
    else if (stageName == "hs") return SLANG_STAGE_HULL;
    else if (stageName == "ds") return SLANG_STAGE_DOMAIN;
    else if (stageName == "cs") return SLANG_STAGE_COMPUTE;
    else if (stageName == "ms") return SLANG_STAGE_MESH;
    else if (stageName == "as") return SLANG_STAGE_AMPLIFICATION;

    else if (stageName == "rg") return SLANG_STAGE_RAY_GENERATION;
    else if (stageName == "rm") return SLANG_STAGE_MISS;
    else if (stageName == "rhg")return SLANG_STAGE_ANY_HIT;
    else if (stageName == "rc") return SLANG_STAGE_CALLABLE;
    else                        return SLANG_STAGE_NONE;
}

static inline SlangCompileTarget ParseShadingLanguage(const std::string& targetName)
{
    if (targetName == "dxil")           return SLANG_DXIL;
    else if (targetName == "spirv")     return SLANG_SPIRV;
    else if (targetName == "hlsl")      return SLANG_HLSL;
    else if (targetName == "glsl")      return SLANG_GLSL;
    else if (targetName == "essl")      return SLANG_GLSL;
    else if (targetName == "msl_macos") return SLANG_METAL;
    else if (targetName == "msl_ios")   return SLANG_METAL;
    else                                return SLANG_TARGET_UNKNOWN;
}

static std::map<SlangCompileTarget, std::string> shaderLanguageMap{
    {SLANG_DXIL, "dxil"},
    {SLANG_SPIRV, "spriv"},
    {SLANG_HLSL, "hlsl"},
    {SLANG_GLSL, "glsl"},
    {SLANG_METAL, "msl_macos"},
};

static std::map<SlangCompileTarget, std::string> shaderLanguageExtMap{
    {SLANG_DXIL, ".dxil"},
    {SLANG_SPIRV, ".spirv"},
    {SLANG_HLSL, ".hlsl"},
    {SLANG_GLSL, ".glsl"},
    {SLANG_METAL, ".msl"},
};

static std::map<SlangCompileTarget, const char*> shaderLanguageVersionMap{
    {SLANG_DXIL, "660"},
    {SLANG_SPIRV,"14"},
    {SLANG_HLSL, "60"},
    {SLANG_GLSL, "430"},
    {SLANG_METAL, nullptr},
};


static int read_file_content(const std::string& filename, std::string& content)
{
    std::ifstream readFile(filename, std::ios_base::binary);
    if (!readFile)
        return FAIL;

    readFile.seekg(0, std::ios::end);
    content.resize(static_cast<size_t>(readFile.tellg()));
    readFile.seekg(0, std::ios::beg);
    readFile.read((char*)content.data(), content.size());
    return SUCCESS;
}

static inline void ltrim(std::string& s)
{
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
        return ch != ' ';
        }));
}

static inline void rtrim(std::string& s)
{
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) {
        return ch != ' ' && ch != '\n' && ch != '\r';
        }).base(), s.end());
}

static inline void trim(std::string& s)
{
    ltrim(s);
    rtrim(s);
}

static inline std::string ltrim_copy(std::string s)
{
    ltrim(s);
    return s;
}

static inline std::string rtrim_copy(std::string s)
{
    rtrim(s);
    return s;
}

static inline std::string trim_copy(std::string s)
{
    trim(s);
    return s;
}

static int GenerateTagFile(const std::string& tagFilePath)
{
    std::ofstream tagFile(tagFilePath, std::ios_base::binary);
    if (!tagFile)
    {
        std::cerr << "couldn't open the tag file: " << tagFilePath << std::endl;
        return FAIL;
    }
    tagFile << "!!!AUTO GENERATED!!!" << std::endl
        << "it's a dummy file, to make incremental build works" << std::endl;
    return SUCCESS;
}