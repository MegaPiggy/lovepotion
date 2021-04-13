#pragma once

#include "common/lmath.h"
#include "objects/canvas/canvas.h"
#include <citro2d.h>

class citro2d
{
  private:
    citro2d();

  public:
    static citro2d& Instance();

    ~citro2d();

    void BindFramebuffer(love::Canvas* canvas = nullptr);

    void ClearColor(const Colorf& color);

    void SetScissor(GPU_SCISSORMODE mode, const love::Rect& scissor, int screenWidth,
                    bool canvasActive);

    void SetStencil(GPU_TESTFUNC compare, int value);

    void Present();

    void SetTextureFilter(const love::Texture::Filter& filter);

    void SetTextureFilter(love::Texture* texture, const love::Texture::Filter& filter);

    void SetTextureWrap(const love::Texture::Wrap& wrap);

    void SetTextureWrap(love::Texture* texture, const love::Texture::Wrap& filter);

    void Set3D(bool enable);

    bool Get3D() const;

    static GPU_TEXTURE_WRAP_PARAM GetCitroWrapMode(love::Texture::WrapMode wrap);

  private:
    std::vector<C3D_RenderTarget*> targets;
    C3D_RenderTarget* current;

    struct
    {
        GPU_TEXTURE_FILTER_PARAM min;
        GPU_TEXTURE_FILTER_PARAM mag;

        GPU_PROCTEX_FILTER mipMap;
    } filter;

    struct
    {
        GPU_TEXTURE_WRAP_PARAM s;
        GPU_TEXTURE_WRAP_PARAM t;
    } wrap;

    bool inFrame = false;

    void EnsureInFrame();
};
