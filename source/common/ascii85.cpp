#include "common/ascii85.h"
#include "common/exception.h"

#include <limits>
#include <stdio.h>

using namespace love;

// Translation table as described in the ASCII85 specification
static const char c85[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz!#$%&()*+-;<=>?@^_`{|}~";

// Translation table to encode as described in Adobe's specification
static const char ca85[] = "!\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

// Translation table to decode
static const char cd85[] =
    "|$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~!\"#$";

// Encode 4 8-bit binary bytes as 5 '7-bit' characters
static void a85_encode_block(char in[4], char out[5], int len)
{
    // Encoding 4 bytes into 5 characters
    uint32_t value = (in[0] << 24) | (in[1] << 16) | (in[2] << 8) | in[3];
    out[0] = (char)ca85[(int)(value / 52200625) % 85];
    out[1] = (char)ca85[(int)(value / 614125) % 85];
    out[2] = (char)ca85[(int)(value / 7225) % 85];
    out[3] = (char)ca85[(int)(value / 85) % 85];
    out[4] = (char)ca85[(int)(value % 85)];

    // Handle cases where the input was not a multiple of 4 bytes
    for (int i = 0; i < 5 - len; i++)
        out[i] = 'z';
}

char* love::a85_encode(const char* src, size_t srclen, size_t linelen, size_t& dstlen)
{
    if (linelen == 0)
        linelen = std::numeric_limits<size_t>::max();

    size_t blocksout = 0;
    size_t srcpos    = 0;

	size_t adjustment = (srclen % 4) ? (4 - (srclen % 4)) : 0;
	size_t paddedlen = ((srclen + adjustment) / 4) * 5;

    dstlen = paddedlen + paddedlen / linelen;

    if (dstlen == 0)
        return nullptr;

    char* dst = nullptr;

    try
    {
        dst = new char[dstlen + 1];
    }
    catch (std::bad_alloc&)
    {
        throw love::Exception("Out of memory.");
    }

    size_t dstpos = 0;

    while (srcpos < srclen)
    {
        char in[4]  = { 0 };
        char out[5] = { 0 };

        int len = 0;

        for (int i = 0; i < 4; i++)
        {
            if (srcpos >= srclen)
                break;

            in[i] = src[srcpos++];
            len++;
        }

        if (len > 0)
        {
            a85_encode_block(in, out, len);

            for (int i = 0; i < 5 && dstpos < dstlen; i++, dstpos++)
                dst[dstpos] = out[i];

            blocksout++;
        }

        if (blocksout >= linelen / 5 || srcpos >= srclen)
        {
            if (blocksout > 0 && dstpos < dstlen)
                dst[dstpos++] = '\n';

            blocksout = 0;
        }
    }

    dst[dstpos] = '\0';
    return dst;
}

static void a85_decode_block(char in[5], char out[4])
{
    // Decoding 5 characters into 4 bytes
    uint32_t value = (cd85[(int)in[0]] * 52200625) + (cd85[(int)in[1]] * 614125) + (cd85[(int)in[2]] * 7225) + (cd85[(int)in[3]] * 85) + cd85[(int)in[4]];
    out[0] = (char)(value >> 24);
    out[1] = (char)((value >> 16) & 0xff);
    out[2] = (char)((value >> 8) & 0xff);
    out[3] = (char)(value & 0xff);

    // Handle cases where the input was not a multiple of 5 characters
    if (in[0] == 'z')
        out[0] = out[1] = out[2] = 0;
    else if (in[1] == 'z')
        out[1] = out[2] = 0;
    else if (in[2] == 'z')
        out[2] = 0;
}

char* love::a85_decode(const char* src, size_t srclen, size_t& size)
{
    size_t paddedsize = (srclen / 5) * 4;
    size = paddedsize;

    char* dst = nullptr;
    try
    {
        dst = new char[paddedsize];
    }
    catch (std::bad_alloc&)
    {
        throw love::Exception("Out of memory.");
    }

    char* d = dst;

    char in[5]  = { 0 };
    char out[4] = { 0 };
    size_t i, len, dstpos = 0, srcpos = 0;

    while (srcpos <= srclen)
    {
        char in[5]  = { 0 };
        char out[4] = { 0 };

        int len = 0;

        for (int i = 0; i < 5; i++)
        {
            if (srcpos >= srclen)
                break;

            in[i] = src[srcpos++];
            len++;
        }

        
        if (len > 0)
        {
            a85_decode_block(in, out);

            for (int i = 0; i < 4 && dstpos < size; i++, dstpos++)
                dst[dstpos] = out[i];
        }
    }

    return dst;
}
