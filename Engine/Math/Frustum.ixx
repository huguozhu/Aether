export module Aether:Frustum;

import :Vector;
import :Matrix;
import :AABBox;
import :Plane;
import :utils;

export namespace Aether
{
    enum class VisibleMark : uint8_t
    {
        Yes,
        No,
        Partial,
    };

    export class Frustum
    {
    public:
        Frustum() {}

        void ClipMatrix(Matrix4 const& clip, Matrix4 const& inv_clip);
        VisibleMark Intersect(AABBox const& aabb) const;

        std::string Str() const;

    private:
        std::array<Plane, 6> m_aPlanes;
        std::array<float3, 8> m_aCorners;
    };

};