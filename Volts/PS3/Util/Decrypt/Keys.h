#pragma once

#include "Meta/Aliases.h"

namespace Volts::PS3
{
    enum class KeyType : Cthulhu::U32
    {
        Level0 = 1,
        Level1 = 2,
        Level2 = 3,
        App = 4,
        DiskImage = 5,
        Loader = 6,
        Other = 7,
        NPDRM = 8,
    };

    namespace SELF
    {
        struct Key
        {
            Cthulhu::U64 Version;
            Cthulhu::U16 Revision;
            KeyType Type;
            Cthulhu::U8 ERK[32];
            Cthulhu::U8 RIV[16];
            Cthulhu::U8 Public[40];
            Cthulhu::U8 Private[21];
            Cthulhu::U32 CurveType;
        };
    }

    namespace Keys
    {
        static const Cthulhu::U8 PkgAES[] = {
            0x2e, 0x7b, 0x71, 0xd7, 0xc9, 0xc9, 0xa1, 0x4e, 0xa3, 0x22, 0x1f, 0x18, 0x88, 0x28, 0xb8, 0xf8
        };

        static const Cthulhu::U8 PkgAES2[] = {
            0x07, 0xf2, 0xc6, 0x82, 0x90, 0xb5, 0x0d, 0x2c, 0x33, 0x81, 0x8d, 0x70, 0x9b, 0x60, 0xe6, 0x2b
        };

        static const Cthulhu::U8 IDPS[] = {
            0x5E, 0x06, 0xE0, 0x4F, 0xD9, 0x4A, 0x71, 0xBF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01
        };

        static const Cthulhu::U8 FreeKlic[] = {
            0x72, 0xF9, 0x90, 0x78, 0x8F, 0x9C, 0xFF, 0x74, 0x57, 0x25, 0xF0, 0x8E, 0x4C, 0x12, 0x83, 0x87
        };

        static const Cthulhu::U8 Omac2[] = {
            0x6B, 0xA5, 0x29, 0x76, 0xEF, 0xDA, 0x16, 0xEF, 0x3C, 0x33, 0x9F, 0xB2, 0x97, 0x1E, 0x25, 0x6B
        };

        static const Cthulhu::U8 Omac3[] = {
            0x9B, 0x51, 0x5F, 0xEA, 0xCF, 0x75, 0x06, 0x49, 0x81, 0xAA, 0x60, 0x4D, 0x91, 0xA5, 0x4E, 0x97
        };

        static const Cthulhu::U8 Klic[] = {
            0xF2, 0xFB, 0xCA, 0x7A, 0x75, 0xB0, 0x4E, 0xDC, 0x13, 0x90, 0x63, 0x8C, 0xCD, 0xFD, 0xD1, 0xEE
        };

        static const Cthulhu::U8 RIF[] = {
            0xDA, 0x7D, 0x4B, 0x5E, 0x49, 0x9A, 0x4F, 0x53, 0xB1, 0xC1, 0xA1, 0x4A, 0x74, 0x84, 0x44, 0x3B
        };

        static const Cthulhu::U8 PSP1[] = {
            0x2A, 0x6A, 0xFB, 0xCF, 0x43, 0xD1, 0x57, 0x9F, 0x7D, 0x73, 0x87, 0x41, 0xA1, 0x3B, 0xD4, 0x2E
        };

        static const Cthulhu::U8 PSP2[] = {
            0x0D, 0xB8, 0x57, 0x32, 0x36, 0x6C, 0xD7, 0x34, 0xFC, 0x87, 0x9E, 0x74, 0x33, 0x43, 0xBB, 0x4F
        };

        static const Cthulhu::U8 PSX[] = {
            0x52, 0xC0, 0xB5, 0xCA, 0x76, 0xD6, 0x13, 0x4B, 0xB4, 0x5F, 0xC6, 0x6C, 0xA6, 0x37, 0xF2, 0xC1
        };

        static const Cthulhu::U8 RAP[] = {
            0x86, 0x9F, 0x77, 0x45, 0xC1, 0x3F, 0xD8, 0x90, 0xCC, 0xF2, 0x91, 0x88, 0xE3, 0xCC, 0x3E, 0xDF
        };

        static const Cthulhu::U8 RAPBox[] = {
            0x0C, 0x03, 0x06, 0x04, 0x01, 0x0B, 0x0F, 0x08, 0x02, 0x07, 0x00, 0x05, 0x0A, 0x0E, 0x0D, 0x09
        };

        static const Cthulhu::U8 RAP1[] = {
            0xA9, 0x3E, 0x1F, 0xD6, 0x7C, 0x55, 0xA3, 0x29, 0xB7, 0x5F, 0xDD, 0xA6, 0x2A, 0x95, 0xC7, 0xA5
        };

        static const Cthulhu::U8 RAP2[] = {
            0x67, 0xD4, 0x5D, 0xA3, 0x29, 0x6D, 0x00, 0x6A, 0x4E, 0x7C, 0x53, 0x7B, 0xF5, 0x53, 0x8C, 0x74
        };

        static const Cthulhu::U8 SDAT[] = {
            0x0D, 0x65, 0x5E, 0xF8, 0xE6, 0x74, 0xA9, 0x8A, 0xB8, 0x50, 0x5C, 0xFA, 0x7D, 0x01, 0x29, 0x33
        };

        static const Cthulhu::U8 EDATK0[] = {
            0xBE, 0x95, 0x9C, 0xA8, 0x30, 0x8D, 0xEF, 0xA2, 0xE5, 0xE1, 0x80, 0xC6, 0x37, 0x12, 0xA9, 0xAE
        };

        static const Cthulhu::U8 EDATH0[] = {
            0xEF, 0xFE, 0x5B, 0xD1, 0x65, 0x2E, 0xEB, 0xC1, 0x19, 0x18, 0xCF, 0x7C, 0x04, 0xD4, 0xF0, 0x11
        };

        static const Cthulhu::U8 EDATK1[] = {
            0x4C, 0xA9, 0xC1, 0x4B, 0x01, 0xC9, 0x53, 0x09, 0x96, 0x9B, 0xEC, 0x68, 0xAA, 0x0B, 0xC0, 0x81
        };

        static const Cthulhu::U8 EDATH1[] = {
            0x3D, 0x92, 0x69, 0x9B, 0x70, 0x5B, 0x07, 0x38, 0x54, 0xD8, 0xFC, 0xC6, 0xC7, 0x67, 0x27, 0x47
        };

        static const Cthulhu::U8 EDATIV[] = {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
        };

        static const Cthulhu::U8 VSHCurveP[] = {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF
        };

        static const Cthulhu::U8 VSHCurveA[] = {
            0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFC
        };

        static const Cthulhu::U8 VSHCurveB[] = {
            0xA6, 0x8B, 0xED, 0xC3, 0x34, 0x18, 0x02, 0x9C, 0x1D, 0x3C, 0xE3, 0x3B, 0x9A, 0x32, 0x1F, 0xCC, 0xBB, 0x9E, 0x0F, 0x0B
        };

        static const Cthulhu::U8 VSHCurveN[] = {
            0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFE, 0xFF, 0xFF, 0xB5, 0xAE, 0x3C, 0x52, 0x3E, 0x63, 0x94, 0x4F, 0x21, 0x27
        };

        static const Cthulhu::U8 VSHCurveGX[] = {
            0x12, 0x8E, 0xC4, 0x25, 0x64, 0x87, 0xFD, 0x8F, 0xDF, 0x64, 0xE2, 0x43, 0x7B, 0xC0, 0xA1, 0xF6, 0xD5, 0xAF, 0xDE, 0x2C
        };

        static const Cthulhu::U8 VSHCurveGY[] = {
            0x59, 0x58, 0x55, 0x7E, 0xB1, 0xDB, 0x00, 0x12, 0x60, 0x42, 0x55, 0x24, 0xDB, 0xC3, 0x79, 0xD5, 0xAC, 0x5F, 0x4A, 0xDF
        };

        static const Cthulhu::U8 VSHPublic[] = {
            0x62, 0x27, 0xB0, 0x0A, 0x02, 0x85, 0x6F, 0xB0, 0x41, 0x08, 0x87, 0x67, 0x19, 0xE0, 0xA0, 0x18, 0x32, 0x91, 0xEE, 0xB9,
            0x6E, 0x73, 0x6A, 0xBF, 0x81, 0xF7, 0x0E, 0xE9, 0x16, 0x1B, 0x0D, 0xDE, 0xB0, 0x26, 0x76, 0x1A, 0xFF, 0x7B, 0xC8, 0x5B
        };

        static const Cthulhu::U8 SCEPKGRIV[] = { 
            0x4A, 0xCE, 0xF0, 0x12, 0x24, 0xFB, 0xEE, 0xDF, 0x82, 0x45, 0xF8, 0xFF, 0x10, 0x21, 0x1E, 0x6E 
        };

        static const Cthulhu::U8 SCEPKGERK[] = { 
            0xA9, 0x78, 0x18, 0xBD, 0x19, 0x3A, 0x67, 0xA1, 0x6F, 0xE8, 0x3A, 0x85, 0x5E, 0x1B, 0xE9, 0xFB, 0x56, 0x40, 0x93, 0x8D,
            0x4D, 0xBC, 0xB2, 0xCB, 0x52, 0xC5, 0xA2, 0xF8, 0xB0, 0x2B, 0x10, 0x31 
        };

    }

    // get a decryption key from info provided in a SELF file
    // this selects a key from a set of predefined list of keys
    // these keys are known because they were hardcoded into the ps3
    SELF::Key GetSELFKey(KeyType Type, Cthulhu::U16 Revision, Cthulhu::U64 Version);
}