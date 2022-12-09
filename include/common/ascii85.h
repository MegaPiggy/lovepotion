#pragma once

#if defined(__3DS__)
    #include <3ds/types.h>
#elif defined(__SWITCH__)
    #include <switch/types.h>
#endif

namespace love
{
    char* a85_encode(const char* src, size_t srcLength, size_t lineLength, size_t& dstLength);

    char* a85_decode(const char* src, size_t srcLength, size_t& dstLength);
} // namespace love