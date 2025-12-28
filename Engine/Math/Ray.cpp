module Aether:ray;

import :ray;
import :vector;

namespace Aether
{
    Ray::Ray(float3 const& ori, float3 const& dir)
    {
        m_fOrigin = ori;
        m_fDirection = dir;
    }
};