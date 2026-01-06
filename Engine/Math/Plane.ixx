export module Aether:Plane;

import :Vector;
import :Matrix;

// Plane Desc Function: ax + by + cz + d = 0
export namespace Aether
{
    class Plane
    {
    public:
        Plane() {}
        Plane(float4 const& rhs);

        float a() const { return m_fPlane[0]; }
        float b() const { return m_fPlane[1]; }
        float c() const { return m_fPlane[2]; }
        float d() const { return m_fPlane[3]; }
        float4 abcd() const { return m_fPlane; }

        Plane& operator=(float4 const& rhs);

        float3 Normal() const;
        void Normal(float3 const& rhs);
        Plane Normalize() const;

        // distance from point to plane
        float GetDistance(float3 const& point) const;

    private:
        float4 m_fPlane;
    };
};
