export module Aether:ImageDecode;

import :EngineDefinition;
import :Buffer;
import :Utils;

export namespace Aether
{
    enum class ImageType
    {
        UNKNOWN,
        JPEG,
        PNG,
        NUM,
    };

    class ImageDecode
    {
    public:
        static BitmapBufferPtr Decode(uint8_t* data, size_t size, ImageType imageType = ImageType::UNKNOWN);
        static BitmapBufferPtr DecodeFromFile(std::string const& filepath, ImageType imageType = ImageType::UNKNOWN);
    };
};