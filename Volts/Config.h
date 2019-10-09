#pragma once

namespace Volts::Config
{
    void Load();
    const char* CurrentRender();
}

#define GUIDEBUG 1

// unself debugging
#define UNSELFDEBUG 1

// enable directx 12 debug stuff
#define VDXDEBUG 1

// enable vulkan debug stuff
#define VVKDEBUG 1

// enable metal debug
#define VMTLDEBUG 1
