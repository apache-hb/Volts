#pragma once

#include "platform.h"
#include "types.h"

#include <vector>
#include <cstdint>

namespace svl::convert
{
    constexpr u16 to_u16(const char* str)
    {
        return *(u16*)str;
    }

    constexpr u32 to_u32(const char* str)
    {
        return *(u32*)str;
    }

    constexpr u64 to_u64(const char* str)
    {
        return *(u64*)str;
    }

    template<typename T>
    std::vector<byte> as_bytes(T dat)
    {
        std::vector<byte> val(sizeof(T));
        memcpy(val.data(), &dat, sizeof(T));
        return val;
    }
}