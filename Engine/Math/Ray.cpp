module Aether:Ray;

import :Ray;
import :Vector;

namespace Aether
{
    Ray::Ray(float3 const& ori, float3 const& dir)
    {
        m_fOrigin = ori;
        m_fDirection = dir;
    }
};