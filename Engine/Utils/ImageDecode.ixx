export module Aether:image_decode;

import :EngineDefinition;
import :buffer;
import :utils;

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