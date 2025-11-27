export module Aether.Math:ray;

import :vector;

export namespace Aether
{
    class Ray
    {
    public:
        Ray(float3 const& ori, float3 const& dir);

        float3 const& GetOrigin() const { return m_fOrigin; }
        float3 const& GetDirection() const { return m_fDirection; }

    private:
        float3 m_fOrigin;
        float3 m_fDirection;
    };
};
