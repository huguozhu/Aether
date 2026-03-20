#include "aether.config.h"
#include "SlangProcess.h"
#include "SlangHelper.h"

#if defined(_WIN32)
#include <windows.h>
#include <d3dcompiler.h>
#include <wrl/client.h>
#include <d3d12.h>
#include <d3d12shader.h>
#include "dxcapi.h"
#endif

#include "CLI/CLI.hpp"
#include "shader_content_def.h"

import Aether;
using namespace Aether;

#pragma comment(lib, "dxcompiler.lib")


static MetaInfo _shaderMetaInfo;
struct ExtentPredefine
{
    ExtentPredefine(const MetaPredefine& metaPredefine)
        : name(metaPredefine.name)
        , candidate_values(metaPredefine.candidate_values)
    { }

    ExtentPredefine() {}

    std::string name;
    std::string value;
    std::vector<std::string> candidate_values;
};


class Blob
{
public:
    Blob()
    {
        m_pData = nullptr;
        m_iSize = 0;
    }
    Blob(const void* data, uint32_t size)
        :m_pData(data), m_iSize(size)
    {}

    void Reset()
    {
        m_pData = nullptr;
        m_iSize = 0;
    }
    void Reset(const void* data, uint32_t size)
    {
        m_pData = data;
        m_iSize = size;
    }

    const void* Data() const { return m_pData; }
    uint32_t Size() const { return m_iSize; }

private:
    const void* m_pData = nullptr;
    uint32_t m_iSize = 0;
};
struct IncludeFilePool
{
    const Blob& load_include_file(const std::string& includeFilePath, size_t* hash)
    {
        auto blobIt = _fileBlob.find(includeFilePath);
        if (blobIt != _fileBlob.end())
            return blobIt->second;

        std::string fileContent;
        int success = read_file_content(std::string{ AETHER_SHADER_SOURCE_DIR "/" } + includeFilePath, fileContent);
        std::cout << "load include file: " << includeFilePath << ", success: " << success << std::endl;
        if (SUCCESS != success)
            return _emptyBlob;

        if (hash)
        {
            *hash = std::hash<std::string>{}(fileContent);
        }

        return _fileBlob[includeFilePath] = Blob{ fileContent.data(), (uint32_t)fileContent.size() };
    }

    bool is_loaded(const std::string& includeFilePath)
    {
        return _fileBlob.find(includeFilePath) != _fileBlob.end();
    }

    std::map<std::string, Blob> _fileBlob;
    Blob _emptyBlob;
};

static IncludeFilePool __includeFilePool;

static void WriteByteArray(std::ofstream& outputHeaderFile, const void* data, size_t size, const std::string& varName)
{
    const uint8_t* data_ = (const uint8_t*)data;
    outputHeaderFile
        << "static const uint8_t " << varName << "[] = " << std::endl
        << "{" << std::endl;

    static const size_t perLineCnt = 8;
    size_t wholeLines = size / perLineCnt;
    for (size_t idx = 0; idx < wholeLines; idx++)
    {
        outputHeaderFile << "    " << std::hex
            << "0x" << std::setfill('0') << std::setw(2) << (uint32_t)data_[idx * perLineCnt + 0] << ", "
            << "0x" << std::setfill('0') << std::setw(2) << (uint32_t)data_[idx * perLineCnt + 1] << ", "
            << "0x" << std::setfill('0') << std::setw(2) << (uint32_t)data_[idx * perLineCnt + 2] << ", "
            << "0x" << std::setfill('0') << std::setw(2) << (uint32_t)data_[idx * perLineCnt + 3] << ", "
            << "0x" << std::setfill('0') << std::setw(2) << (uint32_t)data_[idx * perLineCnt + 4] << ", "
            << "0x" << std::setfill('0') << std::setw(2) << (uint32_t)data_[idx * perLineCnt + 5] << ", "
            << "0x" << std::setfill('0') << std::setw(2) << (uint32_t)data_[idx * perLineCnt + 6] << ", "
            << "0x" << std::setfill('0') << std::setw(2) << (uint32_t)data_[idx * perLineCnt + 7] << ", "
            << std::endl;
    }

    size_t lastOffset = wholeLines * perLineCnt;
    size_t lastCnt = size - lastOffset;
    for (size_t idx = 0; idx < lastCnt; idx++)
    {
        if (idx == 0)
            outputHeaderFile << "    ";
        outputHeaderFile << "0x" << std::setfill('0') << std::setw(2) << (uint32_t)data_[idx + lastOffset];
        if (idx != lastCnt - 1)
            outputHeaderFile << ", ";
        else
            outputHeaderFile << std::endl;
    }

    outputHeaderFile << "};" << std::endl;
}

void WriteMacroComment(std::ofstream& file, const std::vector<ExtentPredefine>& fixed_macros, const std::vector<ExtentPredefine>& active_macros)
{
    if (fixed_macros.size() > 0)
    {
        file << "// FIXED MACROS:" << std::endl;
        for (size_t idx = 0; idx != fixed_macros.size(); idx++)
        {
            file << "//   #define " << fixed_macros[idx].name << " " << fixed_macros[idx].value << std::endl;
        }
    }

    if (active_macros.size() > 0)
    {
        file << "// PREDEFINE MACROS:" << std::endl;
        for (size_t idx = 0; idx != active_macros.size(); idx++)
        {
            file << "//   #define " << active_macros[idx].name << " " << active_macros[idx].value << std::endl;
        }
    }
}

struct ParseContent
{
    std::string stageName;
    std::string sourceCode;
    std::vector<ExtentPredefine> extentPredefines;
};

bool ParseShaderCodeFromFile(const char* filePath, struct ParseContent& content)
{
    std::ifstream inputFile(filePath, std::ios_base::binary);
    if (!inputFile)
    {
        std::cerr << "couldn't open the input file: " << filePath << std::endl;
        return false;
    }

    inputFile.seekg(0, std::ios::end);
    content.sourceCode.reserve(static_cast<size_t>(inputFile.tellg()));
    inputFile.seekg(0, std::ios::beg);

    // parse meta information
    std::string lineStr;
    while (std::getline(inputFile, lineStr))
    {
        if (lineStr.find(PREDEFINE_PREFIX) == 0)
        {
            auto predefineStr = trim_copy(lineStr.substr(PREDEFINE_PREFIX.size()));
            MetaPredefine predefine;
            if (ParseMetaPredefine(predefineStr, predefine) != SUCCESS)
                return false;
            _shaderMetaInfo.predefines.push_back(predefine);
            content.extentPredefines.push_back(ExtentPredefine{ predefine });
        }
        else if (lineStr.find(STAGE_PREFIX) == 0)
        {
            content.stageName = trim_copy(lineStr.substr(STAGE_PREFIX.size()));
        }

        content.sourceCode += lineStr + '\n'; // std::getline use '\n' as delimiter, so need to put it back
    }
    return true;
}


int main(int argc, char** argv)
{
    std::cout << "command line:";
    for (int i = 0; i < argc; i++)
    {
        std::cout << " " << argv[i];
    }
    std::cout << std::endl;

    CLI::App app{ "A tool for seek to compile HLSL to many other shader languages", "Shader Compiler" };

    std::string inputFileName;
    app.add_option("--input", inputFileName, "input shader file")
        ->required();

    std::string stageName;
    app.add_option("--stage", stageName, "shader stage")
        ->default_val("vs")
        ->run_callback_for_default()
        ->check(CLI::IsMember({ "vs", "ps", "cs", "gs", "hs", "ds", "ms", "as", "rs", "rm", "rhg", "rc"}));

    std::string entryPoint;
    app.add_option("--entry", entryPoint, "shader entry point")
        ->default_val("main")
        ->run_callback_for_default();

    std::vector<std::string> defines;
    app.add_option("--define", defines, "macro definitions, format: define=value or define");

    bool generateDebugFile = false;
    app.add_flag("--debug", generateDebugFile, "also generate debuggable shader file(no optimize, more readable to debug)");

    std::vector<std::string> targetName;
    app.add_option("--target", targetName, "target platforms to generate, format: t1;t2;t3")
        ->delimiter(';')
        ->check(CLI::IsMember({ "dxil", "spirv", "hlsl", "essl", "msl_macos", "msl_ios"}));

    bool generateDependFile = false;
    // when --depend is specified, only run preprocess, no compile. ignore other options except --define
    app.add_flag("--depend", generateDependFile, "generate depend file for a shader source file instead of source code file");

    bool tightJson = false;
    app.add_flag("--tightjson", tightJson, "generate tighted json for all output json file");

    CLI11_PARSE(app, argc, argv);
    std::cout << "parsed configs: " << std::endl << app.config_to_str();

    try
    {
        bool generateDebugShaderPass = false;
        std::vector<ExtentPredefine> extPredefines;
        std::string shaderSourceDir = AETHER_SHADER_SOURCE_DIR;
        std::string shaderGenerateDir = AETHER_GENERATED_SHADER_DIR;

        size_t delimiterPos = inputFileName.find_last_of('.');
        const auto inputFileBaseName = inputFileName.substr(0, delimiterPos);
        const auto inputFilePath = shaderSourceDir + "/" + inputFileName;

        SlangProcess sp;
        SlangResult sr = sp.Init();
               
        ParseContent content;
        bool b = ParseShaderCodeFromFile(inputFilePath.c_str(), content);
        _shaderMetaInfo.stage = content.stageName;


        std::map<std::string, std::string> dependIncludeFiles;
        auto includeFileHandleFunc = [&](const char* includeFileName)->Blob
        {
            if (__includeFilePool.is_loaded(includeFileName))
                return __includeFilePool.load_include_file(includeFileName, nullptr);

            size_t hash;
            auto& blob = __includeFilePool.load_include_file(includeFileName, &hash);
            if (generateDependFile)
            {
                dependIncludeFiles[includeFileName] = std::to_string(hash);
            }
            return blob;
        };

        std::vector<ExtentPredefine> fixed_macros;
        if (defines.size() > 0)
        {
            fixed_macros.reserve(defines.size());
            for (const auto& define : defines)
            {
                size_t splitPosition = define.find('=');
                if (splitPosition != std::string::npos)
                {
                    std::string macroName = define.substr(0, splitPosition);
                    std::string macroValue = define.substr(splitPosition + 1, define.size() - splitPosition - 1);
                    ExtentPredefine _predefine;
                    _predefine.name = define.substr(0, splitPosition);
                    _predefine.value = define.substr(splitPosition + 1, define.size() - splitPosition - 1);
                    fixed_macros.emplace_back(_predefine);
                }
                else
                {
                    ExtentPredefine _predefine;
                    _predefine.name = define;
                    fixed_macros.emplace_back(_predefine);
                }
            }
        }

        // Compiler::TargetDesc
        //std::vector<slang::TargetDesc> targetDesc;
        //targetDesc.resize(targetName.size());
        //for (size_t outputIdx = 0; outputIdx != targetName.size(); outputIdx++)
        //{
        //    targetDesc[outputIdx].format = ParseShadingLanguage(targetName[outputIdx]);
        //    if (targetDesc[outputIdx].format == SLANG_TARGET_UNKNOWN)
        //    {
        //        std::cerr << "invalid target shading target: " << targetName[outputIdx] << std::endl;
        //        return FAIL;
        //    }
        //}
        sp.SetTargetNames(targetName);

        using CompileFunc = std::function<int(const std::vector<ExtentPredefine>&, const std::vector<ExtentPredefine>&)>;
        using PredefineIt = std::vector<ExtentPredefine>::iterator;
        using IterateFunc = std::function<int(PredefineIt, PredefineIt, PredefineIt, const std::vector<ExtentPredefine>&, std::vector<ExtentPredefine>&, const CompileFunc&)>;

        IterateFunc iterateFunc; // iterateFunc call self recursive, so we need to define it in a single line
        iterateFunc = [&iterateFunc](PredefineIt begin, PredefineIt end, PredefineIt current,
            const std::vector<ExtentPredefine>& fixed_macros, std::vector<ExtentPredefine>& active_macros, const CompileFunc& compileFunc)->int
        {
            if (current == end)
            {
                return compileFunc(fixed_macros, active_macros);
            }

            PredefineIt next = current + 1;
            for (size_t i = 0; i < current->candidate_values.size(); i++)
            {
                active_macros[current - begin].name = current->name;
                active_macros[current - begin].value = current->candidate_values[i];

                int success = iterateFunc(begin, end, next, fixed_macros, active_macros, compileFunc);
                if (success != SUCCESS)
                    return success;
            }
            return SUCCESS;
        };

        std::vector<std::string> allOutFileNames;
        std::vector<std::string> allShaderCodeVarNames;
        std::vector<std::string> allShaderReflectVarNames;
        std::vector<std::string> allShaderCodeLanguageNames;
        auto compileFunc = [&](const std::vector<ExtentPredefine>& fixed_macros, const std::vector<ExtentPredefine>& active_macros)->int
        {
            std::vector<slang::PreprocessorMacroDesc> macroDefines;
            macroDefines.reserve(fixed_macros.size() + active_macros.size());
            for (auto& macro : fixed_macros)
            {
                macroDefines.emplace_back(slang::PreprocessorMacroDesc{ macro.name.c_str(), macro.value.c_str() });
            }
            std::map<std::string, std::string> activeMacroMap;
            for (auto& macro : active_macros)
            {
                macroDefines.emplace_back(slang::PreprocessorMacroDesc{ macro.name.c_str(), macro.value.c_str() });
                activeMacroMap[macro.name] = macro.value;
            }

            size_t seed = 0;
            {
                for (auto mapIt : activeMacroMap)
                {
                    HashRange(seed, mapIt.second.begin(), mapIt.second.end());
                }
            }

            //// 3. 配置并创建会话(Session)，用于保存编译选项和状态 [citation:2]            
            //sessionDesc.targets = targetDesc.data();
            //sessionDesc.targetCount = targetDesc.size();

            //sessionDesc.preprocessorMacros = macroDefines.data();
            //sessionDesc.preprocessorMacroCount = (SlangInt)macroDefines.size();
            // 
            //std::vector<slang::CompilerOptionEntry> options;
            //// 例如：禁用对未使用的函数的优化，以便生成的HLSL更易读
            //options.push_back( { slang::CompilerOptionName::Optimization, { slang::CompilerOptionValueKind::Int, 0, 0, nullptr, nullptr } } );
            //sessionDesc.compilerOptionEntries = options.data();
            //sessionDesc.compilerOptionEntryCount = (SlangInt)options.size();

            //// 创建会话
            //Slang::ComPtr<slang::ISession> session;
            //result = globalSession->createSession(sessionDesc, session.writeRef());
            //if (SLANG_FAILED(result)) {
            //    std::cerr << "Failed to create session" << std::endl;
            //    return -1;
            //}

            //Slang::ComPtr<slang::IModule> slangModule;
            //Slang::ComPtr<slang::IBlob> diagnosticsBlob;
            //slangModule = session->loadModuleFromSourceString(
            //    "TestModule",      // moduleName: 模块的唯一标识符
            //    "test.slang",      // path: 用于诊断和import解析的路径 [citation:2]
            //    source.c_str(), // 源码字符串
            //    diagnosticsBlob.writeRef() // 输出诊断信息
            //);

            //diagnoseIfNeeded(diagnosticsBlob);
            //if (!slangModule) {
            //    std::cerr << "Failed to load module from source string" << std::endl;
            //    return -1;
            //}

            //// 5. 查找入口点 (Entry Point) [citation:2]
            //Slang::ComPtr<slang::IEntryPoint> entryPoint;
            //result = slangModule->findEntryPointByName("main", entryPoint.writeRef());
            //if (SLANG_FAILED(result) || !entryPoint) {
            //    std::cerr << "Failed to find entry point 'main'" << std::endl;
            //    return -1;
            //}

            //std::vector<Compiler::ResultDesc> result(targetName.size());
            //Compiler::Compile(sourceDesc, compileOptions, targetDesc.data(), (uint32_t)targetDesc.size(), &result[0]);

            int compileRet = SUCCESS;
            //for (size_t resultIdx = 0; resultIdx != result.size(); resultIdx++)
            //{
            //    if (result[resultIdx].errorWarningMsg.Size() > 0)
            //    {
            //        const char* msg = reinterpret_cast<const char*>(result[resultIdx].errorWarningMsg.Data());
            //        std::cerr << "Compiler::Compile errorWarningMsg: " << std::string(msg, msg + result[resultIdx].errorWarningMsg.Size()) << std::endl;
            //    }

            //    if (result[resultIdx].hasError)
            //        return FAIL;

            //    if (result[resultIdx].target.Size() == 0)
            //    {
            //        std::cerr << "size of compiled shader is 0, something wrong" << std::endl;
            //        return FAIL;
            //    }

            //    const void* shaderSourceData = result[resultIdx].target.Data();
            //    uint32_t shaderSourceSize = result[resultIdx].target.Size();
            //    
            //    const void* shaderHeaderData = shaderSourceData;
            //    uint32_t shaderHeaderSize = shaderSourceSize;

            //    std::string outputFileDir = shaderGenerateDir + "/" + targetName[resultIdx];
            //    if (generateDebugShaderPass)
            //        outputFileDir += SHADER_DEBUG_DIR_SUFFIX;
            //    
            //    std::string outputFileName = inputFileBaseName;
            //    if (seed != 0)
            //    {
            //        std::stringstream seedStream;
            //        seedStream << "_" << std::hex << seed;
            //        outputFileName += seedStream.str();
            //    }

            //    ///////////////////// shader code - source file /////////////////////
            //    std::string outShaderSourceFilePath = outputFileDir + "/" + outputFileName + shaderLanguageExtMap[targetDesc[resultIdx].language];
            //    std::ofstream outShaderSourceFile(outShaderSourceFilePath, std::ios_base::binary);
            //    if (!outShaderSourceFile)
            //    {
            //        std::cerr << "couldn't open file: " << outShaderSourceFilePath << std::endl;
            //        return FAIL;
            //    }
            //    WriteMacroComment(outShaderSourceFile, fixed_macros, active_macros);
            //    outShaderSourceFile.write(reinterpret_cast<const char*>(shaderSourceData), shaderSourceSize);
            //    std::cout << "shader code - source file saved in: " << outShaderSourceFilePath << std::endl;

            //    ///////////////////// shader code - header file /////////////////////
            //    std::string outShaderHeaderFilePath = outputFileDir + "/" + outputFileName + ".hpp";
            //    std::ofstream outShaderHeaderFile(outShaderHeaderFilePath, std::ios_base::binary);
            //    if (!outShaderHeaderFile)
            //    {
            //        std::cerr << "couldn't open file: " << outShaderHeaderFilePath << std::endl;
            //        return FAIL;
            //    }
            //    WriteMacroComment(outShaderHeaderFile, fixed_macros, active_macros);
            //    outShaderHeaderFile << "#pragma once" << std::endl << "#include <cstdint>" << std::endl << std::endl;

            //    std::string varShaderCodeName;
            //    if (generateDebugShaderPass)
            //        varShaderCodeName = shader_code_name(outputFileName, shaderLanguageMap[targetDesc[resultIdx].language], SHADER_DEBUG_DIR_SUFFIX);
            //    else
            //        varShaderCodeName = shader_code_name(outputFileName, shaderLanguageMap[targetDesc[resultIdx].language], "");
            //    allOutFileNames.push_back(outputFileName);
            //    allShaderCodeVarNames.push_back(varShaderCodeName);
            //    allShaderCodeLanguageNames.push_back(shaderLanguageMap[targetDesc[resultIdx].language]);
            //    WriteByteArray(outShaderHeaderFile, shaderHeaderData, shaderHeaderSize, varShaderCodeName);
            //    std::cout << "shader code - header file saved in: " << outShaderHeaderFilePath << std::endl;

            //    ///////////////////// shader reflect - source file /////////////////////
            //    std::string outReflectSourceFilePath = outputFileDir + "/" + outputFileName + SHADER_REFLECT_FILE_SUFFIX;
            //    ReflectInfo reflectInfo;
            //    if (targetDesc[resultIdx].language == ShadingLanguage::Dxil)
            //        ParseDxilReflectInfo(shaderSourceSize, shaderSourceData, stageName, reflectInfo);
            //    else if (targetDesc[resultIdx].language == ShadingLanguage::SpirV)
            //        ParseSpirvReflectInfo(shaderSourceSize, shaderSourceData, stageName, targetDesc[resultIdx].language, reflectInfo);
            //    std::string reflectJsonContent;
            //    int wr = WriteReflectJson(reflectInfo, tightJson, reflectJsonContent);
            //    if (wr != 0)
            //    {
            //        std::cerr << "write shader reflect json fail, ret: " << wr << std::endl;
            //        return FAIL;
            //    }

            //    std::ofstream outReflectSourceFile(outReflectSourceFilePath, std::ios_base::binary);
            //    if (!outReflectSourceFile)
            //    {
            //        std::cerr << "couldn't open file: " << outReflectSourceFilePath << std::endl;
            //        return FAIL;
            //    }
            //    outReflectSourceFile << reflectJsonContent << std::endl;
            //    std::cout << "shader reflect - source file saved in: " << outReflectSourceFilePath << std::endl;

            //    ///////////////////// shader reflect - header file /////////////////////
            //    // shader reflect header file use the same file with shader code
            //    std::string varShaderReflectName;
            //    if (generateDebugShaderPass)
            //        varShaderReflectName = shader_reflect_name(outputFileName, shaderLanguageMap[targetDesc[resultIdx].language], SHADER_DEBUG_DIR_SUFFIX);
            //    else
            //        varShaderReflectName = shader_reflect_name(outputFileName, shaderLanguageMap[targetDesc[resultIdx].language], "");
            //    allShaderReflectVarNames.push_back(varShaderReflectName);
            //    WriteByteArray(outShaderHeaderFile, reflectJsonContent.data(), reflectJsonContent.size(), varShaderReflectName);
            //    std::cout << "shader reflect - header file saved in: " << outShaderHeaderFilePath << std::endl;
            //}

            return compileRet;
        };

        auto preprocessFunc = [&](const std::vector<ExtentPredefine>& fixed_macros, const std::vector<ExtentPredefine>& active_macros)->int
        {
            std::vector<slang::PreprocessorMacroDesc> macroDefines;
            macroDefines.reserve(fixed_macros.size() + active_macros.size());
            for (auto& macro : fixed_macros)
            {
                macroDefines.emplace_back(slang::PreprocessorMacroDesc{ macro.name.c_str(), macro.value.c_str() });
            }
            //sessionDesc.preprocessorMacroCount = (uint32_t)macroDefines.size();
            //sessionDesc.preprocessorMacros = macroDefines.data();

            //Compiler::ResultDesc result = Compiler::Preprocess(sourceDesc);

            return SUCCESS;
        };

        //// compile and preprocess is mutual exclusion
        //if (!generateDependFile)
        //{
        //    //compileOptions.needReflection = true;
        //    int success = iterateFunc(extPredefines.begin(), extPredefines.end(), extPredefines.begin(), fixed_macros, extPredefines, compileFunc);
        //    if (success != SUCCESS)
        //        return success;

        //    for (size_t targetIdx = 0; targetIdx != targetName.size(); targetIdx++)
        //    {
        //        std::string headerFilePath = shaderGenerateDir + "/" + targetName[targetIdx] + "/" + inputFileBaseName + ".h";
        //        std::ofstream headerFile(headerFilePath, std::ios_base::binary);
        //        if (!headerFile)
        //        {
        //            std::cerr << "couldn't open file: " << headerFilePath << std::endl;
        //            return FAIL;
        //        }
        //        headerFile << "#pragma once" << std::endl;
        //        headerFile << "#include \"shader_content_def.h\"" << std::endl;
        //        for (size_t idx = 0; idx != allOutFileNames.size(); idx++)
        //        {
        //            headerFile << "#include \"" << allOutFileNames[idx] << ".hpp\"" << std::endl;
        //        }
        //        headerFile << std::endl;
        //        headerFile << "#define SHADER_CODE_MAP_GROUP_" << shaderLanguageMap[targetDesc[targetIdx].language] << "_" << inputFileBaseName << " \\" << std::endl;
        //        for (size_t idx = 0; idx != allOutFileNames.size(); idx++)
        //        {
        //            headerFile << "    SHADER_CODE_MAP_MEMBER_VAR(" << allOutFileNames[idx] << "," << allShaderCodeVarNames[idx] << ") \\"<< std::endl;
        //        }
        //        headerFile << std::endl;
        //        headerFile << "#define SHADER_REFLECT_MAP_GROUP_" << shaderLanguageMap[targetDesc[targetIdx].language] << "_" << inputFileBaseName << " \\" << std::endl;
        //        for (size_t idx = 0; idx != allOutFileNames.size(); idx++)
        //        {
        //            headerFile << "    SHADER_REFLECT_MAP_MEMBER_VAR(" << allOutFileNames[idx] << "," << allShaderReflectVarNames[idx] << ") \\"<< std::endl;
        //        }
        //        headerFile << std::endl;
        //    }

        //    if (generateDebugFile)
        //    {
        //        generateDebugShaderPass = true;
        //        compileOptions.optimizationLevel = 0;
        //        compileOptions.disableOptimizations = true;
        //        compileOptions.enableDebugInfo = true;
        //        
        //        allOutFileNames.clear();
        //        allShaderCodeVarNames.clear();
        //        allShaderReflectVarNames.clear();
        //        
        //        success = iterateFunc(extPredefines.begin(), extPredefines.end(), extPredefines.begin(), fixed_macros, extPredefines, compileFunc);
        //        if (success != SUCCESS)
        //            return success;

        //        for (size_t targetIdx = 0; targetIdx != targetName.size(); targetIdx++)
        //        {
        //            std::string headerFilePath = shaderGenerateDir + "/" + targetName[targetIdx] + SHADER_DEBUG_DIR_SUFFIX + "/" + inputFileBaseName + ".h";
        //            std::ofstream headerFile(headerFilePath, std::ios_base::binary);
        //            if (!headerFile)
        //            {
        //                std::cerr << "couldn't open file: " << headerFilePath << std::endl;
        //                return FAIL;
        //            }
        //            headerFile << "#pragma once" << std::endl;
        //            for (size_t idx = 0; idx != allOutFileNames.size(); idx++)
        //            {
        //                headerFile << "#include \"" << allOutFileNames[idx] << ".hpp\"" << std::endl;
        //            }
        //            headerFile << std::endl;
        //            headerFile << "#define SHADER_CODE_MAP_GROUP_" << shaderLanguageMap[targetDesc[targetIdx].language] << "_" << inputFileBaseName << SHADER_DEBUG_DIR_SUFFIX << " \\" << std::endl;
        //            for (size_t idx = 0; idx != allOutFileNames.size(); idx++)
        //            {
        //                headerFile << "    SHADER_CODE_MAP_MEMBER_VAR(" << allOutFileNames[idx] << "," << allShaderCodeVarNames[idx] << ") \\"<< std::endl;
        //            }
        //            headerFile << std::endl;
        //            headerFile << "#define SHADER_REFLECT_MAP_GROUP_" << shaderLanguageMap[targetDesc[targetIdx].language] << "_" << inputFileBaseName << SHADER_DEBUG_DIR_SUFFIX << " \\" << std::endl;
        //            for (size_t idx = 0; idx != allOutFileNames.size(); idx++)
        //            {
        //                headerFile << "    SHADER_REFLECT_MAP_MEMBER_VAR(" << allOutFileNames[idx] << "," << allShaderReflectVarNames[idx] << ") \\"<< std::endl;
        //            }
        //            headerFile << std::endl;
        //        }
        //    }

        //    std::string metaSourceFilePath = AETHER_GENERATED_META_DIR "/" + inputFileBaseName + SHADER_META_FILE_SUFFIX;
        //    std::string metaHeaderFilePath = AETHER_GENERATED_META_DIR "/" + inputFileBaseName + ".h";

        //    ///////////////////// shader meta - source file /////////////////////
        //    std::string metaJsonContent;
        //    int wr = WriteMetaJson(_shaderMetaInfo, tightJson, metaJsonContent);
        //    if (wr != 0)
        //        return wr;

        //    std::ofstream metaSourceFile(metaSourceFilePath, std::ios_base::binary);
        //    if (!metaSourceFile)
        //    {
        //        std::cerr << "couldn't open file: " << metaSourceFilePath << std::endl;
        //        return FAIL;
        //    }
        //    metaSourceFile << metaJsonContent << std::endl;
        //    std::cout << "shader meta - source file saved in: " << metaSourceFilePath << std::endl;

        //    std::ofstream metaHeaderFile(metaHeaderFilePath, std::ios_base::binary);
        //    if (!metaHeaderFile)
        //    {
        //        std::cerr << "couldn't open file: " << metaHeaderFilePath << std::endl;
        //        return FAIL;
        //    }
        //    metaHeaderFile << "#pragma once" << std::endl
        //                   << "#include <cstdint>" << std::endl
        //                   << std::endl;
        //    std::string varShaderMetaName = shader_meta_name(inputFileBaseName);
        //    WriteByteArray(metaHeaderFile, metaJsonContent.data(), metaJsonContent.size(), varShaderMetaName);
        //    std::cout << "shader meta - header file saved in: " << metaHeaderFilePath << std::endl;
        //    
        //    // generate tag file for incremental build
        //    std::string tagFilePath = AETHER_GENERATED_TAG_DIR "/" SHADER_COMPILE_FILE_SUFFIX "/" + inputFileBaseName + SHADER_TAG_FILE_SUFFIX;
        //    int generateRet = GenerateTagFile(tagFilePath);
        //    if (generateRet != SUCCESS)
        //        return generateRet;
        //    std::cout << "tag file saved in: " << tagFilePath << std::endl;
        //}
        //else
        //{
        //    sourceDesc.loadIncludeCallback = includeFileHandleFunc;
        //    int success = iterateFunc(extPredefines.begin(), extPredefines.end(), extPredefines.begin(), fixed_macros, extPredefines, preprocessFunc);
        //    if (success != SUCCESS)
        //        return success;

        //    bool needToGenerateDependFile = false;
        //    std::string dependFilePath = AETHER_GENERATED_DEPEND_DIR "/" + inputFileBaseName + SHADER_DEPEND_FILE_SUFFIX;
        //    std::string oldDependFileContent;
        //    if (SUCCESS != read_file_content(dependFilePath, oldDependFileContent))
        //    {
        //        needToGenerateDependFile = true;
        //        std::cerr << "no depend file, generate a new depend file" << std::endl;
        //        std::cerr << "new dependency:" << std::endl;
        //        for (auto& _new : dependIncludeFiles)
        //        {
        //            std::cerr << "  " << _new.first << "=" << _new.second << std::endl;
        //        }
        //    }
        //    else
        //    {
        //        std::stringstream sstream;
        //        sstream << oldDependFileContent;
        //        std::string dependName;
        //        std::map<std::string, std::string> oldDependIncludeFiles;
        //        while (std::getline(sstream, dependName, ';'))
        //        {
        //            size_t pos = dependName.find('=');
        //            std::string _name = dependName.substr(0, pos);
        //            std::string _hash = dependName.substr(pos + 1);
        //            oldDependIncludeFiles[_name] = _hash;
        //        }
        //        if (dependIncludeFiles != oldDependIncludeFiles)
        //        {
        //            needToGenerateDependFile = true;
        //            std::cerr << "dependent file is changed, regenerate depend file" << std::endl;
        //            std::cerr << "old dependency:" << std::endl;
        //            for (auto& _old : oldDependIncludeFiles)
        //            {
        //                std::cerr << "  " << _old.first << "=" << _old.second << std::endl;
        //            }
        //            std::cerr << "new dependency:" << std::endl;
        //            for (auto& _new : dependIncludeFiles)
        //            {
        //                std::cerr << _new.first << "=" << _new.second << std::endl;
        //            }
        //        }
        //    }

        //    if (needToGenerateDependFile)
        //    {
        //        std::ofstream dependFile(dependFilePath, std::ios_base::binary);
        //        if (!dependFile)
        //        {
        //            std::cerr << "couldn't open the depend file: " << dependFilePath << std::endl;
        //            return FAIL;
        //        }

        //        for (auto it = dependIncludeFiles.begin(); it != dependIncludeFiles.end(); it++)
        //        {
        //            if (it != dependIncludeFiles.begin())
        //                dependFile << ";";
        //            dependFile << it->first << "=" << it->second;
        //        }

        //        std::cout << "shader depend file: " << dependFilePath << std::endl;
        //    }
        //    else
        //    {
        //        std::cerr << "no need to generate depend file" << std::endl;
        //    }

        //    std::string tagFilePath = AETHER_GENERATED_TAG_DIR "/" SHADER_DEPEND_FILE_SUFFIX "/" + inputFileBaseName + SHADER_TAG_FILE_SUFFIX;
        //    int generateRet = GenerateTagFile(tagFilePath);
        //    if (generateRet != SUCCESS)
        //        return generateRet;
        //    std::cout << "tag file saved in: " << tagFilePath << std::endl;
        //}

        return SUCCESS;
    }
    catch (std::exception& ex)
    {
        std::cerr << "exception happened: " << ex.what() << std::endl;
        return FAIL;
    }

    return SUCCESS;
}
