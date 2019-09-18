#pragma once

#include "Render/Render.h"

#include <vulkan/vulkan.h>
#include "imgui/examples/imgui_impl_vulkan.h"

namespace Volts::RSX
{
    struct Vulkan : Render
    {
        virtual ~Vulkan() {}
        virtual void Start(GUI::Frame* Handle) override;
        virtual void Detach() override;
        virtual const String Name() const override { return "Vulkan"; }
        virtual const String Description() const override { return "Vulkan3D"; }

        virtual void InitGUI() const override
        {
            // TODO
        }

        virtual void NewGUIFrame() const override
        {
            // TODO
        }

        virtual void ShutdownGUI() const override
        {
            // TODO
        }

        virtual void RenderGUI() const override
        {
            // TODO
        }

    private:
        bool Setup();

        bool CreateInstance();

        VkInstance Instance;
    };
}