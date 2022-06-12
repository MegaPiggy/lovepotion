#pragma once

#include "common/bidirectionalmap.h"
#include "common/colors.h"
#include "common/render/vertex.h"
#include "common/vector.h"

#include "deko3d/common.h"

#include <array>
#include <memory>
#include <span>

using namespace love;

namespace VertexAttributes
{
    // clang-format off

    /* Primitives */

    constexpr std::array<DkVtxBufferState, 1> PrimitiveBufferState =
    {
        DkVtxBufferState { Vertex::PRIM_VERTEX_SIZE, 0 },
    };

    constexpr std::array<DkVtxAttribState, 2> PrimitiveAttribState =
    {
        DkVtxAttribState { 0, 0, offsetof(Vertex::PrimitiveVertex, position), DkVtxAttribSize_3x32, DkVtxAttribType_Float, 0 },
        DkVtxAttribState { 0, 0, offsetof(Vertex::PrimitiveVertex, color),    DkVtxAttribSize_4x32, DkVtxAttribType_Float, 0 }
    };

    /*  Textures */

    constexpr std::array<DkVtxBufferState, 1> TextureBufferState = {
        DkVtxBufferState { Vertex::PRIM_VERTEX_SIZE, 0 },
    };

    constexpr std::array<DkVtxAttribState, 3> TextureAttribState =
    {
        DkVtxAttribState { 0, 0, offsetof(Vertex::PrimitiveVertex, position), DkVtxAttribSize_3x32, DkVtxAttribType_Float, 0 },
        DkVtxAttribState { 0, 0, offsetof(Vertex::PrimitiveVertex, color),    DkVtxAttribSize_4x32, DkVtxAttribType_Float, 0 },
        DkVtxAttribState { 0, 0, offsetof(Vertex::PrimitiveVertex, texcoord), DkVtxAttribSize_2x16, DkVtxAttribType_Unorm, 0 }
    };
    // clang-format on

    struct Attribs
    {
        dk::detail::ArrayProxy<const DkVtxAttribState> attributeState;
        dk::detail::ArrayProxy<const DkVtxBufferState> bufferState;
    };

    inline Attribs GetConstant(Vertex::CommonFormat inFormat)
    {
        Attribs attributes {};

        switch (inFormat)
        {
            case Vertex::CommonFormat::RGBAub:
                attributes.attributeState = PrimitiveAttribState;
                attributes.bufferState    = PrimitiveBufferState;
                break;
            case Vertex::CommonFormat::STf_RGBAub:
                attributes.attributeState = TextureAttribState;
                attributes.bufferState    = TextureBufferState;
                break;
            default:
                break;
        }

        return attributes;
    }
} // namespace VertexAttributes
