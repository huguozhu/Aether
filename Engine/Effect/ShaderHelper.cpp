module;
#include <rapidjson/document.h>
#include <rapidjson/filewritestream.h>
#include <rapidjson/writer.h>
#include <rapidjson/prettywriter.h>
#include <rapidjson/reader.h>
#include <rapidjson/filereadstream.h>
#include <rapidjson/stringbuffer.h>

export module Aether:ShaderHelper;
import :ShaderHelper;
import <string>;
import <set>;
import <vector>;

namespace Aether
{
    JsonWriterBase::JsonWriterBase(bool bTIghtJson)
        : m_bTightJson(bTIghtJson)
    {
        m_doc = (void*)new rapidjson::Document();
    }
    JsonWriterBase::~JsonWriterBase()
    {
        if (m_doc)
        {
            delete m_doc;
            m_doc = nullptr;
        }
    }
    int JsonWriterBase::DoWrite(std::string& content)
    {
        rapidjson::StringBuffer writeBuffer;
        if (m_bTightJson)
        {
            rapidjson::Writer<rapidjson::StringBuffer> writer(writeBuffer);
            ((rapidjson::Document*)m_doc)->Accept(writer);
        }
        else
        {
            rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(writeBuffer);
            ((rapidjson::Document*)m_doc)->Accept(writer);
        }

        content = std::string{ writeBuffer.GetString(), writeBuffer.GetSize() };
        return 0;
    }


    JsonReaderBase::JsonReaderBase(const char* jsonBuffer, size_t jsonSize)
        : m_jsonBuffer(jsonBuffer), m_jsonSize(jsonSize)
    {
        m_doc = new rapidjson::Document();
    }
    JsonReaderBase::~JsonReaderBase()
    {
        if (m_doc)
        {
            delete m_doc;
            m_doc = nullptr;
        }
    }
    int JsonReaderBase::DoRead()
    {
        static constexpr uint32_t parseFlags =
            rapidjson::kParseNoFlags |
            rapidjson::kParseCommentsFlag |
            rapidjson::kParseTrailingCommasFlag;
        ((rapidjson::Document*)m_doc)->Parse<parseFlags>(m_jsonBuffer, m_jsonSize);
        auto errorcode = ((rapidjson::Document*)m_doc)->GetParseError();
        if (errorcode != rapidjson::kParseErrorNone)
            return -__LINE__;
        return 0;
    }


    int MetaJsonWriter::Write(const MetaInfo& metaInfo, std::string& content)
    {
        auto& root = ((rapidjson::Document*)Document())->SetObject();

        rapidjson::Value metaVal(rapidjson::kObjectType);
        AddStage(&metaVal, metaInfo);
        AddPredefines(&metaVal, metaInfo);

        root.AddMember("meta", metaVal, ((rapidjson::Document*)m_doc)->GetAllocator());
        return DoWrite(content);
    }

    void MetaJsonWriter::AddStage(void*/*(rapidjson::Value*)*/ parent, const MetaInfo& metaInfo)
    {
        if (metaInfo.stage.empty())
            return;

        ((rapidjson::Value*)parent)->AddMember("stage", rapidjson::StringRef(metaInfo.stage.c_str(), metaInfo.stage.size()), ((rapidjson::Document*)m_doc)->GetAllocator());
    }

    void MetaJsonWriter::AddPredefines(void*/*(rapidjson::Value*)*/ parent, const MetaInfo& metaInfo)
    {
        if (metaInfo.predefines.size() == 0)
            return;

        rapidjson::Value predefinesVal(rapidjson::kArrayType);
        for (auto& predefine : metaInfo.predefines)
        {
            rapidjson::Value predefineVal(rapidjson::kObjectType);
            predefineVal.AddMember("name", rapidjson::StringRef(predefine.name.c_str()), ((rapidjson::Document*)m_doc)->GetAllocator());
            rapidjson::Value candicateVal(rapidjson::kArrayType);
            for (auto& candicate : predefine.candidate_values)
            {
                candicateVal.PushBack(rapidjson::StringRef(candicate.c_str()), ((rapidjson::Document*)m_doc)->GetAllocator());
            }
            predefineVal.AddMember("candidate_values", candicateVal, ((rapidjson::Document*)m_doc)->GetAllocator());
            predefinesVal.PushBack(predefineVal, ((rapidjson::Document*)m_doc)->GetAllocator());
        }
        ((rapidjson::Value*)parent)->AddMember("predefines", predefinesVal, ((rapidjson::Document*)m_doc)->GetAllocator());
    }

    int MetaJsonReader::Read(MetaInfo& metaInfo)
    {
        int ret = DoRead();
        if (ret != 0)
            return ret;

        if (!((rapidjson::Document*)m_doc)->HasMember("meta"))
            return -__LINE__;

        // TODO: validate
        auto& metaVal = (*((rapidjson::Document*)m_doc))["meta"];
        metaInfo.stage = metaVal["stage"].GetString();
        if (metaVal.HasMember("predefines"))
        {
            auto& predefinesVal = metaVal["predefines"];
            for (uint32_t predefineIdx = 0; predefineIdx < predefinesVal.Size(); predefineIdx++)
            {
                auto& predefineVal = predefinesVal[predefineIdx];
                MetaPredefine predefine;
                predefine.name = predefineVal["name"].GetString();
                auto& candidateVal = predefineVal["candidate_values"];
                for (uint32_t candidateIdx = 0; candidateIdx < candidateVal.Size(); candidateIdx++)
                {
                    predefine.candidate_values.push_back(candidateVal[candidateIdx].GetString());
                }
                metaInfo.predefines.push_back(predefine);
            }
        }
        return 0;
    }

    int ReflectJsonWriter::Write(const ReflectInfo& reflectInfo, std::string& content)
    {
        auto& root = (*(rapidjson::Document*)Document()).SetObject();

        rapidjson::Value reflectVal(rapidjson::kObjectType);
        reflectVal.AddMember("code_type", (uint32_t)reflectInfo.code_type, ((rapidjson::Document*)m_doc)->GetAllocator());
        reflectVal.AddMember("stage", rapidjson::StringRef(reflectInfo.stage.c_str()), ((rapidjson::Document*)m_doc)->GetAllocator());
        reflectVal.AddMember("entry_point", rapidjson::StringRef(reflectInfo.entry_point.c_str()), ((rapidjson::Document*)m_doc)->GetAllocator());
        if (reflectInfo.resources.size() > 0)
        {
            rapidjson::Value resourcesVal(rapidjson::kArrayType);
            for (uint32_t resourceIdx = 0; resourceIdx < reflectInfo.resources.size(); resourceIdx++)
            {
                auto& resource = reflectInfo.resources[resourceIdx];
                rapidjson::Value resourceVal(rapidjson::kObjectType);
                resourceVal.AddMember("type", (uint32_t)resource.type, ((rapidjson::Document*)m_doc)->GetAllocator());
                resourceVal.AddMember("name", rapidjson::StringRef(resource.name.c_str()), ((rapidjson::Document*)m_doc)->GetAllocator());
                resourceVal.AddMember("binding", resource.binding, ((rapidjson::Document*)m_doc)->GetAllocator());
                resourceVal.AddMember("bindCount", resource.bindCount, ((rapidjson::Document*)m_doc)->GetAllocator());
                resourceVal.AddMember("id", resource.id, ((rapidjson::Document*)m_doc)->GetAllocator());
                resourceVal.AddMember("space", resource.space, ((rapidjson::Document*)m_doc)->GetAllocator());
                if (resource.size > 0)
                    resourceVal.AddMember("size", resource.size, ((rapidjson::Document*)m_doc)->GetAllocator());
                //if (!resource.fallback_name.empty())
                //    resourceVal.AddMember("fallback_name", rapidjson::StringRef(resource.fallback_name.c_str()), ((rapidjson::Document*)m_doc)->GetAllocator());
                //if (!resource.sampler_name.empty())
                //    resourceVal.AddMember("sampler_name", rapidjson::StringRef(resource.sampler_name.c_str()), ((rapidjson::Document*)m_doc)->GetAllocator());
                //if (!resource.texture_name.empty())
                //    resourceVal.AddMember("texture_name", rapidjson::StringRef(resource.texture_name.c_str()), ((rapidjson::Document*)m_doc)->GetAllocator());
                resourcesVal.PushBack(resourceVal, ((rapidjson::Document*)m_doc)->GetAllocator());
            }
            reflectVal.AddMember("resources", resourcesVal, ((rapidjson::Document*)m_doc)->GetAllocator());
        }
        if (reflectInfo.input_signatures.size() > 0)
        {
            rapidjson::Value inputSignatureVals(rapidjson::kArrayType);
            for (size_t idx = 0; idx != reflectInfo.input_signatures.size(); idx++)
            {
                const SignatureParameter& inputSignature = reflectInfo.input_signatures[idx];
                rapidjson::Value inputSignatureVal(rapidjson::kObjectType);
                inputSignatureVal.AddMember("semantic", rapidjson::StringRef(inputSignature.semantic.c_str()), ((rapidjson::Document*)m_doc)->GetAllocator());
                inputSignatureVal.AddMember("semantic_index", inputSignature.semantic_index, ((rapidjson::Document*)m_doc)->GetAllocator());
                inputSignatureVal.AddMember("location", inputSignature.location, ((rapidjson::Document*)m_doc)->GetAllocator());
                inputSignatureVals.PushBack(inputSignatureVal, ((rapidjson::Document*)m_doc)->GetAllocator());
            }
            reflectVal.AddMember("inputs", inputSignatureVals, ((rapidjson::Document*)m_doc)->GetAllocator());
        }
        if (reflectInfo.output_signatures.size() > 0)
        {
            rapidjson::Value outputSignatureVals(rapidjson::kArrayType);
            for (size_t idx = 0; idx != reflectInfo.output_signatures.size(); idx++)
            {
                const SignatureParameter& outputSignature = reflectInfo.output_signatures[idx];
                rapidjson::Value inputSignatureVal(rapidjson::kObjectType);
                inputSignatureVal.AddMember("semantic", rapidjson::StringRef(outputSignature.semantic.c_str()), ((rapidjson::Document*)m_doc)->GetAllocator());
                inputSignatureVal.AddMember("semantic_index", outputSignature.semantic_index, ((rapidjson::Document*)m_doc)->GetAllocator());
                inputSignatureVal.AddMember("location", outputSignature.location, ((rapidjson::Document*)m_doc)->GetAllocator());
                outputSignatureVals.PushBack(inputSignatureVal, ((rapidjson::Document*)m_doc)->GetAllocator());
            }
            reflectVal.AddMember("outputs", outputSignatureVals, ((rapidjson::Document*)m_doc)->GetAllocator());
        }
        if (reflectInfo.stage == "cs")
        {
            rapidjson::Value blockSizeVal(rapidjson::kArrayType);
            blockSizeVal.PushBack(reflectInfo.block_size.x, ((rapidjson::Document*)m_doc)->GetAllocator());
            blockSizeVal.PushBack(reflectInfo.block_size.y, ((rapidjson::Document*)m_doc)->GetAllocator());
            blockSizeVal.PushBack(reflectInfo.block_size.z, ((rapidjson::Document*)m_doc)->GetAllocator());
            reflectVal.AddMember("block_size", blockSizeVal, ((rapidjson::Document*)m_doc)->GetAllocator());
        }
        root.AddMember("reflect", reflectVal, ((rapidjson::Document*)m_doc)->GetAllocator());
        return DoWrite(content);
    }


    int ReflectJsonReader::Read(ReflectInfo& reflectInfo)
    {
        int ret = DoRead();
        if (ret != 0)
            return ret;

        // TODO: validate
        auto& reflectVal = (*(rapidjson::Document*)m_doc)["reflect"];
        reflectInfo.code_type = (CodeType)reflectVal["code_type"].GetUint();
        reflectInfo.stage = reflectVal["stage"].GetString();
        reflectInfo.entry_point = reflectVal["entry_point"].GetString();
        if (reflectVal.HasMember("resources"))
        {
            auto& resourcesVal = reflectVal["resources"];
            reflectInfo.resources.resize(resourcesVal.Size());
            for (uint32_t idx = 0; idx < resourcesVal.Size(); idx++)
            {
                auto& resourceVal = resourcesVal[idx];
                reflectInfo.resources[idx].type = (ResourceType)resourceVal["type"].GetUint();
                reflectInfo.resources[idx].name = resourceVal["name"].GetString();
                reflectInfo.resources[idx].binding = resourceVal["binding"].GetUint();
                reflectInfo.resources[idx].bindCount = resourceVal["bindCount"].GetUint();
                reflectInfo.resources[idx].id = resourceVal["id"].GetUint();
                reflectInfo.resources[idx].space = resourceVal["space"].GetUint();

                if (resourceVal.HasMember("size"))
                    reflectInfo.resources[idx].size = resourceVal["size"].GetUint();
                //if (resourceVal.HasMember("fallback_name"))
                //    reflectInfo.resources[idx].fallback_name = resourceVal["fallback_name"].GetString();
                //if (resourceVal.HasMember("sampler_name"))
                //    reflectInfo.resources[idx].sampler_name = resourceVal["sampler_name"].GetString();
                //if (resourceVal.HasMember("texture_name"))
                //    reflectInfo.resources[idx].texture_name = resourceVal["texture_name"].GetString();
            }
        }
        if (reflectVal.HasMember("inputs"))
        {
            auto& inputSignatureVals = reflectVal["inputs"];
            reflectInfo.input_signatures.resize(inputSignatureVals.Size());
            for (uint32_t idx = 0; idx < inputSignatureVals.Size(); idx++)
            {
                auto& inputSignatureVal = inputSignatureVals[idx];
                reflectInfo.input_signatures[idx].semantic = inputSignatureVal["semantic"].GetString();
                reflectInfo.input_signatures[idx].semantic_index = inputSignatureVal["semantic_index"].GetUint();
                reflectInfo.input_signatures[idx].location = inputSignatureVal["location"].GetUint();
            }
        }
        if (reflectVal.HasMember("outputs"))
        {
            auto& outputSignatureVals = reflectVal["outputs"];
            reflectInfo.output_signatures.resize(outputSignatureVals.Size());
            for (uint32_t idx = 0; idx < outputSignatureVals.Size(); idx++)
            {
                auto& outputSignatureVal = outputSignatureVals[idx];
                reflectInfo.output_signatures[idx].semantic = outputSignatureVal["semantic"].GetString();
                reflectInfo.output_signatures[idx].semantic_index = outputSignatureVal["semantic_index"].GetUint();
                reflectInfo.output_signatures[idx].location = outputSignatureVal["location"].GetUint();
            }
        }
        if (reflectVal.HasMember("block_size"))
        {
            auto& blockSizeVal = reflectVal["block_size"];
            reflectInfo.block_size.x = blockSizeVal[0].GetUint();
            reflectInfo.block_size.y = blockSizeVal[1].GetUint();
            reflectInfo.block_size.z = blockSizeVal[2].GetUint();
        }
        return 0;
    }

};