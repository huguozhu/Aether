export module Aether:ShaderHelper;
import <set>;
import <string>;
import <vector>;

export namespace Aether
{
    const std::string PREDEFINE_PREFIX = "//PREDEFINE:";
    const std::string STAGE_PREFIX = "//STAGE:";

    struct JsonWriterBase
    {
        JsonWriterBase(bool bTIghtJson = false);
        virtual ~JsonWriterBase();

        int DoWrite(std::string& content);

        void* Document() { return m_doc; }
        //rapidjson::Document::AllocatorType& Allocator() { return ((rapidjson::Document*)m_doc)->GetAllocator(); }

        void* m_doc; // rapidjson::Document*
        bool m_bTightJson = false;
    };

    struct JsonReaderBase
    {
        JsonReaderBase(const char* jsonBuffer, size_t jsonSize);
        virtual ~JsonReaderBase();

        int DoRead();
        void* Document() { return m_doc; }
        //rapidjson::Document::AllocatorType& Allocator() { return ((rapidjson::Document*)m_doc)->GetAllocator(); }

        void* m_doc;    // rapidjson::Document*
        const char* m_jsonBuffer = nullptr;
        size_t m_jsonSize = 0;
    };

    ////////////////////////////////////////////////////////////////////////////////////////
    ///// shader meta information
    struct MetaPredefine
    {
        std::string name;
        std::vector<std::string> candidate_values;
    };

    struct MetaInfo
    {
        std::string stage;
        std::vector<MetaPredefine> predefines;
    };

    struct MetaJsonWriter : public JsonWriterBase
    {
        MetaJsonWriter(bool bTightJson = false)
            : JsonWriterBase(bTightJson)
        { }

        int Write(const MetaInfo& metaInfo, std::string& content);
        void AddStage(void*/*(rapidjson::Value*)*/ parent, const MetaInfo& metaInfo);
        void AddPredefines(void*/*(rapidjson::Value*)*/ parent, const MetaInfo& metaInfo);
    };

    struct MetaJsonReader : public JsonReaderBase
    {
        MetaJsonReader(const char* jsonBuffer, size_t jsonSize)
            : JsonReaderBase(jsonBuffer, jsonSize)
        { }

        int Read(MetaInfo& metaInfo);
    };

    inline int ParseMetaPredefine(const std::string& input, MetaPredefine& predefine)
    {
        size_t pos = input.find('=');
        if (pos == std::string::npos) {
            //std::cerr << "invalid PREDEFINE input: " << input << ", PREDEFINE must have a delimiter char \'=\'" << std::endl;
            return -__LINE__;
        }

        if (pos == 0) {
            //std::cerr << "invalid PREDEFINE input: " << input << ", PREDEFINE must have a name" << std::endl;
            return -__LINE__;
        }

        predefine.name = input.substr(0, pos);

        std::string candidate_values = input.substr(pos + 1);
        size_t off = 0;
        while (true)
        {
            pos = candidate_values.find(',', off);
            if (pos == std::string::npos)
            {
                predefine.candidate_values.push_back(candidate_values.substr(off));
                break;
            }

            predefine.candidate_values.push_back(candidate_values.substr(off, pos - off));
            off = pos + 1;
        }

        std::set<std::string> _candidate_values(predefine.candidate_values.begin(), predefine.candidate_values.end());
        if (_candidate_values.size() != predefine.candidate_values.size())
        {
            //std::cerr << "invalid PREDEFINE input: " << input << ", contains duplicated value" << std::endl;
            return -__LINE__;
        }

        return 0;
    }

    inline int WriteMetaJson(const MetaInfo& metaInfo, bool bTightJson, std::string& content)
    {
        MetaJsonWriter writer(bTightJson);
        return writer.Write(metaInfo, content);
    }

    ////////////////////////////////////////////////////////////////////////////////////////
    ///// shader reflect information
    enum class CodeType : uint8_t
    {
        SourceCode = 0,
        ByteCode,
    };

    enum class ResourceType : uint8_t
    {
        ConstantBuffer = 0,
        Texture,
        RWTexture,
        Sampler,
        Buffer,
        RWBuffer,
        
    };

    const std::string ResourceTypeStr[] = {
        "cbuffer",
        "texture",
        "sampler",
        "sampled_texture",
        //"srv",
        //"uav",
        "buffer",
        "rwbuffer",
        "rwtexture",
    };

    struct ResourceInfo
    {
        ResourceType type;
        std::string name;
        uint32_t binding;
        uint32_t bindCount;
        uint32_t size;
        std::string fallback_name;
        std::string sampler_name;
        std::string texture_name;
    };

    struct SignatureParameter
    {
        std::string semantic;
        uint32_t semantic_index;
        uint32_t location;
    };

    struct ThreadBlockSize
    {
        uint32_t x = 0;
        uint32_t y = 0;
        uint32_t z = 0;
    };

    struct ReflectInfo
    {
        CodeType code_type;
        std::string stage;
        std::string entry_point;
        std::vector<ResourceInfo> resources;
        std::vector<SignatureParameter> input_signatures;
        std::vector<SignatureParameter> output_signatures;
        ThreadBlockSize block_size; // only valid for cs, otherwise it will be {0, 0, 0}
    };

    struct ReflectJsonWriter : public JsonWriterBase
    {
        ReflectJsonWriter(bool bTightJson = false)
            : JsonWriterBase(bTightJson)
        { }

        int Write(const ReflectInfo& reflectInfo, std::string& content);
    };

    inline int WriteReflectJson(const ReflectInfo& reflectInfo, bool bTightJson, std::string& content)
    {
        ReflectJsonWriter writer(bTightJson);
        return writer.Write(reflectInfo, content);
    }

    struct ReflectJsonReader : public JsonReaderBase
    {
        ReflectJsonReader(const char* jsonBuffer, size_t jsonSize)
            : JsonReaderBase(jsonBuffer, jsonSize)
        { }

        int Read(ReflectInfo& reflectInfo);

    };
    
} // namespace Aether
