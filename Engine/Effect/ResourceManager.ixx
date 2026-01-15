export module Aether:ResourceManager;
import :Engine;
import :EngineDefinition;
import :Utils;
import :File;
import :Vector;
import :Format;
import :ShaderHelper;
import std;

export namespace Aether
{
    struct VertexStream {};
    using ResourceID = uint64_t;

    class ResourceManager;
    struct IResource
    {
        using Uninitializer = std::function<void(IResource*)>;

        IResource(ResourceManager* mgr = nullptr);

        virtual ~IResource()
        {
            if (_uninitializer)
                _uninitializer(this);
        }

        void RetainBackendResource(const std::shared_ptr<IResource>& backendRes)
        {
            _backendResources.push_back(backendRes);
        }

        void RetainBackendResource(std::shared_ptr<IResource>&& backendRes)
        {
            _backendResources.push_back(std::forward<std::shared_ptr<IResource>>(backendRes));
        }

        void ReleaseBackendResource()
        {
            _backendResources.clear();
        }

        virtual bool IsAvailable()
        {
            return _isAvailabled;
        }

        void SetAvailable(bool bAvailable)
        {
            _isAvailabled = bAvailable;
        }

        ResourceID _id; // OPT: generate only when resource is valid?
        std::string _name;
        //Initializer _initializer;
        Uninitializer _uninitializer;
        // current resource does not own memory, just a sub-resource of backendResources
        std::vector<std::shared_ptr<IResource>> _backendResources;
        bool _isAvailabled = false;
        ResourceManager* resourceMgr = nullptr;
    };

    struct FileResource : public IResource
    {
        FileResource(char* pRes, int32_t resLength)
        {
            _backendBuffer.resize(resLength);
            memcpy(_backendBuffer.data(), pRes, resLength);
            _data = _backendBuffer.data();
            _size = _backendBuffer.size();
            SetAvailable(true);
        }

        FileResource(const std::string& filepath)
            : _filePath(filepath)
        {
            AResult ret = read_file_content(filepath.c_str(), "rb", _backendBuffer);
            if (!AETHER_CHECKFAILED(ret))
            {
                _data = _backendBuffer.data();
                _size = _backendBuffer.size();
                SetAvailable(true);
            }
        }

        std::vector<uint8_t> _backendBuffer;
        std::string _filePath;
        void* _data = nullptr;
        size_t _size = 0;
    };

    struct BufferResource : public IResource
    {
        void* _data = nullptr;
        size_t _size = 0;
    };

    struct VertexAttributeResource : public IResource
    {
        std::vector<VertexStream> _vertexStreams;
        std::vector<std::shared_ptr<BufferResource>> _vertexBuffers;
    };

    struct VertexIndicesResource : public IResource
    {
        EIndexBufferType _indexBufferType = EIndexBufferType::Unknown;
        uint32_t _indexCount = 0;
        void* _data = nullptr;
        size_t _size = 0;
    };

    struct MorphTargetResource : public IResource
    {
        MorphInfo _morphInfo;
        void* _data = nullptr;
        size_t _size = 0;
        bool _refreshRHIGpuBuffer = false;
    };

    struct MaterialResource : public IResource
    {
        BitmapBufferPtr _albedoImage;
        BitmapBufferPtr _normalImage;
        BitmapBufferPtr _normalMaskImage;
        BitmapBufferPtr _occlusionImage;
        BitmapBufferPtr _metallicRoughnessImage;
        BitmapBufferPtr _emmissiveImage;
        BitmapBufferPtr _clearcoatImage;
        BitmapBufferPtr _clearcoatRoughnessImage;
        BitmapBufferPtr _sheenColorImage;
        BitmapBufferPtr _sheenRoughnessImage;

        float4      _albedoFactor = float4{ 1.0, 1.0, 1.0, 1.0 };
        float       _normalScale = 1.0;
        float4      _normalMaskWeights = float4{ 0.0, 0.0, 0.0, 0.0 };
        float3      _emissiveFactor = float3{ 0.0, 0.0, 0.0 };
        float       _metallicFactor = 1.0;
        float       _roughnessFactor = 1.0;
        float       _clearcoatFactor = 1.0;
        float       _clearcoatRoughnessFactor = 1.0;
        float3      _sheenColorFactor = float3{ 1.0, 1.0, 1.0 };
        float       _sheenRoughnessFactor = 1.0;

        EAlphaMode  _alphaMode = EAlphaMode::Opaque;
        float       _alphaCutoff = 0.5;
        bool        _doubleSided = false;
        float       _IORFactor;
    };

    struct MetaShaderResource : public IResource
    {
        MetaShaderResource(ResourceManager* mgr);
        ~MetaShaderResource() = default;
        AResult Load(const std::string& metaShaderName);

        MetaInfo metaInfo;
    };

    struct ShaderResource : public IResource
    {
        ShaderResource(ResourceManager* mgr);
        ~ShaderResource() = default;
        AResult Load(const std::string& shaderName);

        ReflectInfo     reflectInfo;
        const void* sourceCode = nullptr;
        size_t                          sourceCodeSize = 0;
    };

    class ResourceManager
    {
    public:
        static std::string ResourceRootPath();

        ResourceManager(AetherEngine* engine);
        ~ResourceManager() = default;

        FileResourcePtr             LoadFileResource(const std::string& filePath);
        MetaShaderResourcePtr       LoadMetaShaderResource(const std::string& shaderName);
        ShaderResourcePtr           LoadShaderResource(const std::string& shaderName);

        std::string                 GetShaderMetaPath(const std::string& shaderName);
        std::string                 GetShaderCodePath(const std::string& shaderName);
        std::string                 GetShaderReflectPath(const std::string& shaderName);

        const std::string& GetShaderLanguageStr();

    private:
        AetherEngine* m_pEngine = nullptr;
        std::map<std::string, FileResourcePtr>          m_fileRes;
        std::map<std::string, MetaShaderResourcePtr>    m_metaShaderRes;
        std::map<std::string, ShaderResourcePtr>        m_shaderRes;
    };
    
};
