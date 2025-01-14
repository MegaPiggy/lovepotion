#include "objects/image/image.h"
#include "modules/graphics/graphics.h"

#include "citro2d/citro.h"
#include "common/pixelformat.h"

using namespace love;

Image::Image(const Slices& slices, bool validate) :
    Texture(data.GetTextureType()),
    data(slices),
    mipmapsType(MIPMAPS_NONE),
    sRGB(false)
{
    if (validate && this->data.Validate() == MIPMAPS_DATA)
        mipmapsType = MIPMAPS_DATA;
}

Image::Image(const Slices& slices) : Image(slices, true)
{
    this->Init(slices.Get(0, 0));
}

Image::Image(TextureType type, PixelFormat format, int width, int height, int slices) :
    Image(Slices(type), false)
{
    this->Init(format, width, height);
}

void Image::Init(ImageDataBase* data)
{
    this->Init(data->GetFormat(), data->GetWidth(), data->GetHeight());
}

void Image::Init(PixelFormat format, int width, int height)
{
    this->texture.tex = new C3D_Tex();

    int copyWidth = width;
    if (!::citro2d::IsSizeValid(width))
        copyWidth = NextPO2(width);

    int copyHeight = height;
    if (!::citro2d::IsSizeValid(height))
        copyHeight = NextPO2(height);

    GPU_TEXCOLOR color;
    ::citro2d::GetConstant(format, color);

    if (!C3D_TexInit(this->texture.tex, copyWidth, copyHeight, color))
        throw love::Exception("Failed to initialize texture!");

    size_t copySize = copyWidth * copyHeight * GetPixelFormatSize(format);

    if (this->data.Get(0, 0))
        memcpy(this->texture.tex->data, this->data.Get(0, 0)->GetData(), copySize);
    else
        memset(this->texture.tex->data, 0, copySize);

    C3D_TexFlush(this->texture.tex);

    this->format = format;

    this->width  = width;
    this->height = height;

    this->InitQuad();

    this->SetFilter(this->filter);
    this->SetWrap(this->wrap);
}

void Image::ReplacePixels(const void* data, size_t size, const Rect& rect)
{
    if (!this->texture.tex)
        throw love::Exception("Failed to replace pixels. Texture is uninitialized.");

    if (size == 0)
        throw love::Exception("Failed to replace pixels. Data is nullptr.");

    size_t srcPowTwoWidth  = NextPO2(rect.w);
    size_t srcPowTwoHeight = NextPO2(rect.h);

    if (this->texture.tex->width == srcPowTwoWidth && this->texture.tex->height == srcPowTwoHeight)
    {
        memcpy(this->texture.tex->data, data, size);
        return;
    }

    /* love::Rect should be Po2 already */

    auto getFunction = ImageData::GetPixelGetFunction(this->format);
    auto setFunction = ImageData::GetPixelSetFunction(this->format);

    for (int y = 0; y < rect.h; y++)
    {
        for (int x = 0; x < rect.w; x++)
        {
            unsigned srcIndex = coordToIndex(srcPowTwoWidth, x, y);
            unsigned dstIndex = coordToIndex(this->texture.tex->width, x, y);

            Colorf color {};

            /* grab the pixel data from our source */
            const ImageData::Pixel* srcPixel =
                reinterpret_cast<const ImageData::Pixel*>((uint32_t*)data + srcIndex);
            getFunction(srcPixel, color);

            /* set the pixel we got to ours */
            ImageData::Pixel* dstPixel =
                reinterpret_cast<ImageData::Pixel*>((uint32_t*)this->texture.tex->data + dstIndex);
            setFunction(color, dstPixel);
        }
    }

    C3D_TexFlush(this->texture.tex);
}

Image::~Image()
{
    C3D_TexDelete(this->texture.tex);
    delete this->texture.tex;
}
