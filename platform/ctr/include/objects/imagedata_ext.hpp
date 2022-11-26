#pragma once

#include <objects/imagedata/imagedata.tcc>

#include <common/math.hpp>
#include <common/strongreference.hpp>

#include <utilities/formathandler/types/t3xhandler.hpp>

namespace love
{
    template<>
    class ImageData<Console::CTR> : public ImageData<Console::ALL>
    {
      public:
        ImageData(Data* data);

        using ImageData<Console::ALL>::ImageData;

        virtual ~ImageData();

        void Paste(ImageData* data, int x, int y, Rect& source);

        void SetPixel(int x, int y, const Color& color);

        void GetPixel(int x, int y, Color& color) const;

        Color GetPixel(int x, int y) const;

      private:
        void Create(int width, int height, PixelFormat format, void* data = nullptr);

        void Decode(Data* data);

        StrongReference<FormatHandler> decoder;
    };
} // namespace love
