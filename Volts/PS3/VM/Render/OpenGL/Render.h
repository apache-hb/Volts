#pragma once

#include "PS3/VM/Render/Render.h"

namespace Volts::PS3::RSX
{
    struct OpenGL : Render
    {
        virtual ~OpenGL() override {}

        virtual bool Supported() const { return true; /* i have yet to see a device that doesnt support OpenGL */ }
        virtual RenderDevice* Devices(unsigned& Count) const override { return nullptr; }

        virtual const char* Name() const override { return "OpenGL"; }
        virtual const char* Detail() const override { return "OpenGL is a high level rendering API with great compatibility on windows and linux"; }
    };

    VSCRIPT({
        Render::Register(new OpenGL());
    });
}