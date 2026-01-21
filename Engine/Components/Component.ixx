export module Aether:Component;

import :Engine;
import <string>;

class Entity;
export namespace Aether
{
    enum class ComponentType : uint8_t
    {
        Unknown,
        // 2D
        //Sprite2D,

        // 3D
        Scene,
        Camera,
        LightCamera,
        CsmCamera,
        Light,
        Mesh,
        SkeletalMesh,
        SkyBox,
        Image,
        Animation,
        ParticleSystem,
    };

    class Component
    {
    public:
        std::string const& GetName() const { return m_szName; }
        void SetName(std::string const& name) { m_szName = name; }

        Entity* GetOwner() const { return m_pOwner; }
        void SetOwner(Entity* entity) { m_pOwner = entity; }

        ComponentType GetComponentType() const { return m_eComponentType; }

        virtual ~Component() = default;
        virtual AResult Tick(float delta_time) { return A_Success; }

    protected:
        Component(AetherEngine* engine, std::string const& name = "UnknownComponent", ComponentType type = ComponentType::Unknown)
            : m_pEngine(engine), m_szName(name), m_eComponentType(type)
        {
        }

    protected:
        AetherEngine*           m_pEngine = nullptr;
        std::string             m_szName;
        ComponentType           m_eComponentType = ComponentType::Unknown;
        Entity*                 m_pOwner = nullptr;
    };
};
