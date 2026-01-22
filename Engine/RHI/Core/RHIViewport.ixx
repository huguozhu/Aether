export module Aether:RHIViewport;

import :EngineDefinition;
import :RHIStruct;


export namespace Aether
{

    struct RHIViewport
    {
        RHIViewport() {}
        RHIViewport(int32_t _left, int32_t _top, uint32_t _width, uint32_t _height)
            : left(_left)
            , top(_top)
            , width(_width)
            , height(_height)
        {
        }

        uint32_t Width()
        {
            return width;
        }

        uint32_t Height()
        {
            return height;
        }

        int32_t Left()
        {
            return left;
        }

        int32_t Right()
        {
            return left + width;
        }

        int32_t Top()
        {
            return top;
        }

        int32_t Bottom()
        {
            return top + height;
        }

        bool IsSameSize(const RHIViewport& rhs)
        {
            return Valid() && rhs.Valid() && (width == rhs.width) && (height == rhs.height);
        }

        int32_t left = 0;
        int32_t top = 0;
        uint32_t width = 0;
        uint32_t height = 0;

        bool operator==(const RHIViewport& rhs)
        {
            if (this == &rhs)
                return true;
            else
                return (left == rhs.left) && (top == rhs.top) && (width == rhs.width) && (height == rhs.height);
        }

        bool operator != (const RHIViewport& rhs)
        {
            return !(*this == rhs);
        }

        bool Valid() const
        {
            return (width != 0) && (height != 0);
        }
    };

};
