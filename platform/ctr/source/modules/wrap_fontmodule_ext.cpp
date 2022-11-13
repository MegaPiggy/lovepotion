#include <modules/font/wrap_fontmodule.hpp>
#include <modules/fontmodule_ext.hpp>

#include <modules/filesystem/wrap_filesystem.hpp>
#include <objects/data/wrap_data.hpp>

#include <utilities/functions.hpp>

#include <3ds.h>

using namespace love;

using Rasterizer = love::Rasterizer<Console::CTR>;
#define instance() (Module::GetInstance<FontModule<Console::Which>>(Module::M_FONT))

int Wrap_FontModule::NewBCFNTRasterizer(lua_State* L)
{
    ::Rasterizer* self = nullptr;

    if (lua_type(L, 1) == LUA_TNUMBER || lua_isnone(L, 1))
    {
        int size = luaL_optinteger(L, 1, 12);

        luax::CatchException(L, [&]() { self = instance()->NewBCFNTRasterizer(size); });
    }
    else
    {
        Data* data = nullptr;
        std::optional<CFG_Region> region;

        if (luax::IsType(L, 1, love::Data::type))
        {
            data = Wrap_Data::CheckData(L, 1);
            data->Retain();
        }
        else
        {
            const char* filename = luaL_checkstring(L, 1);

            if (love::has_file_extension(filename))
            {
                if (!(region = FontModule<Console::CTR>::systemFonts.Find(filename)))
                    return luax::EnumError(L, "font type", FontModule<Console::CTR>::systemFonts.GetNames(), filename);
            }
            else
                data = Wrap_Filesystem::GetFileData(L, 1);
        }

        int size = (int)luaL_optinteger(L, 2, 12);

        if (region == std::nullopt)
        {
            luax::CatchException(
                L, [&]() { self = instance()->NewBCFNTRasterizer(data, size); },
                [&](bool) { data->Release(); });
        }
        else
            luax::CatchException(L, [&]() { self = instance()->NewBCFNTRasterizer(size, *region); });
    }

    luax::PushType(L, self);
    self->Release();

    return 1;
}

static constexpr luaL_Reg functions[] =
{
    { "newBCFNTRasterizer", Wrap_FontModule::NewBCFNTRasterizer }
};

std::span<const luaL_Reg> Wrap_FontModule::extensions = functions;
